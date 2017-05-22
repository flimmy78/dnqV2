/* dnq common Program
 * 
 *  Copyright (c) 2017 yuchen
 *  Copyright 2017-2017 jiuzhoutech Inc.
 *  yuchen  <yuchen@jiuzhoutech.com, 382347665@qq.com>
 * 
 *  this is a common interface API, for app.
 * Note : 
 */


#include "dnq_common.h"
#include "ngx_palloc.h"
#include "dnq_checksum.h"
#include "cJSON.h"

#include <sys/time.h>
#include <sys/types.h>  
#include <sys/wait.h>

static U32 g_dnq_init = 0;
static ngx_pool_t *mem_pool;

static U32 init_time_ms = 0;

S32 dnq_time_init()
{
    struct timeval   tv;

    if (gettimeofday(&tv, NULL) == 0)
    {
        init_time_ms =  tv.tv_sec*1000 + tv.tv_usec/1000;
    }
    else
    {
        perror("gettimeofday falied");
        return -1;
    }
    return 0;
}

U32 dnq_time_now()
{
    struct timeval  tv;
    U32 Clk = 1;	   /* Default value */

    if (gettimeofday(&tv, NULL) == 0)
    {
        Clk =  tv.tv_sec*1000 + tv.tv_usec/1000;
    }
    else
    {
        perror("gettimeofday falied");
    }

    return Clk - init_time_ms;
}

U32 dnq_time_now_us()
{
    struct timeval   tv;
    U32  Clk = 1;	  /* Default value */

    if (gettimeofday(&tv, NULL) == 0)
    {
        Clk =  tv.tv_sec*1000*1000 + tv.tv_usec;
    }
    else
    {
        perror("gettimeofday falied");
    }

    return Clk - init_time_ms*1000;
}

S32 dnq_system_call(U8 *command)
{
    S32 status;

    status = system(command);
    
    if (-1 == status)
    {
        printf(" system run error! err=%s\n", strerror(errno));
		return -1;
    }
    else
    {
        //printf("[dnq_system_call] exit status value = [0x%x]\n", status);
        if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
                printf("[dnq_system_call]run shell script successfully.\n");
                return 0;
            }
            else
            {
                printf("[dnq_system_call]run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
				return -1;
            }
        }
        else
        {
            printf("[dnq_system_call]exit status = [%d]\n", WEXITSTATUS(status));
			return -1;
        }
    }

    return 0;
}

S32 dnq_init()
{
    ngx_pool_t * pool = NULL;

    if(g_dnq_init)
        return 0;
    
    dnq_time_init();
    dnq_checksum_init();
    
    pool = dnq_mempool_init(1024*1024);
    if(!pool)
        return -1;
    
    mem_pool = pool;
    g_dnq_init = 1;
    return 0;
}

S32 dnq_deinit()
{
    g_dnq_init = 0;
    dnq_mempool_deinit(mem_pool);
    return 0;
}

