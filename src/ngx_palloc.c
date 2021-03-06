
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include "ngx_palloc.h"
#include "dnq_log.h"

#define KB_SIZE   (1024)
#define MB_SIZE   (1024*1024)

ngx_uint_t  ngx_pagesize = 4096;
ngx_uint_t  ngx_pagesize_shift;
ngx_uint_t  ngx_cacheline_size;
static ngx_pool_t *os_mem_pool = NULL;
static ngx_pool_t *app_mem_pool = NULL;
static U32 malloc_sem = 0;


static void *ngx_palloc_block(ngx_pool_t *pool, size_t size);
static void *ngx_palloc_large(ngx_pool_t *pool, size_t size);
static size_t ngx_pool_free_size(ngx_pool_t *pool);
void dnq_pool_check(ngx_pool_t *pool);

S32 dnq_mempool_init()
{
    ngx_pool_t *pool = NULL;

    malloc_sem = 0;
    if(os_mem_pool || app_mem_pool)
    {
        DNQ_ERROR(DNQ_MOD_ALL, "mempool already exist! os_mempool=0x%0x8, app_mempool=0x%08x\n",\
            os_mem_pool, app_mem_pool);
        return 0;
    }

    /* create os mem pool, it is used by dnq_os_malloc, dnq_os_free */
    pool= ngx_create_pool(DNQ_OS_MEM_POOL_SIZE);
    if(pool == NULL)
    {
        DNQ_ERROR(DNQ_MOD_ALL, "create os mempol error!!");
        return -1;
    }
    os_mem_pool = pool;

    DNQ_INFO(DNQ_MOD_ALL, "create os mempool success! base_addr=0x%08x, size=%d(%dM)", \
        pool, DNQ_OS_MEM_POOL_SIZE, DNQ_OS_MEM_POOL_SIZE/MB_SIZE);

    /* create app mem pool, it is used by dnq_malloc, dnq_free */
    pool= ngx_create_pool(DNQ_APP_MEM_POOL_SIZE);
    if(pool == NULL)
    {
        ngx_destroy_pool(os_mem_pool);
        DNQ_ERROR(DNQ_MOD_ALL, "create app mempool error!!");
        return -1;
    }
    app_mem_pool = pool;

    DNQ_INFO(DNQ_MOD_ALL, "create app mempool success! base_addr=0x%08x, size=%d(%dM)", \
        pool, DNQ_APP_MEM_POOL_SIZE, DNQ_APP_MEM_POOL_SIZE/MB_SIZE);
    
    return 0;
}

void dnq_mempool_deinit()
{
    if(app_mem_pool)
        ngx_destroy_pool(app_mem_pool);
    if(os_mem_pool)
        ngx_destroy_pool(os_mem_pool);
    app_mem_pool = NULL;
    os_mem_pool = NULL;
    malloc_sem = 0;
    DNQ_INFO(DNQ_MOD_ALL, "destroy all mempool success!");
}

void *dnq_os_malloc(size_t size)
{
    //return malloc(size);
    void *p = NULL;
    p = ngx_palloc(os_mem_pool, size);
    if(!p)
    {
        printf("####dnq_os_malloc error!!\n");
        ngx_destroy_pool(os_mem_pool);
        exit(1);
    }
    //printf(" dnq_os_malloc: ptr=0x%08x, len=%d\n", p, size);
    return p;
}

void dnq_os_free(void *ptr)
{
    //printf(" os_free: ptr=0x%08x\n", ptr);
    if(ptr)
    {
        ngx_pfree(os_mem_pool, ptr);
        ptr = NULL;
    }
    return ;
}

void *dnq_malloc(size_t size)
{
    //return malloc(size);
    void *p = NULL;
    p = ngx_palloc(app_mem_pool, size);
    if(!p)
    {
        printf("####dnq_malloc error!!\n");
        ngx_destroy_pool(app_mem_pool);
        exit(1);
    }
    malloc_sem++;
    //printf("malloc size=%d, freesize=%d\n", size, ngx_pool_free_size(app_mem_pool));
    //printf(" dnq_malloc: ptr=0x%08x, len=%d\n", p, size);
    return p;
}

