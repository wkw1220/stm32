#include "Dri_USART.h"
#include <stdio.h>
#include "Delay.h"
#include "Driver_Lcd.h"
#include "Inf_Lcd.h"
int main()
{
     Dri_USART_Init();
     Inf_LCD_Init();

    uint32_t lcdId = Inf_LCD_ReadId();
    printf("lcdId = 0x%X\r\n", lcdId);
    Inf_LCD_ClearAll(WHITE);

    /* Inf_LCD_WriteAsciiChar(10, 10, 16, 'A', WHITE, RED);
    Inf_LCD_WriteAsciiChar(10, 30, 24, 'A', WHITE, RED);
    Inf_LCD_WriteAsciiChar(20, 60, 32, 'A', BLUE, WHITE);
    Inf_LCD_WriteAsciiChar(20, 100, 12, 'A', BLUE, WHITE);

    Inf_LCD_WriteAsciiString(200, 150, 24, "Hell\n\no!atguigu!Hello,Hello!at\nguigu!Hello", BLACK, WHITE);
    uint8_t buff[] = {'a', 'b', '\0'};
    Inf_LCD_WriteAsciiString(200, 300, 24, buff, BLACK, WHITE);

    Inf_LCD_WriteChineseChar(20, 330, 0, RED, BLUE);
    Inf_LCD_WriteChineseChar(20, 362, 1, BLUE, RED);
    Inf_LCD_WriteChineseChar(20, 394, 2, GRAY, RED); */

    //Inf_LCD_WriteAtguiguLogo(57, 100);
    Inf_LCD_WriteImage(0, 0);
    
    //Inf_LCD_DrawPoint(300,300, 5, RED);

    //Inf_LCD_DrawLine(10, 10, 10, 300, 5, RED);
    //Inf_LCD_DrawLine(10, 10, 300, 10, 5, RED);

    //Inf_LCD_DrawRectangle(20, 20, 300, 300, 5, RED);

    //Inf_LCD_DrawCircle_1(160, 240, 100, 5, BLUE);
    //Inf_LCD_DrawCircleFill_1(160, 240, 100, 2, RED, BLUE);
    while (1)
    {
      
    }
}
