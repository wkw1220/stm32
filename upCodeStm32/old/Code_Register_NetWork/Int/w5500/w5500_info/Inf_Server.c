#include "Inf_Server.h"
uint8_t rBuff[2048] = {0};
int8_t listenStatus;
uint16_t rDataLength;
uint8_t clientIp[4];
uint16_t clientPort;
void TCP_Server_Socket0() //uint8_t* ledStatus
{
    /* 0. socket0中断使能 */
    // setSIMR(0);
    /* 1. 获取Socket0的状态 */
    uint8_t socketStatus = getSn_SR(0);
    switch (socketStatus) // 获取socket 0的状态
    {
    /* 2. 表示socket 0 已经打开，处于TCP工作模式，可以监听或连接 */
    case SOCK_INIT:
        /*  使用socket 0 监听来自客户端的请求 */
        listenStatus = listen(0);
        if (listenStatus == SOCK_OK)
        {
            printf("socket 0 监听成功....\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKINIT)
        {
            printf("socket 0 没有初始化....\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKCLOSED)
        {
            printf("socket 0 已经关闭....\r\n");
        }
        else
        {
            printf("socket 0 已经关闭....\r\n");
        }
        break;
    case SOCK_ESTABLISHED:
        /* 获取客户端ip和端口 */
        getSn_DIPR(0, clientIp);
        clientPort = getSn_DPORT(0);
        printf("客户端：%d.%d.%d.%d:%d,成功建立连接....\r\n",clientIp[0],clientIp[1],clientIp[2],clientIp[3],clientPort);
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
            rDataLength = getSn_RX_RSR(0);
            if (rDataLength > 0)
            {
                /* 接收数据 */
                recv(0, rBuff, rDataLength);
                printf("接收到的数据长度是：%d,内容是：%s",rDataLength,rBuff);
                /* 接收到数据返回给发送方 */
                send(0, rBuff, rDataLength);
                if(rBuff[0]=='1'){
                    Dri_Led_Off(LED2_BLUE);
                }else if(rBuff[0]=='0'){
                    Dri_Led_On(LED2_BLUE);
                }
            }
        }
     /* 4. socket处于关闭状态状态，资源已经被释放。  在这里我们可以重新打开socket */
    case SOCK_CLOSED:
        /* 并打开socket
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


