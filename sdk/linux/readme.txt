
nuc972_config_dnqV2  Ϊ��Ŀ����ʹ�õ��ں�.config�ļ���

��¼һ��֮ǰkernel���ԵĹ��̣�

�������õ�linux��������ڵ�ů��Ӳ��ƽ̨�ϣ�kernel���������Ĺ��̡�
1����ʹ��nuc972_defconfigĬ�����á�
make nuc972_defconfig
2�����Ӳ������ں���������
make menuconfig
2.1 ����ں˵�.config support��proc/config.gz֧�֡�
ѡ��General setup  ---> 
    <*> Kernel .config support
    [*]   Enable access to .config through /proc/config.gz 

.config�ж�Ӧ�����������£�
CONFIG_IKCONFIG=y
CONFIG_IKCONFIG_PROC=y
����˵�������ɵ�kernel����procĿ¼�´��config.gz�����ǵ�ǰ�ں˵�.config�����ļ�����gzipѹ����ʽ��š�

2.2 ȡ���ں�initramfs/initrd support��ȡ����������
ѡ��General setup  ---> 
[ ] Initial RAM filesystem and RAM disk (initramfs/initrd) support
()    Initramfs source file(s) (NEW)
.config�ж�Ӧ�����������£�
CONFIG_BLK_DEV_INITRD=y
CONFIG_INITRAMFS_SOURCE="../rootfs"
CONFIG_INITRAMFS_ROOT_UID=0
CONFIG_INITRAMFS_ROOT_GID=0
CONFIG_RD_GZIP=y
CONFIG_RD_BZIP2=y
CONFIG_RD_LZMA=y
CONFIG_RD_XZ=y
CONFIG_RD_LZO=y
CONFIG_INITRAMFS_COMPRESSION_NONE=y
# CONFIG_INITRAMFS_COMPRESSION_GZIP is not set
# CONFIG_INITRAMFS_COMPRESSION_BZIP2 is not set
# CONFIG_INITRAMFS_COMPRESSION_LZMA is not set
# CONFIG_INITRAMFS_COMPRESSION_XZ is not set
# CONFIG_INITRAMFS_COMPRESSION_LZO is not set
# CONFIG_CC_OPTIMIZE_FOR_SIZE is not set
����˵�������ɵĶ�����kernel��kernel���Դ�rootfs����Ҫmount�ⲿ��rootfs�ļ�ϵͳ��

2.3 ȡ���ں����ߵ�֧�֣���Ϊ��Ŀû�����ߵ���ع��ܡ�ȡ��CONFIG_WIRELESSѡ�
ѡ��[*] Networking support  --->
        [ ]   Wireless  ---> 
   
�ں˵�.config���ص�����ѡ�
CONFIG_WIRELESS=y
# CONFIG_NVT_EXT_WIFI is not set
CONFIG_WEXT_CORE=y
CONFIG_WEXT_PROC=y
CONFIG_CFG80211=y
# CONFIG_NL80211_TESTMODE is not set
# CONFIG_CFG80211_DEVELOPER_WARNINGS is not set
# CONFIG_CFG80211_REG_DEBUG is not set
CONFIG_CFG80211_DEFAULT_PS=y
CONFIG_CFG80211_DEBUGFS=y
# CONFIG_CFG80211_INTERNAL_REGDB is not set
CONFIG_CFG80211_WEXT=y
# CONFIG_LIB80211 is not set
# CONFIG_MAC80211 is not set
����˵����ȥ���ں��е����߹��ܡ�

2.4 ���spi����֧�֣���Ϊ��Ŀʹ����spi nor flashӲ������Ҫ��spi���ѡ�
ѡ��Device Drivers  --->
    [*] SPI support  --->
    <*>   Nuvoton NUC970 Series SPI Port 0
        SPI0 pin selection by transfer mode (Quad mode)  --->
         ( ) Normal mode
         (X) Quad mode

�ں˵�.config����������ѡ�
#
# SPI Master Controller Drivers
#
# CONFIG_SPI_ALTERA is not set
CONFIG_SPI_BITBANG=y
# CONFIG_SPI_GPIO is not set
# CONFIG_SPI_OC_TINY is not set
# CONFIG_SPI_PXA2XX_PCI is not set
# CONFIG_SPI_SC18IS602 is not set
# CONFIG_SPI_XCOMM is not set
# CONFIG_SPI_XILINX is not set
CONFIG_SPI_NUC970_P0=y
# CONFIG_SPI_NUC970_P0_NORMAL is not set
CONFIG_SPI_NUC970_P0_QUAD=y
# CONFIG_SPI_NUC970_P0_SS1 is not set
# CONFIG_SPI_NUC970_P1 is not set
# CONFIG_SPI_DESIGNWARE is not set

