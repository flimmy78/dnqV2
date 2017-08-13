#ifndef _CONFIG_H_
#define _CONFIG_H_

/* �ڴ�صĴ�С */
#define DNQ_OS_MEM_POOL_SIZE       (1*1024*1024)
#define DNQ_APP_MEM_POOL_SIZE      (1*1024*1024)

/* ��ů����ǰ·������ů�����·�� */ 
#define DNQ_ROOM_CNT               16
#define DNQ_ROOM_MAX               16

/* LCD�趨�¶�ʱ�����ڵĲ�������λ��50��=0.5�� */
#define DNQ_TEMP_ADJUST_STEP       50
/* LCD�趨�¶�У׼ʱ�����ڵĲ��� ��λ��1��=1�� */
#define DNQ_CORRECT_ADJUST_STEP    1

/* ��ů�������ĸߵ��¼���ֵ��ʵ���¶�ֵҪ����100 */
#define DNQ_TEMP_MAX    3100
#define DNQ_TEMP_MIN    0

/* �Ʒ��������� */
#define DNQ_SERVER_URL   "iot.wiseheater.com"
#define DNQ_SERVER_PORT     5672

/* ɨ��ÿһ·�������ļ��ʱ�� */
#define DNQ_SENSOR_SCAN_INTERVAL  1

/* ʹ��485�Զ��շ�����ҪӲ��֧�֣�����֧���Զ��շ���ע�͵� */
#define DNQ_RS485_AUTO_TXRX_SUPPORT    1

/* ������Ϣ */
#define ETH_NAME  "eth0"

/* ����·�� */
#define ROOT_PATH           "/root/dnq"
#define MAIN_PROGRAM_NAME   "dnq_manage"
#define UPGRD_PROGRAM_NAME  "dnq_upgrade"

#define VERSION_FILE        "version.txt"

/*
*3 ��ȡ��������������ȫ������
*��һ��ʱ��Ҫ���ƶ�Ҫһ��ʱ�䣬����ͬ��
*�ڴ���ͷ�����
*
���Ӳ���汾��
mode=0 �������з��䣬��Ҫ�����£�lcd��Ҫ�������з���
����rootfs��������д����
*
*
*/

#endif /* _CONFIG_H_ */