void dnq_free(void *ptr)
{
    //printf(" dnq_free: ptr=0x%08x\n", ptr);
    if(ptr)
    {
        ngx_pfree(app_mem_pool, ptr);
        ptr = NULL;
        malloc_sem--;
        /* memory check maybe reset*/
        dnq_pool_check(app_mem_pool);
    }
    return ;
}

void dnq_pool_check(ngx_pool_t *pool)
{
    if(malloc_sem == 0){
        if(ngx_pool_free_size(pool) < 1024) {
            ngx_reset_pool(pool);
            DNQ_INFO(DNQ_MOD_ALL, "mempool almost run out, reset pool!!!");
        }
    }
}
static size_t ngx_pool_free_size(ngx_pool_t *pool)
{
    U32 cnt = 0;
    ngx_pool_t *p;

    cnt = 0;
    for (p = pool; p; p = p->d.next) {
        cnt++;
        if(p == pool->current)
            break;
    }
    if(cnt > 1){
        DNQ_ERROR(DNQ_MOD_ALL, "mempool is too many! pool count=%d", cnt);
    }

    p = pool->current;
    return (size_t)(p->d.end - p->d.last);
}
    
void *
ngx_alloc(size_t size)
{
    void  *p;

    p = malloc(size);
    if (p == NULL) {
        DNQ_ERROR(DNQ_MOD_ALL, "malloc(%uz) failed", size);
    }

    //DNQ_INFO(DNQ_MOD_ALL, "malloc: %p:%uz", p, size);

    return p;
}

ngx_pool_t *
ngx_create_pool(size_t size)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size);
    if (p == NULL) {
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    //p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    //p->log = log;

    return p;
}


void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
            //               "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) {

        //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);

        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

#if (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        //ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
        //               "free: %p, unused: %uz", p, p->d.end - p->d.last);

        if (n == NULL) {
            break;
        }
    }

#endif

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        if (n == NULL) {
            break;
        }
    }
}


void
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    for (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + sizeof(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    //pool->chain = NULL;
    pool->large = NULL;
}


void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    if (size <= pool->max) {

        p = pool->current;

        do {
            m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);

            if ((size_t) (p->d.end - m) >= size) {
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;

        } while (p);

        return ngx_palloc_block(pool, size);
    }

    return ngx_palloc_large(pool, size);
}


void *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    if (size <= pool->max) {

        p = pool->current;

        do {
            m = p->d.last;

            if ((size_t) (p->d.end - m) >= size) {
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;

        } while (p);

        return ngx_palloc_block(pool, size);
    }

    return ngx_palloc_large(pool, size);
}


static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize);
    if (m == NULL) {
        return NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    for (p = pool->current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;

    return m;
}


static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    DNQ_ERROR(DNQ_MOD_ALL, "###dnq alloc large memory!!!");
    exit(1);
    p = ngx_alloc(size);
    if (p == NULL) {
        return NULL;
    }

    n = 0;

    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

    large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}


void *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size);
    if (p == NULL) {
        return NULL;
    }

    large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}

ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (p == l->alloc) {
            //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
            //               "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = NULL;

            return NGX_OK;
        }
    }

    return NGX_DECLINED;
}


void *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;

    p = ngx_palloc(pool, size);
    if (p) {
        ngx_memzero(p, size);
    }

    return p;
}

// not use it
#if USE_POOL_CLEANUP

ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, sizeof(ngx_pool_cleanup_t));
    if (c == NULL) {
        return NULL;
    }

    if (size) {
        c->data = ngx_palloc(p, size);
        if (c->data == NULL) {
            return NULL;
        }

    } else {
        c->data = NULL;
    }

    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    return c;
}


void
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    for (c = p->cleanup; c; c = c->next) {
        if (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            if (cf->fd == fd) {
                c->handler(cf);
                c->handler = NULL;
                return;
            }
        }
    }
}


void
ngx_pool_cleanup_file(void *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    if (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


void
ngx_pool_delete_file(void *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    if (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        if (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    if (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}
#endif

#if 0

static void *
ngx_get_cached_block(size_t size)
{
    void                     *p;
    ngx_cached_block_slot_t  *slot;

    if (ngx_cycle->cache == NULL) {
        return NULL;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    if (slot->number) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->number--;
        return p;
    }

    return NULL;
}

#endif