#
# SPI Protocol Masters
#
# CONFIG_SPI_SPIDEV is not set
# CONFIG_SPI_TLE62X0 is not set
����˵�������spi����֧�֣�о��nuc972��2��spi������ʹ����port0������spi nor flash������Ҫ��֧�֡�
    
2.5 ���mtd�豸��֧�֣���Ϊ��Ŀʹ����spi nor flashӲ������Ҫͨ��mtd�����flash�Ķ�д���ʡ�
ѡ��Device Drivers  --->
    <*> Memory Technology Device (MTD) support  --->
         <*>   Command line partition table parsing
         -*-   Common interface to block layer for MTD 'translation layers'
         <*>   Caching block device access to MTD devices
            Self-contained MTD device drivers  --->
            <*> Support most SPI Flash chips (AT26DF, M25P, W25X, ...) 
            [*]   Use FAST_READ OPCode allowing SPI CLK >= 50MHz (NEW)

�ں˵�.config����������ѡ�
CONFIG_MTD=y
# CONFIG_MTD_TESTS is not set
# CONFIG_MTD_REDBOOT_PARTS is not set
CONFIG_MTD_CMDLINE_PARTS=y
# CONFIG_MTD_AFS_PARTS is not set
# CONFIG_MTD_AR7_PARTS is not set

#
# User Modules And Translation Layers
#
CONFIG_MTD_BLKDEVS=y
CONFIG_MTD_BLOCK=y
# CONFIG_FTL is not set
# CONFIG_NFTL is not set
# CONFIG_INFTL is not set
# CONFIG_RFD_FTL is not set
# CONFIG_SSFDC is not set
# CONFIG_SM_FTL is not set
# CONFIG_MTD_OOPS is not set
# CONFIG_MTD_SWAP is not set

#
# RAM/ROM/Flash chip drivers
#
# CONFIG_MTD_CFI is not set
# CONFIG_MTD_JEDECPROBE is not set
CONFIG_MTD_MAP_BANK_WIDTH_1=y
CONFIG_MTD_MAP_BANK_WIDTH_2=y
CONFIG_MTD_MAP_BANK_WIDTH_4=y
# CONFIG_MTD_MAP_BANK_WIDTH_8 is not set
# CONFIG_MTD_MAP_BANK_WIDTH_16 is not set
# CONFIG_MTD_MAP_BANK_WIDTH_32 is not set
CONFIG_MTD_CFI_I1=y
CONFIG_MTD_CFI_I2=y
# CONFIG_MTD_CFI_I4 is not set
# CONFIG_MTD_CFI_I8 is not set
# CONFIG_MTD_RAM is not set
# CONFIG_MTD_ROM is not set
# CONFIG_MTD_ABSENT is not set

#
# Mapping drivers for chip access
#
# CONFIG_MTD_COMPLEX_MAPPINGS is not set
# CONFIG_MTD_PLATRAM is not set

#
# Self-contained MTD device drivers
#
# CONFIG_MTD_DATAFLASH is not set
CONFIG_MTD_M25P80=y
CONFIG_M25PXX_USE_FAST_READ=y
# CONFIG_MTD_SST25L is not set
# CONFIG_MTD_SLRAM is not set
# CONFIG_MTD_PHRAM is not set
# CONFIG_MTD_MTDRAM is not set
# CONFIG_MTD_BLOCK2MTD is not set

#
# Disk-On-Chip Device Drivers
#
# CONFIG_MTD_DOCG3 is not set
# CONFIG_MTD_NAND is not set
# CONFIG_MTD_ONENAND is not set

#
# LPDDR flash memory drivers
#
# CONFIG_MTD_LPDDR is not set
# CONFIG_MTD_UBI is not set
����˵�������mtd��֧�֣���о����spi flash���������ڶ�дflash����ͨ��mtdblock���صȡ�

2.6 ���I2C֧�֣���������һ��rtcоƬ��ds1312sn����Ҫ��i2c���ܶ����д����rtc������
��ʵ�ʲ�û���õ�I2C�����Բ���I2C����Ϊ�����˵�Ƭ����rtcͨ�ţ�cpuֱ��ͨ����Ƭ������ȡʵʱʱ�����ݣ�
ѡ��Device Drivers  --->
    <*> I2C support  --->
        I2C Hardware Bus support  --->
        <*> NUC970 I2C Driver for Port 0
2.7 ���RTC֧�֣�������RTCоƬ������Ƭ�������ƣ�RTC��ѡ�����Ҳ�У����Ҫͨ��cpu�����п��ƣ���������֧�֣�
ѡ��Device Drivers  --->
    [*] Real Time Clock  --->
        <*>   Dallas/Maxim DS1305/DS1306

