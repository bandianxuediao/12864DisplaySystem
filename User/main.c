#include "includes.h"
#include "display.h"


extern u8 blank[24];

int main(void)
{
	    unsigned char x, y;
int i=0;
    delay_init();	    	 //延时函数初始化
    NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		lcd12864_init();
//		input_rect(11, 11,33, 33);
		ShowMainMenu();
	while(1)
    {
        delay_ms(20);
    }
}



