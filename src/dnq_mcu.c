/* 
 * cpu and mcu communicate Program
 * 
 *  Copyright (c) 2017 yuchen
 *  Copyright 2017-2017 jiuzhoutech Inc.
 *  yuchen  <yuchen@jiuzhoutech.com, 382347665@qq.com>
 * 
 *  this is a communicate Program between cpu and mcu.
 *  mcu1:  heater control      protocol: RS232
 *  mcu2:  temperature sensor  protocol: RS485
 *
 * Note : 
 */


#include "dnq_common.h"
#include "ngx_palloc.h"
#include "dnq_mcu.h"


static char g_rs232_cmdbuf[][5] =
{
    //ctrl all rooms
    {
        0xFF,0xFE,0xFE,0xFF,  /* frame header, ֡ͷ */
        0xAA,                 /* Flag , Ӳ����־λ*/
        0x00,                 /* data lenght, ���ݳ��� */
        0xB0,                 /* mode, 0xB0: power mode, 0xB1: switch mode */
        0xFF,                 /* ctrl all rooms */
        /*
        * 24·���������
        * ��ѹģʽ: 0x01��ʾ100%���ʣ�0x02��ʾ75%����,0x03��ʾ�ر�
        * �̵���ģʽ: ����0x02��0x01��ʾ�򿪣�0x03��ʾ�ر�
        */
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

        0x00,0x00             /* CRC16 */
        0xFE,0xFF,0xFF,0xFE   /* frame footer */
    },

    //ctrl singal room
    {
        0xFF,0xFE,0xFE,0xFF,  /* frame header, ֡ͷ */
        0xAA,                 /* Flag , Ӳ����־λ*/
        0x00,                 /* data lenght, ���ݳ��� */
        0xB0,                 /* mode, 0xB0: power mode, 0xB1: switch mode */
        
        0xFF,                 /* ctrl single room, 0x00~0x18 */               
        0x00,                 /* power mode:  0x01:100%, 0x02:75%, 0x03:close */ 
                              /* switch mode: 0x01:open, 0x03:close */ 

        0x00,0x00             /* CRC16 */
        0xFE,0xFF,0xFF,0xFE   /* frame footer */
    },

    
}

static char g_rs485_cmdbuf[][5] =
{
    
}

S32 dnq_heater_ctrl(U32 id, U32 mode, U32 value)
{
    
}

S32 dnq_heater_ctrl(U32 id, U32 mode, U32 value)
{
    
}

S32 dnq_mcu_init()
{
    
    return 0;
}

S32 dnq_mcu_deinit()
{
    
    return 0;
}

