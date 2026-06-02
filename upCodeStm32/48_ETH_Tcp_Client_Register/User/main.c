#include "Dri_USART.h"
#include <stdio.h>
#include "Delay.h"
#include "Inf_Client.h"
#include "Inf_W5500_Ping.h"
#include "Driver_Led.h"
int main()
{
    Dri_USART_Init();
    Dri_Led_Init();
    printf("NetWork Tcp Client test .......\r\n");
    Inf_W5500_Init();
    while (1)
    {
      TCP_Client_Socket0();
    }
}
