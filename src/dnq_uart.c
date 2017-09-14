/* dnq uart Program
 * 
 *  Copyright (c) 2017 yuchen
 *  Copyright 2017-2017 jiuzhoutech Inc.
 *  yuchen  <yuchen@jiuzhoutech.com, 382347665@qq.com>
 * 
 *  this is a uart interface API, for app.
 * Note : 
 */
 
#include "dnq_uart.h"
#include "dnq_log.h"

#include <termios.h>
 
static S32 lcd_uart_fd = -1;
static S32 mcu_uart_fd = -1;
static S32 sensor_uart_fd = -1;

static U8 uart_name[4][8] = {"lcd", "mcu", "sensor", "unknown"};


struct termios stNew;
struct termios stOld;

#define BAUDRATE   B115200

static U8* dnq_get_uart_name_by_fd(S32 fd);

//Open Port & Set Port
S32 dnq_uart_open(U8 *dev)
{
    U32   fd;
    fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if(-1 == fd)
    {
        DNQ_ERROR(DNQ_MOD_UART, "Open Serial \"%s\" Port failed! errno:%s", \
        dev, strerror(errno));
        return -1;
    }
    
    if( (fcntl(fd, F_SETFL, 0)) < 0 )
    {
        close(fd);
        DNQ_ERROR(DNQ_MOD_UART, "Fcntl \"%s\" F_SETFL error! errno:%s", \
        dev, strerror(errno));
        return -1;
    }
    
    if(tcgetattr(fd, &stOld) != 0)
    {
        close(fd);
        DNQ_ERROR(DNQ_MOD_UART, "tcgetattr \"%s\" error! errno:%s", \
        dev, strerror(errno));
        return -1;
    }

    stNew = stOld;
    cfmakeraw(&stNew);//���ն�����Ϊԭʼģʽ����ģʽ�����е������������ֽ�Ϊ��λ������
 
    //set speed
    cfsetispeed(&stNew, BAUDRATE);//115200
    cfsetospeed(&stNew, BAUDRATE);
 
    //set databits
    stNew.c_cflag |= (CLOCAL|CREAD);
    stNew.c_cflag &= ~CSIZE;
    stNew.c_cflag |= CS8;
 
    //set parity
    stNew.c_cflag &= ~PARENB;
    stNew.c_iflag &= ~INPCK;
 
    //set stopbits
    stNew.c_cflag &= ~CSTOPB;
    stNew.c_cc[VTIME]=0;    //ָ����Ҫ��ȡ�ַ�����С����
    stNew.c_cc[VMIN]=1; //ָ����ȡ��һ���ַ��ĵȴ�ʱ�䣬ʱ��ĵ�λΪn*100ms
                //�������VTIME=0�������ַ�����ʱread�������������ڵ�����

    //stNew.c_cflag = 0x1cb2;
    //stNew.c_iflag = 0;
    tcflush(fd,TCIFLUSH);  //����ն�δ��ɵ�����/����������ݡ�
    if( tcsetattr(fd,TCSANOW,&stNew) != 0 )
    {
        close(fd);
        DNQ_ERROR(DNQ_MOD_UART, "tcsetattr \"%s\" error! errno:%s", \
        dev, strerror(errno));
        return -1;
    }
    dnq_uart_set_baudrate(fd, B9600);

    return fd;
}

S32 dnq_uart_set_baudrate(S32 fd, U32 baudrate)
{
    struct termios st;
    U32 set_rate = BAUDRATE;
    
    if(tcgetattr(fd, &st) != 0)
    {
        close(fd);
        DNQ_ERROR(DNQ_MOD_UART, "tcgetattr error! err:%s", strerror(errno));
        return -1;
    }
    if(baudrate == 9600)
        set_rate = B9600;
    else if(baudrate == 115200)
        set_rate = B115200;

    tcflush(fd,TCIFLUSH);
    
    cfsetispeed(&st, set_rate);
    cfsetospeed(&st, set_rate);

    if(tcsetattr(fd, TCSANOW, &st) != 0)
    {
        close(fd);
        DNQ_ERROR(DNQ_MOD_UART, "tcsetattr  error! err:%s", strerror(errno));
        return -1;
    }
    DNQ_INFO(DNQ_MOD_UART, "set new baudrate: %d", baudrate);

    return 0;
}

