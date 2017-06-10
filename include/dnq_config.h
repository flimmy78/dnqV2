#ifndef _DNQ_CONFIG_H_
#define _DNQ_CONFIG_H_

#define DNQ_OS_MEM_POOL_SIZE       (1*1024*1024)
#define DNQ_APP_MEM_POOL_SIZE      (1*1024*1024)

#define DNQ_ROOM_CNT               16
#define DNQ_ROOM_MAX               16

#define DNQ_TEMP_ADJUST_STEP       50
#define DNQ_CORRECT_ADJUST_STEP    1

#define DNQ_SERVER_URL   "iot.wiseheater.com"
#define DNQ_SERVER_PORT     5672


#define ETH_NAME  "eth0"

#define ROOT_PATH           "/root/dnq"
#define MAIN_PROGRAM_NAME   "dnq_manage"
#define UPGRD_PROGRAM_NAME  "dnq_upgrade"

/*
*1 ��ʼ��ȡip����ʼ��ȡmac
*2 ��̬ɨ��SN������
*3 ��ȡ��������������ȫ������
*4 rabbitmq��Ϣ���ء��������ݴ���״̬
*5 485��������
*6 RTCʱ���������

*��һ��ʱ��Ҫ���ƶ�Ҫһ��ʱ�䣬����ͬ��
*SN״̬ȫ��Ĭ��Ϊ ����
*
*
*/

#endif /* _DNQ_CONFIG_H_ */

