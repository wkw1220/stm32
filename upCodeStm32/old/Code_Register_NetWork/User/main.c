#include "Driver_USART.h"
#include "Inf_W5500_Ping.h"
#include "stdio.h"
#include "Driver_Led.h"
#include "Inf_HttpServer.h"
uint8_t status[];;
int main()
{
    //while(1);
    Dri_USART1_Init();
    Inf_W5500_Init();
    Dri_Led_Init();
    WebServer_Init();
    WebServer_Start();
    while (1)
    {
         //TCP_Server_Socket0();
         //UDP_Socket0();
    }
}
