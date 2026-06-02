#include "Dri_USART.h"
#include <stdio.h>
#include "Delay.h"
#include "Inf_Udp.h"
#include "Driver_Led.h"
#include "Inf_W5500_Ping.h"
#include "Inf_Udp.h"
int main()
{
    Dri_USART_Init();
    Dri_Led_Init();
    Inf_W5500_Init();
    printf("NetWork Udp Client test .......\r\n");
    while (1)
    {
      UDP_Socket0();
    }
}