S32 dnq_uart_set_timeout(S32 fd, U32 timeout, U32 read_min)
{
	S32 ret = 0;
	struct  termios options;

	tcgetattr(fd, &options);
	
	tcflush(fd,TCIFLUSH); 
	options.c_cc[VTIME] = timeout;
	options.c_cc[VMIN] = read_min;

	ret = tcsetattr(fd,TCSANOW,&options);
	if(ret < 0) {
		close(fd);
        DNQ_ERROR(DNQ_MOD_UART, "tcsetattr error! errno:%s", strerror(errno));
        return -1;
	}
	
	tcflush(fd,TCIOFLUSH);

	return ret;
}


S32 dnq_uart_sync(U32 fd)
{
    return tcdrain(fd);
}

S32 dnq_uart_close(U32 fd)
{
    close(fd);
}

S32 dnq_uart_init()
{
    mcu_uart_fd = dnq_uart_open(DNQ_MCU_UART);
    if(mcu_uart_fd < 0)
    {
        DNQ_ERROR(DNQ_MOD_UART, "Mcu uart port1 init failed!");
        return -1;
    }
    
    lcd_uart_fd = dnq_uart_open(DNQ_LCD_UART);
    if(lcd_uart_fd < 0)
    {
        DNQ_ERROR(DNQ_MOD_UART, "Lcd uart port6 init failed!");
        return -1;
    }
    
    sensor_uart_fd = dnq_uart_open(DNQ_SENSOR_UART);
    if(sensor_uart_fd < 0)
    {
        DNQ_ERROR(DNQ_MOD_UART, "485 uart port8 init failed!");
        return -1;
    }
    dnq_uart_set_baudrate(sensor_uart_fd, 9600);
    dnq_uart_set_timeout(lcd_uart_fd, 2, 0);
    dnq_uart_set_timeout(mcu_uart_fd, 2, 0);
    dnq_uart_set_timeout(sensor_uart_fd, 4, 0);
    
    DNQ_INFO(DNQ_MOD_UART, "dnq_uart_init ok!");
    return 0;
}

S32 dnq_uart_deinit()
{
    if(lcd_uart_fd != -1)
    {
        close(lcd_uart_fd);
        lcd_uart_fd = -1;
    }
    if(mcu_uart_fd != -1)
    {
        close(mcu_uart_fd);
        mcu_uart_fd = -1;
    }
    if(sensor_uart_fd != -1)
    {
        close(sensor_uart_fd);
        sensor_uart_fd = -1;
    }
}

static S32 dnq_uart_read(U32 fd, U8 *buffer, U32 len)
{
    int i;
    int rlen;
    if(fd < 0)
    {
        DNQ_ERROR(DNQ_MOD_UART, "uart fd is not opened!");
        return -1;
    }
    
    rlen = read(fd, buffer, len);
    if(rlen < 0)
    {
        DNQ_ERROR(DNQ_MOD_UART, "read error! errno:%s", strerror(errno));
        return rlen;
    }
    DNQ_DEBUG(DNQ_MOD_UART, "<%s_mod> total=%d, read len=%d, data:", \
        dnq_get_uart_name_by_fd(fd), len, rlen);
    for(i=0; i<rlen; i++)
        DNQ_PRINT2(DNQ_MOD_UART, "%02x ", buffer[i]);
    DNQ_PRINT2(DNQ_MOD_UART, "\n");

    return rlen;
}

