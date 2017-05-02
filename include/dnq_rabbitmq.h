#ifndef _DNQ_RABBITMQ_H_
#define _DNQ_RABBITMQ_H_

#include "common.h"

#define SIZE      32

/*
* cjson convert to struct, authorization manage
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� ������Ȩ����
*
*/
typedef struct server_authorization
{
    char    type[SIZE];
    char    time[SIZE];
    char    authorization[SIZE];
}server_authorization_t;

/*
* cjson convert to struct, temperature policy
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� �·��¶Ȳ���
*
*/
typedef struct timesetting
{
    char starttime[SIZE];
    char endtime[SIZE];
    char degrees[SIZE];
}timesetting_t;

typedef struct room_temp_policy
{
    int            room_id;
    int            dpid;
    int            time_setting_cnt;
    timesetting_t  time_setting[5];
}room_temp_policy_t;

typedef struct server_temp_policy
{
    char      type[SIZE];
    char      time[SIZE];
    char      ctrl_id[SIZE];
    int       mode;
    int       rooms_cnt;
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
    int       room_id;  
    int       max;  
    int       min;  
}room_temp_limit_t;

typedef struct server_temp_limit
{
    char      type[SIZE];
    char      time[SIZE];
    char      ctrl_id[SIZE];
    int       mode;
    int       rooms_cnt;
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
    int       room_id;
    int       error;
}room_temp_error_t;

typedef struct server_temp_error
{
    char      type[SIZE];
    char      time[SIZE];
    char      ctrl_id[SIZE];
    int       mode;
    int       rooms_cnt;
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
    int       room_id;
    int       config_cnt;
    int       power[5];   /* need fixed?  */
    int       num[5];     /* need fixed?  */
}room_power_config_t;

typedef struct server_power_config
{
    char      type[SIZE];
    char      time[SIZE];
    char      ctrl_id[SIZE];
    int       mode;
    int       rooms_cnt;
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
    char      type[SIZE];
    char      time[SIZE];
    char      status[SIZE];
}server_response_t;


/*
* cjson convert to struct, temperature rectify 
*
* cjson���ݵ�C�ṹ�壬�ƶ�������� ���÷�������¶�
*
*/

typedef struct room_temp_correct
{
    int       room_id;
    int       correct;
}room_temp_correct_t;

typedef struct server_temp_correct
{
    char      type[SIZE];
    char      time[SIZE];
    char      ctrl_id[SIZE];
    int       mode;
    int       rooms_cnt;
    room_temp_correct_t  rooms[DNQ_ROOM_MAX];
}server_temp_correct_t;


/*
* controller to server 
* ������-->������ ������Ϣ
*/
typedef struct client_room
{
    int       room_id;
    int       value;
    int       loss;
}client_room_t;

typedef struct client_config_room
{
    int       room_id;
    int       degreePolicy;
    int       maxdegree;
    int       mindegree;
    int       error;
    int       corrent;
    int       power;
    
}client_room_config_t;

typedef struct client_status
{
    char      mac[SIZE];
    int       rooms_cnt;
    client_room_t  rooms[DNQ_ROOM_MAX];
}client_status_t;

typedef struct client_loss
{
    char      mac[SIZE];
    int       rooms_cnt;
    client_room_t  rooms[DNQ_ROOM_MAX];
}client_loss_t;

typedef struct client_response
{
    char      mac[SIZE];
    char      status[SIZE];
}client_response_t;

typedef struct client_config
{
    char      mac[SIZE];
    int       rooms_cnt;
    client_room_config_t  rooms[DNQ_ROOM_MAX];
}client_config_t;

typedef struct client_warn
{
    char      mac[SIZE];
    int       rooms_cnt;
    client_room_t  rooms[DNQ_ROOM_MAX];
}client_warn_t;

#endif /* _DNQ_RABBITMQ_H_ */