2.8 ���keyboard֧�֣�о������һ��keypadģ�飬֧���ⲿ4*2��4*4��4*8������̡���Ҫ������֧�֡�
ѡ��Device Drivers  --->
    Input device support  --->
        <*>   Event interface
        [*]   Keyboards  --->
            <*>   NUC970 Matrix Keypad support
            < >     NUC970 KEYPAD wake-up support (NEW)
            <*>     Pull-up NUC970 Matrix Keypad Pin (NEW)
                    NUC970 matrix keypad pin selection (Keypad pins are 4x8 matrix PH pin)  --->
                    ( ) Keypad pins are 4x2 matrix PA pin
                    ( ) Keypad pins are 4x4 matrix PA pin
                    ( ) Keypad pins are 4x8 matrix PA pin
                    ( ) Keypad pins are 4x2 matrix PH pin
                    ( ) Keypad pins are 4x4 matrix PH pin
                    (X) Keypad pins are 4x8 matrix PH pin

�򿪺��ں�.config���кܶ�KEYBOARDѡ�

2.9 ��Ӵ���֧�֣�о��NUC972֧��11·���ڣ����Ը���ʵ������������ü�ʹ�á�
    ��ů����Ŀʹ����4�����ڣ�
    uart1��consolse debug����
    uart2���װ嵥Ƭ��(MCU)ͨ�Ŵ���
    uart6��LCD ��Ļͨ�Ŵ���
    uart8��������̽ͷ���¶ȴ��������м�ʹ��RS232-->RS485ת��оƬ
ѡ��Device Drivers  --->
    Character devices  --->
        Serial drivers  --->
            [*] NUC970 serial support
            [*]   NUC970 UART1 support                                                                | |  
            [ ]     Enable UART1 CTS wake-up function (NEW)                                           | |  
                    NUC970 UART1 pin selection (Tx:PE2, Rx:PE3)  --->                                 | |  
            [*]   NUC970 UART2 support                                                                | |  
            [ ]     Enable UART2 CTS wake-up function (NEW)                                           | |  
                    NUC970 UART2 pin selection (Tx:PF11, Rx:PF12)  --->                               | |  
            [ ]   NUC970 UART3 support                                                                | |  
            [ ]   NUC970 UART4 support                                                                | |  
            [ ]   NUC970 UART5 support                                                                | |  
            [*]   NUC970 UART6 support                                                                | |  
            [ ]     Enable UART6 CTS wake-up function (NEW)                                           | |  
                    NUC970 UART6 pin selection (Tx:PB2, Rx:PB3)  --->                                 | |  
            [ ]   NUC970 UART7 support                                  
                  NUC970 UART7 support                                                                | |  
            [*]   NUC970 UART8 support                                                                | |  
            [ ]     Enable UART8 CTS wake-up function (NEW)                                           | |  
                    NUC970 UART8 pin selection (Tx:PH12, Rx:PH13)  --->                               | |  
            [ ]   NUC970 UART9 support                                                                | |  
            [ ]   NUC970 UART10 support                                                               | |  
            [*]   Console on NUC970 serial port   
2.10 ���GPIO�ܽſ��ƹ��ܣ�����ʹ����RS485���¶ȴ�����ͨ�ţ���Ҫ��485��ctrl_pin�����������Ͳ�����
ѡ��Device Drivers  --->
    -*- GPIO Support  --->
        [*]   /sys/class/gpio/... (sysfs interface)
����˵������֧�ֺ󣬿���ͨ����д/sys/class/gpio/�еĽڵ�����gpio�ܽŽ��п��ơ�

2.11 ���jffs2���ļ�ϵͳ֧��
ѡ��File systems  --->
    [*] Miscellaneous filesystems  --->
        <*>   Journalling Flash File System v2 (JFFS2) support
        (0)     JFFS2 debugging verbosity (0 = quiet, 2 = noisy) (NEW)
        [*]     JFFS2 write-buffering support (NEW)
        
        [ ]       Verify JFFS2 write-buffer reads (NEW)
        [ ]     JFFS2 summary support (NEW)
        [ ]     JFFS2 XATTR support (NEW)
        [ ]     Advanced compression options for JFFS2 (NEW)
        
        <*>   SquashFS 4.0 - Squashed file system support
        [ ]     Squashfs XATTR support (NEW)
        [*]     Include support for ZLIB compressed file systems (NEW)
        [ ]     Include support for LZO compressed file systems (NEW) 
        [ ]     Include support for XZ compressed file systems (NEW)
        [ ]     Use 4K device block size? (NEW)  
        [ ]     Additional option for memory-constrained systems (NEW) 
    
        ## �����ʹ�ã�����ȥ��
        < >   ROM file system support
        
        ##�������֧��
        [*] Network File Systems  --->

2.11 .configָ��cmdline������kernel������
CONFIG_CMDLINE="root=/dev/mtdblock2 rw rootfstype=jffs2 console=ttyS0,115200n8 rdinit=/sbin/init mem=64M"
