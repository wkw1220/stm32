#include "Inf_Client.h"
uint8_t rBuffClient[2048] = {0};
int8_t connectStatusClient;
uint16_t rDataLengthClient;
uint8_t clientIpClient[4];
uint16_t clientPortClient;
uint8_t fromAddr[4];
uint16_t fromPort;

/* 服务器IP */
uint8_t serverIp[4] = {192, 168, 1, 100};
/* 服务器端口 */
uint32_t serverPort = 8081;
void TCP_Client_Socket0() 
{
    /* 0. socket0中断使能 */
    // setSIMR(0);
    /* 1. 获取Socket0的状态 */
    printf("-----TCP Client Socket0 状态查询-----\n");
    uint8_t socketStatus = getSn_SR(0);
    printf("socketStatus=0x%X\n",socketStatus);
    switch (socketStatus) // 获取socket 0的状态
    {
    /* 2. 表示socket 0 已经打开，处于TCP工作模式，可以监听或连接 */
    case SOCK_INIT:
        /*  作为客户端主动去连接服务器 */
        connectStatusClient = connect(0,serverIp,serverPort);
        if (connectStatusClient == SOCK_OK)
        {
            printf("连接服务器成功....\r\n");
        }
        else
        {
            close(0);
            printf("连接服务器失败....\r\n");
        }
        break;
        /* 3. 表示连接已建立，可以接收数据或发送数据了 */
    case SOCK_ESTABLISHED:
        send(0,"Hello\r\n",7);
        while (1) // 循环读取数据
        {
            /* 等待接收数据。 Sn_IR(RECV) 中断无论何时，只要接收到了对方数据，此位置1*/
            while ((getSn_IR(0) & Sn_IR_RECV) == 0)
            {
                // 在等待连接期间，socket状态 != SOCK_ESTABLISHED，则表示socket发生了意外，就关闭socket 0,并退出函数
                if (getSn_SR(0) != SOCK_ESTABLISHED)
                {
                    printf("socket 0发生意外, 将重新打开socket 0 ...\r\n");
                    close(0);
                    return;
                }
            }
            /* 把Sn_IR_RECV 置1(注意是置1而不是置0)， 清除接收中断位，用于下次接收 */
            setSn_IR(0, getSn_IR(0) & Sn_IR_RECV);
            /* 获取接收到数据的长度 */
            rDataLengthClient = getSn_RX_RSR(0);
            if (rDataLengthClient > 0)
            {
                /* 接收数据 */
                recv(0, rBuffClient, rDataLengthClient);
                printf("接收到的数据长度是：%d,内容是：%s",rDataLengthClient,rBuffClient);
                /* 接收到数据返回给发送方 */
                send(0, rBuffClient, rDataLengthClient);
                memset(rBuffClient, 0, sizeof(rBuffClient));
            }
        }
        break;
     /* 4. socket处于关闭状态状态，资源已经被释放。在这里我们可以重新打开socket */
    case SOCK_CLOSED:
        /* 打开socket
        这个是打开客户端自己的socket0 端口号
         参数1 : socket编号,
         参数2 : 协议
         参数3 : 端口
         参数4 : sock的flag(无延时响应) *
         如果打开成功,则返回socket 编号
        */
       if (socket(0, Sn_MR_TCP, 8080, SF_TCP_NODELAY) == 0)
       {
          printf("socket 0 打开成功!... \r\n");
       }else{
          printf("socket 0 打开失败!... \r\n");
       }
       break;
    default:
      break;
    }
}

