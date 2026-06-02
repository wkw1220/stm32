#include "Dri_USART.h"
#include <stdio.h>
#include "Delay.h"

int main()
{
    Dri_USART_Init();
    printf("NetWork Ping test .......\r\n");
    Inf_W5500_Init();
    while (1)
    {
      
    }
}