static S32 dnq_uart_write(U32 fd, U8 *buffer, U32 len)
{
    int i = 0;
    int wlen;
    
    if(fd < 0)
    {
        DNQ_ERROR(DNQ_MOD_UART, "uart fd is not opened!");
        return -1;
    }
        
    wlen = write(fd, buffer, len);
    if(wlen < 0)
    {
        DNQ_ERROR(DNQ_MOD_UART, "write error:%s", strerror(errno));
        return wlen;
    }
    
    DNQ_DEBUG(DNQ_MOD_UART, "<%s_mod> total=%d, send len=%d, data:", \
        dnq_get_uart_name_by_fd(fd), len, wlen);
    for(i=0; i<wlen; i++)
        DNQ_PRINT2(DNQ_MOD_UART, "%02x ", buffer[i]);
    DNQ_PRINT2(DNQ_MOD_UART, "\n");
    return wlen;
}

static U8* dnq_get_uart_name_by_fd(S32 fd)
{
    if(fd < 0)
    {
        DNQ_ERROR(DNQ_MOD_UART, "invalid fd=%d!", fd);
        return uart_name[3];
    }
    if(fd == lcd_uart_fd)
        return uart_name[0];
    if(fd == mcu_uart_fd)
        return uart_name[1];
    if(fd == sensor_uart_fd)
        return uart_name[2];
    
    return uart_name[3];
}

S32 dnq_lcd_uart_read(U8 *buffer, U32 len)
{
    S32 ret;
    ret = dnq_uart_read(lcd_uart_fd, buffer, len);
    return ret;
}

S32 dnq_lcd_uart_write(U8 *buffer, U32 len)
{
    S32 ret;
    ret = dnq_uart_write(lcd_uart_fd, buffer, len);
    return ret;
}

S32 dnq_mcu_uart_read(U8 *buffer, U32 len)
{
    S32 i = 0;
    S32 ret;
    ret = dnq_uart_read(mcu_uart_fd, buffer, len);
    if(ret > 0)
    {
        DNQ_DEBUG(DNQ_MOD_MCU, "total=%d, recv=%d, data:",len, ret);
        for(i=0; i<ret; i++)
            DNQ_PRINT2(DNQ_MOD_MCU, "%02x ", buffer[i]);
        DNQ_PRINT2(DNQ_MOD_MCU, "\n\n");
    }
    return ret;
}

S32 dnq_mcu_uart_write(U8 *buffer, U32 len)
{
    S32 ret;
    ret = dnq_uart_write(mcu_uart_fd, buffer, len);
    return ret;
}

S32 dnq_sensor_uart_read(U8 *buffer, U32 len)
{
    S32 ret;
    ret = dnq_uart_read(sensor_uart_fd, buffer, len);
    return ret;
}

S32 dnq_sensor_uart_write(U8 *buffer, U32 len)
{
    S32 ret;
    ret = dnq_uart_write(sensor_uart_fd, buffer, len);
    return ret;
}

S32 dnq_sensor_uart_sync()
{
    return dnq_uart_sync(sensor_uart_fd);
}

int uart_test(int argc, char **argv)
{
    int  nRet = 0;
    int  fd;
    char buf[1024];
    char buffer[64] = {0xA5, 0x5A, 0x03, 0x81, 0x00, 0x01};
 
    if( dnq_uart_open(DNQ_LCD_UART) == -1)
    {
        perror("SerialInit Error!\n");
        return -1;
    }
 
    memset(buf, 0, sizeof(buf));
    while(1)
    {
    	strcpy(buf, "hello !\n");
    	int i;
    	for(i=0; i<sizeof(buf); i++)
    		;//buf[i] = 0xFF;
    		
        nRet = write(fd, buffer, 6);
        if(-1 == nRet)
        {
            perror("send Data Error!\n");
            break;
        }
        if(0 < nRet)
        {
            buf[nRet] = 0;
            printf("send Data: %s, len=%d\n", buf, nRet);
        }
        dnq_msleep(100);
        nRet = read(fd, buf, sizeof(buf));
        if(-1 == nRet)
        {
            perror("read Data Error!\n");
            break;
        }
        if(0 < nRet)
        {
            buf[nRet] = 0;
            printf("read Data: %s, len=%d\n", buf, nRet);
        }
        for(i=0; i<nRet; i++)
            printf("%02x ", buf[i]);
        printf("\n");
    }
 
    dnq_uart_close(fd);
    return 0;
}
