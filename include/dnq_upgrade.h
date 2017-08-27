#ifndef _DNQ_UPGRADE_H_
#define _DNQ_UPGRADE_H_

#include "common.h"
#include <pthread.h>


#define UPGRD_INFO_LEN      20

#define UPGRD_FILE    "upgrade_file"
#define UPGRD_TAG     0x47

#define ERR_BASE        10             /* ��������ʼֵ */
#define ERR_LENGHT     -(ERR_BASE+1)   /* ��������ݳ��� */
#define ERR_TAG        -(ERR_BASE+2)   /* �����TAG */
#define ERR_TYPE       -(ERR_BASE+3)   /* ��������� */
#define ERR_MAC        -(ERR_BASE+4)   /* MAC��ַ��ƥ�� */
#define ERR_HWVER      -(ERR_BASE+5)   /* Ӳ���汾��ƥ�� */
#define ERR_SWVER      -(ERR_BASE+6)   /* ����汾��ƥ�� */
#define ERR_CRC        -(ERR_BASE+7)   /* �����У��ֵCRC32 */
#define ERR_MODE       -(ERR_BASE+8)   /* ���������ģʽ */

#define ERR_WRITE      -(ERR_BASE+10)   /* дflash���� */
#define ERR_DECOMPRESS -(ERR_BASE+11)   /* ��ѹ���� */


#define DBG_NONE      0
#define DBG_ERROR     1
#define DBG_WARN      2
#define DBG_DEBUG     3
#define DBG_INFO      4
#define DBG_ALL       5


#define upgrd_error_en(en, msg) \
    do { errno = en; perror(msg); return (en); } while (0)

#define upgrd_error(ret, msg) \
    do { perror(msg); return (ret); } while (0)

S32 upgrd_debug(U32 lever, const char *fmt, ...);
#define UPGRD_ERROR( msg,...)  upgrd_debug(DBG_ERROR, "[@UPGRD_ERROR]%s:%d: " msg "\n",__func__,__LINE__, ## __VA_ARGS__)
#define UPGRD_WARN( msg,...)   upgrd_debug(DBG_WARN, "[@UPGRD_WARN]%s:%d: " msg "\n",__func__,__LINE__, ## __VA_ARGS__)
#define UPGRD_DEBUG( msg,...)  upgrd_debug(DBG_DEBUG, "[@UPGRD_DEBUG]%s:%d: " msg "\n",__func__,__LINE__, ## __VA_ARGS__)
#define UPGRD_INFO( msg,...)   upgrd_debug(DBG_DEBUG, "[@UPGRD_INFO]%s:%d: " msg "\n",__func__,__LINE__, ## __VA_ARGS__)
#define UPGRD_PRINT( msg,...)  upgrd_debug(DBG_ALL, msg)


typedef struct _upgrd_info
{
    U8  tag;           /* upgrd tag, 0x47 */
    U8  upgrade_type;  /* upgrd type, 0x10:app 0x11:kernel: 0x12:rootfs 0x13:data */
    U8  file_type;     /* compress type, 0:tar  1:tar.gz  2:tar.bz2  3:zip*/
    U8  mode;          /* 1:�߰汾���� 2:ָ���汾���� */
    U16 need_ver;      /* control=1ʱ, ����汾��Ϊneed_ver���������� */
    U8  mac[6];        /* host mac addr */
    U16 hw_ver;        /* hardware version */
    U16 sw_ver;        /* software version */
    U32 crc_32;        /* checksum У�� */
}upgrd_info_t;

typedef enum _upgrd_type
{
    UPGRD_TYPE_BOOT,
    UPGRD_TYPE_ENV,
    UPGRD_TYPE_KERNEL,
    //UPGRD_TYPE_ROOTFS,  /* Unsupport */
    UPGRD_TYPE_APP = 0x10,
    UPGRD_TYPE_DNQ_MANAGE,
    UPGRD_TYPE_DNQ_CONFIG,
    UPGRD_TYPE_DNQ_UPGRADE,
    UPGRD_TYPE_MAX,
}upgrd_type_e;

typedef enum _file_type
{
    FILE_TYPE_TAR,
    FILE_TYPE_TAR_GZ,
    FILE_TYPE_TAR_BZ2,
    FILE_TYPE_ZIP,
    FILE_TYPE_MAX
}file_type_e;

typedef enum _msg_type_e
{
    UPGRD_MSG_TYPE_DESC,
    UPGRD_MSG_TYPE_DATA

}msg_type_e;

#define UPGRD_MSG_LEN_MAX  512
typedef struct _upgrd_msg
{
    U16 type;          /* msg type */
    U16 is_processing; /* message is release or processing */ 
    U8  msg[UPGRD_MSG_LEN_MAX];   /* small memory */
    U8 *data;          /* malloc a large memory or point to msg */
    U32 data_len;      /* data lenght */
}upgrd_msg_t;

typedef struct _response
{
    U8        mac[16];
    U16       ret_code;
}response_t;

typedef enum _upgrd_status
{
    UPGRD_WAIT = 0x0,  /* �ȴ����� */
    UPGRD_READY,       /* ������������ȷ���ȴ������������� */
    UPGRD_DATA_CHECK,  /* �ѽ����������ݣ�����У������ */
    UPGRD_DATA_WRITE,  /* �ѽ����������ݣ�����д���ݵ�flash */
    UPGRD_DECOMPRESS,  /* �ѽ����������ݣ����ڽ�ѹ�ļ� */
    UPGRD_DONE,        /* ������� */
}upgrd_status_e;


typedef struct _upgrd_channel{
	int chid;
	char qname[32];
	char exchange[32];
	char rtkey[32];
}upgrd_channel_t;


S32 upgrd_get_host_macaddr(U8 *if_name, U8 *mac_addr);
S32 upgrd_get_host_mac_string(U8 *mac_str);
S32 upgrd_set_host_mac(U8 *mac_addr);
S32 upgrd_get_host_ipaddr(U8 *if_name);
S32 upgrd_get_server_ipaddr(U8 *ipaddr);
U32 upgrd_get_server_port();
U32 upgrd_get_host_by_name(U8 *cname);
S32 upgrd_server_link_isgood(U32 isSaveIp);
S32 send_msg_to_upgrade(U8 *msg, U32 len);
S32 recv_msg_timeout(U8 **msg, U32 timeout_ms);
S32 dnq_upgrd_init();
S32 dnq_upgrd_deinit();


#endif /* _DNQ_UPGRADE_H_ */

