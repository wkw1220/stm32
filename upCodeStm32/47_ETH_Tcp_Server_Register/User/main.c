#include "Dri_USART.h"
#include <stdio.h>
#include "Delay.h"
#include "Inf_Server.h"
#include "Inf_W5500_Ping.h"
#include "Driver_Led.h"
int main()
{
    Dri_USART_Init();
    Dri_Led_Init();
    printf("NetWork Tcp Server test .......\r\n");
    Inf_W5500_Init();
    //TCP_Server_Socket00();
    //TCP_Server_Socket_More();
    while (1)
    {
      //TCP_Server_Socket00();
      TCP_Server_Socket_More();
      printf("-----\r\n");
    }
}
