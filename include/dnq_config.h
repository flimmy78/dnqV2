
#ifndef _DNQ_CONFIG_H_
#define _DNQ_CONFIG_H_

#include "common.h"


#define DNQ_CONFIG_PATH           "/root/dnq/configs"
#define DNQ_DATA_FILE             "/root/dnq/dnq.dat"
#define DNQ_CONFIG_FILE           "/root/dnq/dnq.conf"
/*
* json config define
* json�����ļ� �ļ����ƶ���
*/
#define JSON_FILE_AUTHORRIZATION  "authorization.json"
#define JSON_FILE_POLICY          "policy.json"
#define JSON_FILE_LIMIT           "limit.json"
#define JSON_FILE_ERROR           "error.json"
#define JSON_FILE_POWER           "power.json"
#define JSON_FILE_RESPONSE        "response.json"
#define JSON_FILE_CORRECT         "correct.json"
#define JSON_FILE_INIT            "init.json"
#define DEGREES_NULL    0xFF

/*
* cjson convert to struct, authorization manage
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� ������Ȩ����
*
*/
typedef struct server_authorization
{
    U8      type[SIZE_32];
    U8      time[SIZE_32];
    U8      authorization[SIZE_32];
}server_authorization_t;

/*
* cjson convert to struct, temperature policy
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� �·��¶Ȳ���
*
*/
typedef struct timesetting
{
    U32  start;  /* starttime second */
    U32  end;    /* endtime second */
    U16  degrees;
    U8   starttime[SIZE_16];
    U8   endtime[SIZE_16];
}timesetting_t;

typedef struct room_temp_policy
{
    U16            room_id;
    U16            dpid;
    U16            time_setting_cnt;
    timesetting_t  time_setting[4];
}room_temp_policy_t;

typedef struct server_temp_policy
{
    U8        type[SIZE_32];
    U8        time[SIZE_32];
    U8        ctrl_id[SIZE_32];
    U16       mode;
    U16       rooms_cnt;
    room_temp_policy_t    rooms[DNQ_ROOM_MAX];
}server_temp_policy_t;

/*
* cjson convert to struct, temperature limit
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� ���͸ߵ�������
*
*/
typedef struct room_temp_limit
{
    U16       room_id;  
    U16       min;  
    U16       max;  
    
}room_temp_limit_t;

typedef struct server_temp_limit
{
    U8        type[SIZE_32];
    U8        time[SIZE_32];
    U8        ctrl_id[SIZE_32];
    U16       mode;
    U16       rooms_cnt;
    room_temp_limit_t      rooms[DNQ_ROOM_MAX];
}server_temp_limit_t;

/*
* cjson convert to struct, temperature error
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� �����¶Ȼز�
*
*/

typedef struct room_temp_error
{
    U16       room_id;
    U16       error;
}room_temp_error_t;

typedef struct server_temp_error
{
    U8        type[SIZE_32];
    U8        time[SIZE_32];
    U8        ctrl_id[SIZE_32];
    U16       mode;
    U16       rooms_cnt;
    room_temp_error_t      rooms[DNQ_ROOM_MAX];
}server_temp_error_t;

/*
* cjson convert to struct, power config
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� ���͹�������
*
*/

typedef struct room_power_config
{
    U16       room_id;
    U16       config_cnt;
    U16       power[6];   /* need fixed?  */
    U16       num[6];     /* need fixed?  */
}room_power_config_t;

typedef struct server_power_config
{
    U8        type[SIZE_32];
    U8        time[SIZE_32];
    U8        ctrl_id[SIZE_32];
    U16       mode;
    U16       rooms_cnt;
    room_power_config_t      rooms[DNQ_ROOM_MAX];
}server_power_config_t;

/*
* cjson convert to struct, a response from server to controller
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� ����Ӧ����Ϣ
*
*/
typedef struct server_response
{
    U8        type[SIZE_32];
    U8        time[SIZE_32];
    U8        status[SIZE_32];
}server_response_t;


/*
* cjson convert to struct, temperature rectify 
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� ���÷�������¶�
*
*/

typedef struct room_temp_correct
{
    U16       room_id;
    S16       correct;
}room_temp_correct_t;

typedef struct server_temp_correct
{
    U8        type[SIZE_32];
    U8        time[SIZE_32];
    U8        ctrl_id[SIZE_32];
    U16       mode;
    U16       rooms_cnt;
    room_temp_correct_t  rooms[DNQ_ROOM_MAX];
}server_temp_correct_t;

