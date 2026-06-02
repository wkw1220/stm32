#include "Inf_Udp.h"
uint8_t rBuffClient[2048] = {0};
int8_t connectStatusClient;
uint16_t rDataLengthClient;
uint8_t clientIpClient[4];
uint16_t clientPortClient;
uint8_t fromAddr[4];
uint16_t fromPort;

/* 端口 */
uint32_t localPort = 8888;
void UDP_Socket0()
{
    /* 0. socket0中断使能 */
    // setSIMR(0);
    /* 1. 获取Socket0的状态 */
    uint8_t socketStatus = getSn_SR(0);
    switch (socketStatus) // 获取socket 0的状态
    {
    case SOCK_UDP:
        while (1) // 循环读取数据
        {
            /* 等待接收数据。 Sn_IR(RECV) 中断无论何时，只要接收到了对方数据，此位置1*/
            while ((getSn_IR(0) & Sn_IR_RECV) == 0)
            {
                // 在等待连接期间，socket状态 != SOCK_UDP，则表示socket发生了意外，就关闭socket 0,并退出函数
                if (getSn_SR(0) != SOCK_UDP)
                {
                    printf("socket 0发生意外, 将重新打开socket 0 ...\r\n");
                    close(0);
                    return;
                }
            }
            /* 把Sn_IR_RECV 置1(注意是置1而不是置0)， 清除接收中断位，用于下次接收 */
            setSn_IR(0, getSn_IR(0) & Sn_IR_RECV);
            /* 获取接收到数据的长度 对于UDP来说，数据的长度比实际数据的长度多8个字节 */
            rDataLengthClient = getSn_RX_RSR(0);
            if (rDataLengthClient > 0)
            {
                /* 接收数据 */
                recvfrom(0, rBuffClient, rDataLengthClient,fromAddr,&fromPort);
                printf("接收到的数据长度是：%d,内容是：%s",rDataLengthClient-8,rBuffClient);
                /* 接收到数据返回给发送方 */
                sendto(0, rBuffClient, rDataLengthClient-8,fromAddr,fromPort);
                memset(rBuffClient, 0, sizeof(rBuffClient));
            }
        }
     /* 4. socket处于关闭状态状态，资源已经被释放.在这里我们可以重新打开socket */
    case SOCK_CLOSED:
        /* 并打开socket
         参数1 : socket编号,
         参数2 : 协议
         参数3 : 端口
         参数4 : sock的flag(无延时响应) *
         如果打开成功,则返回socket 编号
  */
       if (socket(0, Sn_MR_UDP, localPort, 0) == 0)
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
