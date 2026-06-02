#include "Dri_USART.h"
#include <stdio.h>
#include "Delay.h"
#include "Driver_Led.h"
#include "Inf_W5500_Ping.h"
#include "Inf_HttpServer.h"
int main()
{
    Dri_USART_Init();
    Dri_Led_Init();
    Inf_W5500_Init();
    printf("NetWork http test .......\r\n");
    HttpServer_Init();
    HttpServer_Start();
    while (1)
    {
      
    }
}
