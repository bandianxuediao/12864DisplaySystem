#include "includes.h"
#include "display.h"


extern u8 blank[24];

int main(void)
{
	    unsigned char x, y;
int i=0;
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		lcd12864_init();
//		input_rect(11, 11,33, 33);
		ShowMainMenu();
	while(1)
    {
        delay_ms(20);
    }
}