/*
* cjson convert to struct, initialize device install info 
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� ��ʼ��������Ϣ���豸��װ��Ϣ��
*
*/
typedef struct partition_info
{
    U8        no[SIZE_32];
    U16       id;
    U8        memo[SIZE_32];
    U8        name[SIZE_32];
    U16       isDelete;
    
}partition_info_t;

typedef struct room_info
{
    U16       error;
    U16       max;
    U16       min;
    S16       correct;
    U16       room_id;
    U8        room_name[SIZE_16];
    U16       room_order;
    U16       room_floor;
    U8        position[SIZE_16];
}room_info_t;

typedef struct server_init_info
{
    U8        type[SIZE_32];
    U8        time[SIZE_32];
    partition_info_t partition;
    U32       project_id;
    U32       building_id;
    U32       equipment_id;
    U8        project_name[SIZE_32];
    U8        building_name[SIZE_32];
    U8        equipment_mac[SIZE_32];
    U16       rooms_cnt;
    room_info_t    rooms[DNQ_ROOM_MAX];
    U32       inited;
    
}server_init_info_t;

/*
* cjson convert to struct, All config info 
*
* �������õĽṹ�壬����ƶ�����������͵���������
*
*/
typedef struct _dnq_config
{
    U32  inited;
    server_authorization_t authorization;
    server_temp_policy_t   temp_policy;
    server_temp_limit_t    temp_limit;
    server_temp_error_t    temp_error;
    server_power_config_t  power_config;
    server_response_t      response;
    server_temp_correct_t  temp_correct;
    server_init_info_t     init;
    U32                    reserved;
}dnq_config_t;


/*
* controller to server 
* ������-->������ ������Ϣ
*/
typedef struct client_room
{
    U16       room_id;
    U16       degree;
    U16       loss;
}client_room_t;

typedef struct client_config_room
{
    U16       room_id;
    U16       degreePolicy;
    U16       maxdegree;
    U16       mindegree;
    U16       error;
    S16       correct;
    U32       power;
    
}client_room_config_t;

typedef struct client_status
{
    U8        mac[SIZE_32];
    U32       rooms_cnt;
    client_room_t  rooms[DNQ_ROOM_MAX];
}client_status_t;

typedef struct client_loss
{
    U8        mac[SIZE_32];
    U32       rooms_cnt;
    client_room_t  rooms[DNQ_ROOM_MAX];
}client_loss_t;

typedef struct client_response
{
    U8        type[SIZE_32];
    U8        mac[SIZE_32];
    U8        status[SIZE_32];
}client_response_t;

typedef struct client_config
{
    U8        mac[SIZE_32];
    U32       rooms_cnt;
    client_room_config_t  rooms[DNQ_ROOM_MAX];
}client_config_t;

typedef struct client_warn
{
    U8        mac[SIZE_32];
    U32       rooms_cnt;
    client_room_t  rooms[DNQ_ROOM_MAX];
}client_warn_t;


/*
* message type define
* ��Ϣ�����ֶ�  ö��
*/

typedef enum json_type
{
    JSON_TYPE_AUTHORRIZATION,
    JSON_TYPE_TEMP_POLICY,
    JSON_TYPE_TEMP_LIMIT,
    JSON_TYPE_TEMP_ERROR,
    JSON_TYPE_POWER_CONFIG,
    JSON_TYPE_RESPONSE,
    JSON_TYPE_CORRECT,
    JSON_TYPE_INIT
}json_type_e;

S32 dnq_config_init();
S32 dnq_config_deinit();
void dnq_config_print();
S32 dnq_config_load();
S32 dnq_data_file_save();
S32 dnq_config_check_and_sync(json_type_e json_type, U8 *json_data, U32 len, void *cjson_struct);
S32 dnq_config_sync_to_lcd(json_type_e json_type, void *cjson_struct);

extern dnq_config_t g_dnq_config;

#define init_info_is_ok()  (g_dnq_config.init.inited)

server_authorization_t*
    dnq_get_authorization_config(server_authorization_t *config);
server_temp_policy_t*
    dnq_get_temp_policy_config(server_temp_policy_t *config);
server_temp_limit_t*
    dnq_get_temp_limit_config(server_temp_limit_t *config);
server_temp_error_t*
    dnq_get_temp_error_config(server_temp_error_t *config);
server_power_config_t*
    dnq_get_power_config_config(server_power_config_t *config);
server_response_t*
    dnq_get_response_config(server_response_t *config);
server_temp_correct_t*
    dnq_get_temp_correct_config(server_temp_correct_t *config);
server_init_info_t*
    dnq_get_init_config(server_init_info_t *config);

#endif /* _DNQ_CONFIG_H_ */

