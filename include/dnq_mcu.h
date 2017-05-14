#ifndef _DNQ_MCU_H_
#define _DNQ_MCU_H_


#include "common.h"

/* heater operate mode */
#define HEATER_MODE_POWER      0xB0
#define HEATER_MODE_SWITCH     0xB1

/* heater operate status value */
#define HEATER_OPEN    0
#define HEATER_CLOSE   1

#define HEATER_POWER_100   0
#define HEATER_POWER_75    1
#define HEATER_POWER_0     2   /* poweroff */

/* command define */
#define CMD_IDX_ALL_CTRL       0
#define CMD_IDX_SINGLE_CTRL    1

/* mcu response data lenght */
#define MCU_RESPONSE_LEN_REPLY    13
#define MCU_RESPONSE_LEN_GETTIME  19
#define MCU_RESPONSE_LEN_HEART    12

/* sensor response data lenght */
#define SENSOR_RESPONSE_LEN       13

/* error code */
#define ERR_HEADER     -1
#define ERR_FLAG       -2
#define ERR_CMD_FLAG   -3
#define ERR_VALUE      -4
#define ERR_TIME       -5
#define ERR_CRC        -6
#define ERR_FOOTER     -7
#define ERR_AGAIN      -8

typedef enum cmd_id
{
    CMD_ID_CTRL_ALL,
    CMD_ID_CTRL_SINGLE,
    CMD_ID_SET_TIME,
    CMD_ID_GET_TIME
}cmd_id_e;

typedef struct uart_data
{
    int  len;
    char data[64];
}uart_data_t;

S32 dnq_mcu_init();
S32 dnq_mcu_deinit();
S32 dnq_heater_ctrl_single(U32 id, U32 mode, U32 value);
S32 dnq_heater_ctrl_whole(U32 mode, U32 *value_array);
S32 dnq_rtc_set_time(U8 *datetime);
S32 dnq_rtc_get_time(U8 *datetime);
S32 dnq_room_get_temperature(U32 room_id);

#endif /* _DNQ_MCU_H_ */

