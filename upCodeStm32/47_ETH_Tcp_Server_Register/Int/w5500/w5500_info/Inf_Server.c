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
            printf("socket 0 listen success....\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKINIT)
        {
            printf("socket 0 no init....\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKCLOSED)
        {
            printf("socket 0 has closed....\r\n");
        }
        else
        {
            printf("socket 0 has closed....\r\n");
        }
        break;
      case SOCK_ESTABLISHED:
        /* 获取客户端ip和端口 */
        getSn_DIPR(0, clientIp);
        clientPort = getSn_DPORT(0);
        printf("Client:%d.%d.%d.%d:%d,has estabilished successful....\r\n",clientIp[0],clientIp[1],clientIp[2],clientIp[3],clientPort);
        while (1) // 循环读取数据
        {
            /* 等待接收数据。 Sn_IR(RECV) 中断无论何时，只要接收到了对方数据，此位置1*/
            while ((getSn_IR(0) & Sn_IR_RECV) == 0)
            {
                // 在等待连接期间，socket状态 != SOCK_ESTABLISHED，则表示socket发生了意外，就关闭socket 0,并退出函数
                if (getSn_SR(0) != SOCK_ESTABLISHED)
                {
                    printf("socket 0 has an accident occurred,will reopen socket 0 ...\r\n");
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
                printf("The length of received data is:%d,Content is:%s",rDataLength,rBuff);
                /* 接收到数据返回给发送方 */
                send(0, rBuff, rDataLength);
                if(rBuff[0]=='1'){
                    Dri_Led_Off(LED2_BLUE);
                }else if(rBuff[0]=='0'){
                    Dri_Led_On(LED2_BLUE);
                }
                memset(rBuff,0,2048);
            }
        }
        break;
       /* 4. socket处于关闭状态状态，资源已经被释放。  在这里我们可以重新打开socket */
      case SOCK_CLOSED:
        /* 并打开socket
            参数1 : socket编号,
            参数2 : 协议
            参数3 : 端口
            参数4 : sock的flag(无延时响应) 如果打开成功,则返回socket 编号
        */
       if (socket(0, Sn_MR_TCP, 8080, SF_TCP_NODELAY) == 0)
       {
          printf("socket 0 open success!... \r\n");
       }else{
          printf("socket 0 open failure!... \r\n");
       }
       break;
    default:
      break;
    }
}

uint8_t socketStatus;
void TCP_Server_Socket00() //uint8_t* ledStatus
{
    /* 0. socket0中断使能 */
    //setSIMR(0);
    /* 1. 获取Socket0的状态 */
    socketStatus= getSn_SR(0);
    //printf("socketStatus=0x%X\n",socketStatus);

    /* 2. 表示socket 0 已经打开，处于TCP工作模式，可以监听或连接 */
    if (socketStatus == SOCK_INIT)
    {
       /*  使用socket 0 监听来自客户端的请求 */
        listenStatus = listen(0);
        if (listenStatus == SOCK_OK)
        {
            printf("socket 0 listen success....\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKINIT)
        {
            printf("socket 0 no init....\r\n");
        }
        else if (listenStatus == SOCKERR_SOCKCLOSED)
        {
            printf("socket 0 has closed....\r\n");
        }
        else
        {
            printf("socket 0 has closed....\r\n");
        }
    }else if(socketStatus==SOCK_ESTABLISHED){
        /* 获取客户端ip和端口 */
        getSn_DIPR(0, clientIp);
        clientPort = getSn_DPORT(0);
        printf("Client:%d.%d.%d.%d:%d,has estabilished successful....\r\n",clientIp[0],clientIp[1],clientIp[2],clientIp[3],clientPort);
        while (1) // 循环读取数据
        {
            /* 等待接收数据。 Sn_IR(RECV) 中断无论何时，只要接收到了对方数据，此位置1*/
            while ((getSn_IR(0) & Sn_IR_RECV) == 0)
            {
                // 在等待连接期间，socket状态 != SOCK_ESTABLISHED，则表示socket发生了意外，就关闭socket 0,并退出函数
                if (getSn_SR(0) != SOCK_ESTABLISHED)
                {
                    printf("socket 0 has an accident occurred,will reopen socket 0 ...\r\n");
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
                printf("The length of received data is:%d,Content is:%s",rDataLength,rBuff);
                /* 接收到数据返回给发送方 */
                send(0, rBuff, rDataLength);
                if(rBuff[0]=='1'){
                    Dri_Led_Off(LED2_BLUE);
                }else if(rBuff[0]=='0'){
                    Dri_Led_On(LED2_BLUE);
                }
                memset(rBuff,0,2048);
            }
        }
    }else if(socketStatus==SOCK_CLOSED){
        /**  4. socket处于关闭状态状态，资源已经被释放。  在这里我们可以重新打开socket */
        /* 并打开socket
         *   参数1 : socket编号,
         *   参数2 : 协议
         *   参数3 : 端口
         *   参数4 : sock的flag(无延时响应) 如果打开成功,则返回socket 编号
        */
       if (socket(0, Sn_MR_TCP, 8080, SF_TCP_NODELAY) == 0)//重新打开socket 0
       {
          printf("socket 0 open success!... \r\n");
       }else{
          printf("socket 0 open failure!... \r\n");
       }
    }else if(socketStatus==SOCK_LISTEN){
        //printf("socket 0 is listening....\r\n");
        //Delay_s(1);
    }else if(socketStatus==SOCK_CLOSE_WAIT){
        //Close wait状态(半关闭状态),表示连接已经关闭，可以关闭socket
        //printf("socket 0 is listening....\r\n");
        //Delay_s(1);
    }

}

//////////////////////////////////////////////////////////////////////
uint8_t socketStatus0,socketStatus1;
int8_t listenStatus0,listenStatus1;
uint8_t clientIp0[4],clientIp1[4];
uint16_t clientPort0,clientPort1;
uint8_t getlistenStatus(uint8_t socketNum)
{
   uint8_t status=getSn_SR(socketNum);
   printf("socket%d:socketStatus=0x%X\n",socketNum,status);
   return status;
}

void listenInit(uint8_t socketStatus,uint8_t socketNum)
{
   
   if(socketStatus==SOCK_CLOSED){
        /**  4. socket处于关闭状态状态，资源已经被释放。  在这里我们可以重新打开socket */
        /* 并打开socket
         *   参数1 : socket编号,
         *   参数2 : 协议
         *   参数3 : 端口
         *   参数4 : sock的flag(无延时响应) 如果打开成功,则返回socket 编号
        */
       if (socket(socketNum, Sn_MR_TCP, 8080, SF_TCP_NODELAY) == socketNum)//重新打开socket 0
       {
          printf("socket %d open success!... \r\n",socketNum);
       }else{
          printf("socket %d open failure!... \r\n",socketNum);
       }
    }else if(socketStatus==SOCK_LISTEN){
        printf("socket %d is listening....\r\n",socketNum);
    }else if (socketStatus == SOCK_INIT){  /* 2. 表示socket 1 已经打开，处于TCP工作模式，可以监听或连接 */
       /*  使用socket 1 监听来自客户端的请求 */
        listenStatus = listen(socketNum);
        if (listenStatus == SOCK_OK)
        {
            printf("socket %d listen success....\r\n",socketNum);
        }
        else if (listenStatus == SOCKERR_SOCKINIT)
        {
            printf("socket %d no init....\r\n",socketNum);
        }
        else if (listenStatus == SOCKERR_SOCKCLOSED)
        {
            printf("socket %d has closed....\r\n",socketNum);
        }
        else
        {
            printf("socket %d has closed....\r\n",socketNum);
        }
    }
}

void isEstablished(uint8_t socketNum){
   /* 获取客户端ip和端口 */
   getSn_DIPR(socketNum, clientIp);
   clientPort = getSn_DPORT(socketNum);
   printf("Client---%d :%d.%d.%d.%d:%d,has estabilished successful....\r\n",socketNum,clientIp0[0],clientIp0[1],clientIp0[2],clientIp0[3],clientPort0);
   //Delay_s(1);
}

void receiveData(uint8_t socketNum){
             /* 等待接收数据。 Sn_IR(RECV) 中断无论何时，只要接收到了对方数据 此位生效*/
             if((getSn_IR(socketNum) & Sn_IR_RECV) ==0){
                // 在等待连接期间，socket状态 != SOCK_ESTABLISHED，则表示socket发生了意外，就关闭socket 0,并退出函数
                if (getSn_SR(socketNum) != SOCK_ESTABLISHED)
                {
                    printf("socket %d has an accident occurred,will reopen socket %d ...\r\n",socketNum,socketNum);
                    close(socketNum);
                    return;
                }
                //如果没有收到数据 就退出接收
                return;
            }
            /* 把Sn_IR_RECV 置1(注意是置1而不是置0)， 清除接收中断位，用于下次接收 */
            setSn_IR(socketNum, getSn_IR(socketNum) & Sn_IR_RECV);
            /* 获取接收到数据的长度 */
            rDataLength = getSn_RX_RSR(socketNum);
            if (rDataLength > 0)
            {
                /* 接收数据 */
                recv(socketNum, rBuff, rDataLength);
                printf("The length of received data is:%d,Content is:%s",rDataLength,rBuff);
                /* 接收到数据返回给发送方 */
                send(socketNum, rBuff, rDataLength);
                if(rBuff[0]=='1'){
                    Dri_Led_Off(LED2_BLUE);
                }else if(rBuff[0]=='0'){
                    Dri_Led_On(LED2_BLUE);
                }
                memset(rBuff,0,2048);
            }
}

void TCP_Server_Socket_More() //uint8_t* ledStatus
{
    /* 0. socket0中断使能 */
    //setSIMR(0);
    /* 1. 获取Socket0的状态 */
    socketStatus0= getlistenStatus(0);
    socketStatus1= getlistenStatus(1);
    listenInit(socketStatus0,0);
    listenInit(socketStatus1,1);
    if(socketStatus0==SOCK_ESTABLISHED){
        isEstablished(0);
        receiveData(0);
    }
    if(socketStatus1==SOCK_ESTABLISHED){
        isEstablished(1);
        receiveData(1);
    }
   
}


