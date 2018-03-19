/****************************************************************
���� �� �� �ơ�lcd12864.h
���� �� �� ����lcd12864 ͷ�ļ�
�� ��      �� ��shifu
*****************************************************************/

#include "includes.h"


//*****************************************************************
//�ܽŶ���

//#define GPIO_LCD    GPIOC
#define RCC_APB2Periph_GPIO_LCD    RCC_APB2Periph_GPIOD
#define LCD_DATA_IN()  {GPIOD->CRH&=0X00000000;GPIOD->CRH|=0X88888888;GPIOD->BSRR|=0xff00;}
//#define LCD_DATA_IN()  {GPIOD->CRH&=0X00000000;GPIOD->CRH|=0X44444444;}
#define LCD_DATA_OUT() {GPIOD->CRH&=0X00000000;GPIOD->CRH|=0X77777777;}//PD8-15 ������� 
//������***********************************************************
#define LCD_RS_1    PGout(6)=1
#define LCD_RS_0    PGout(6)=0
//��д
#define LCD_RW_1    PGout(4)=1
#define LCD_RW_0    PGout(4)=0
//ʹ��
#define LCD_EN_1    PGout(5)=1
#define LCD_EN_0    PGout(5)=0
#define  LCD_PSB_   PGout(2)
#define  LCD_RST   PGout(7)


/*****************************************************************
         Һ��ģ��ָ�����
*****************************************************************
      0x01      //����ʾָ��
      0x06      //��������ģʽ
      0x0c      //���ÿ��Կ���
      0x30      //�����趨(����ָ��)
      0x34      //�����趨(����ָ��)
      0x36      //�򿪻�ͼ(����ָ��)
*****************************************************************/

#define FIRST_ADDR 0       //�����ַ�/������ʾ��ʼλ��
u8 blank[72] = {0x72,0x04,0x52,0x04,0xDE,0x03,0x00,0x01,0x7C,0x01,0x55,0x01,0x56,0x01,0xFC,0x07,
0x56,0x01,0x7D,0x01,0x00,0x01,0x00,0x00,

0x00,0x00,0xDE,0x07,0x10,0x04,0x10,0x04,0x10,0x04,0xFF,0x07,0x10,0x04,0x10,0x04,
0x10,0x04,0xDE,0x07,0x00,0x00,0x00,0x00,

 0xC8,0x00,0x28,0x00,0xFF,0x0F,0x48,0x00,
      0x08,0x00,0xFE,0x0F,0x92,0x08,0x92,0x08,
      0x92,0x08,0x92,0x08,0xFB,0x0C,0x12,0x08};
unsigned char music_sign[3] = {0xe, 0x20, 0x00};
u8 blank2[] = {0x20, 0x20, 0x00};

/*
*	LCD��ʼ��
*
*/
void lcd12864_init(void)
{
    delay_ms(300);
    LCDIO_Init();
    lcdreset();                    //��ʼ��LCD��
    clrgdram();
	
//Disp_Img(music_sign);
	
	
}
void lcdreset(void)
{
    //delay_ms(100);
    write_com(0x30);

    delay_ms(1);             //ѡ�����ָ�
    write_com(0x30);       //ѡ��8bit������
    delay_ms(1);
    write_com(0x0c);       //����ʾ(���αꡢ������)
    delay_ms(1);
    write_com(0x01);       //�����ʾ�������趨��ַָ��Ϊ00H
    delay_ms(11);
    write_com(0x06);       //ָ�������ϵĶ�ȡ��д��ʱ���趨�α���ƶ�����ָ����ʾ����λ
    delay_ms(1);

}
void clrgdram(void)
{
    unsigned char x, y;

    for(y = 0; y < 64; y++)
        for(x = 0; x < 16; x++)
        {
            write_com(0x34);
            write_com(y + 0x80);      //�е�ַ
            write_com(x + 0x80);   //�е�ַ
            write_com(0x30);
            write_data(0x00);
            write_data(0x00);
        }
}

void LCDIO_Init(void)
{
    //Init_LCDIO();
    RCC->APB2ENR |= 1 << 8; //ʹ��PORTGʱ��

    GPIOG->CRL &= 0X000000FF;	//PG234567 ������� 	  RRD
    GPIOG->CRL |= 0X33333300;

    RCC->APB2ENR |= 1 << 5; //ʹ��PORTDʱ��
    GPIOD->CRH &= 0X00000000;	//PD8-15 ������� 	WR
    GPIOD->CRH |= 0X33333333;

    LCD_PSB_S;
    LCD_RST = 1;
}

#if 0
void chk_busy()
{
    RS = 0;
    RW = 1;
    E = 1;
    Lcd_Bus = 0xff;

    while((Lcd_Bus & 0x80) == 0x80);

    E = 0;
}
#endif





/*��ʼ�� port c Ϊ12864 ��Ҫ���������
* pc10:rs, pc11:rw, pc12:en, pc0~pc7:data
*/
void ck_busy1(void)
{
    u16 i;
    u8 j;
    j = 0x80;


    LCD_DATA_IN();
    //while(j == 1)
    {

        while(j == 0x80)
        {
            LCD_RS_0;
            LCD_RW_1;
            LCD_EN_1;

            i = GPIOD->IDR;
            j = i >> 8;
            j = j & 0x80;

        }

        LCD_EN_0;
    }
}
void ck_busy(void)
{
    u16 i;
    u8 j;
    j = 0x80;


    LCD_DATA_IN();
    //while(j == 1)
    {
        LCD_RS_0;
        LCD_RW_1;
        LCD_EN_1;

        while(j == 0x80)
        {

            LCD_RS_0;
            LCD_RW_1;
            LCD_EN_1;
						delay_us(1);
            i = GPIOD->IDR;
            j = i >> 8;
            j = j & 0x80;
					delay_us(1);
					//LCD_EN_0;
        }

        LCD_EN_0;
    }
}
//������(����)*******************************************************
void Lcd_Bus(unsigned char cmd)       //MCU ------> LCD
{
    LCD_DATA_OUT();
    DATAOUT(cmd);
}


void write_com(unsigned char cmdcode)
{
    	//ck_busy();
    LCD_RS_0;
    LCD_RW_0;
    delay_us(1);
    LCD_EN_1;
    delay_us(22);////////////////////������д���ʱ������ʵ�����ʱ
    Lcd_Bus(cmdcode);
    delay_us(10);
    LCD_EN_0;
    delay_us(1);

}

void write_data(unsigned char Dispdata)
{
    ck_busy();
    LCD_RS_1;
    LCD_RW_0;
    LCD_EN_1;
    // delay_us(8);///////////////////������д���ʱ������ʵ�����ʱ
    Lcd_Bus(Dispdata);
    delay_us(1);
    LCD_EN_0;
    delay_us(1);
}



unsigned char ST7920_ReadData(void)
{
    u16 uchRevData;
    u8 i;
    ck_busy();   //æµ���
    //����Ϊ����
    LCD_DATA_IN();
    LCD_RS_1;
    LCD_RW_1;
    LCD_EN_1;
    delay_us(1);
    uchRevData = GPIOD->IDR & 0xff00;
    //  LCD_EN_1;
    delay_us(1);
    LCD_EN_0;
    i = (uchRevData >> 8) & 0x00ff;
    return i;
}

#define BASIC_SET 0x00 //����ָ�����������ֲ������ֲᣬ��ͬ��??
#define EXTEND_SET 0x34 //��չָ�??
#define DRAW_ON 0x36 //��ͼ��ʾ��??
#define DRAW_OFF 0x30 //��ͼ��ʾ��?

void lcd_set_dot(unsigned char x, unsigned char y, unsigned char color)
{
    unsigned char x_byte, x_bit; //�ں��������һ���ֽڣ���һ��λ
    unsigned char y_byte, y_bit;

    unsigned char tmph, tmpl; //����������ʱ���������ڴ�Ŷ�����������

    x &= 0x7F;
    y &= 0x3F;

    x_byte = x / 16; //���������һ���ֽ�(��ַ)  //ע��һ����ַ��16λ��

    x_bit = x & 0x0F; //���������һ��λ

    y_byte = y / 32; //y��û���ĸ��ֽ����˵��  //����ֻ��ȷ�������ϰ��������°���  //0:�ϰ��� 1:�°���

    y_bit = y & 0x3F; //y_bitȷ�������ڵڼ���

    if(y_bit > 31)
    {
        y_bit = y_bit - 32;
    }

    write_com(0X34); //�رջ�ͼ��ʾ
    write_com(0X36);
    write_com(0x80 + y_bit); //��д��ֱ��ַ(���λ����Ϊ1)  //������������ֲ�
    write_com(0x80 + x_byte + 8 * y_byte);
//ˮƽ����
//�°�����ˮƽ������ʼ��ַΪ0x88
//(+8*y_byte)��������ȷ��
//���ϰ��������°���

    ST7920_ReadData(); //�ȿն�һ��
    tmph = ST7920_ReadData(); //����λ
    tmpl = ST7920_ReadData();
    write_com(0x80 + y_bit); //��������ı�AC��������������һ��
    write_com(0x80 + x_byte + 8 * y_byte);

    if(x_bit < 8) //���x_bitλ��С��8
    {
        if(color)
        {
            write_data(tmph | (0x01 << (7 - x_bit)));  //д���ֽڡ���Ϊ�����Ǵ������ҵ�//��GDRAM��λ���󣬵�λ����
            write_data(tmpl); //ԭ�����ͻ�
        }
        else
        {
            write_data(tmph & (~(0x01 << (7 - x_bit))));  //д���ֽڡ���Ϊ�����Ǵ������ҵ�//��GDRAM��λ���󣬵�λ����
            write_data(tmpl); //ԭ�����ͻ�
        }
    }
    else
    {
        if(color)
        {
            write_data(tmph); //ԭ�����ͻ�
            write_data(tmpl | (0x01 << (15 - x_bit)));
        }
        else
        {
            write_data(tmph); //ԭ�����ͻ�
            write_data(tmpl & (~(0x01 << (15 - x_bit))));
        }
    }

    ck_busy();

    write_com(0X30);


}

u16  lcd_get_dot(unsigned char x, unsigned char y)
{
    unsigned char x_byte, x_bit; //�ں��������һ���ֽڣ���һ��λ
    unsigned char y_byte, y_bit;

    unsigned char tmph, tmpl; //����������ʱ���������ڴ�Ŷ�����������

    u8 data;
    x &= 0x7F;
    y &= 0x3F;

    x_byte = x / 16; //���������һ���ֽ�(��ַ)  //ע��һ����ַ��16λ��

    x_bit = x & 0x0F; //���������һ��λ

    y_byte = y / 32; //y��û���ĸ��ֽ����˵��  //����ֻ��ȷ�������ϰ��������°���  //0:�ϰ��� 1:�°���

    y_bit = y & 0x3F; //y_bitȷ�������ڵڼ���

    if(y_bit > 31)
    {
        y_bit = y_bit - 32;
    }

    write_com(0X34); //�رջ�ͼ��ʾ
    write_com(0X36);
    write_com(0x80 + y_bit); //��д��ֱ��ַ(���λ����Ϊ1)  //������������ֲ�
    write_com(0x80 + x_byte + 8 * y_byte);
//ˮƽ����
//�°�����ˮƽ������ʼ��ַΪ0x88
//(+8*y_byte)��������ȷ��
//���ϰ��������°���

    ST7920_ReadData(); //�ȿն�һ��
    tmph = ST7920_ReadData(); //����λ
    tmpl = ST7920_ReadData();

//write_com(0x80 + y_bit); //��������ı�AC��������������һ��
//write_com(0x80 + x_byte + 8 * y_byte);
    if(x_bit < 8) //���x_bitλ��С��8
    {
        data = tmph & (0x01 << (7 - x_bit));  //д���ֽڡ���Ϊ�����Ǵ������ҵ�//��GDRAM��λ���󣬵�λ����

    }
    else
    {

        data = tmpl & (0x01 << (15 - x_bit));
    }

    ck_busy();
//write_com(0X36);
//write_com(0X30);

    return data;

}

void CGRAM()
{
    int i;
    write_com(0x30);
    write_com(0x40);

    for(i = 0; i < 16; i++)
    {
//        write_data(zk[i * 2]);
//        write_data(zk[i * 2 + 1]);
    }
}

//��ʾһ�麺��
void xy_hzkdis(char x, char y, unsigned char *s)
{
    char y0 = 0;

    if(y == 0)
        y0 = 0;
    else if(y == 1)
        y0 = 0x10;
    else if(y == 2)
        y0 = 0x08;
    else if(y == 3)
        y0 = 0x18;

    write_com(0x80 + y0 + x);

    while(*s > 0)
    {
        write_data(*s);
        s++;
        delay_us(1);
    }
}
void hzkdis(unsigned char *s)
{

    while(*s > 0)
    {
        write_data(*s);
        s++;
        delay_us(1);
    }
}




void Disp_Img(unsigned char *img)
{

    unsigned int j = 0;
    unsigned char x, y, i;

    for(i = 0; i < 9; i += 8)
        for(y = 0; y < 32; y++)
            for(x = 0; x < 8; x++)
            {
                write_com(0x36);//��������---8BIT���ƽ��棬����ָ�
                write_com(y + 0x80);      //�е�ַ
                write_com(x + 0x80 + i); //�е�ַ
                write_com(0x30);
                write_data(music_sign[j++]);
                write_data(music_sign[j++]);
            }
}


//˳������ʽ
void Disp_hz16x16(const unsigned char *img, char y, char x)
{
    unsigned char i;
    for(i = 0; i < 12; i++)
    {
        write_com(0x36);
        write_com(x + 0x80 + i);
        write_com(y * 8 + 0x80);
        write_com(0x30);
        write_data(blank[2 * i]);
        write_data(blank[2 * i + 1]);
    }
}

void Disp_hz8x16(const unsigned char *img, char x, char y)
{

//	 unsigned int j=0;
    unsigned char i;

    if(y < 32)
    {
        for(i = 0; i < 16; i++)
        {
            write_com(0x36);
            write_com(0x80 + y + i);
            write_com(0x80 + x);
            write_com(0x30);
            write_data(img[i]);
            //  write_data(img[2*i+1]);
        }
    }
    else
    {
        y = y - 32;
        x = x + 8;

        for(i = 0; i < 16; i++)
        {
            write_com(0x36);
            write_com(0x80 + y + i);
            write_com(0x80 + x);
            write_com(0x30);
            write_data(img[i]);
            //  write_data(img[2*i+1]);
        }
    }



}


void lcdfill(unsigned char disdata)
{
    unsigned char x, y;

    for(y = 0; y < 32; y++)
        for(x = 0; x < 16; x++)
        {
            write_com(0x36);
            write_com(y + 0x80);      //�е�ַ
            write_com(x + 0x80);   //�е�ַ
            write_com(0x30);
            write_data(disdata);
            write_data(disdata);
        }

    write_com(0x34);
    write_com(0x36);
}

void Disp_H_Line()
{
    unsigned char x, y;
    unsigned char k = 0x00;

    for(y = 0; y < 32; y++)
    {
        k = ~k;

        for(x = 0; x < 16; x++)
        {
            write_com(0x36);
            write_com(y + 0x80);    //�е�ַ
            write_com(x + 0x80);   //�е�ַ
            write_com(0x30);
            write_data(k);
            write_data(k);
        };
    }

    write_com(0x34);
    write_com(0x36);
}


void CLR_screen(void)
{
//    unsigned int j = 0;
    unsigned char x, y, i;

    for(i = 0; i < 9; i += 8)
        for(y = 0; y < 32; y++)
            for(x = 0; x < 8; x++)
            {
                write_com(0x36);//��������---8BIT���ƽ��棬����ָ�
                write_com(y + 0x80);      //�е�ַ
                write_com(x + 0x80 + i); //�е�ַ
                write_com(0x30);
                write_data(0);
                write_data(0);
            }

    write_com(0x34);
    write_com(0x36);

}
void clrscreen(void)
{
    write_com(0x01);
    delay_ms(2);
}



void dis_playnum(int num, u8 hang, u8 lie)
{




}


/*
*		��һ�����ߣ�
*				X0:������
*				X1������
*				Y :������
*		 color��1��ʾ��0����ʾ
*/
void v_Lcd12864DrawLineX_f(unsigned char x0, unsigned char y, unsigned char x1, unsigned char Color)
{
    unsigned char Temp ;

    if(x0 > x1)
    {
        Temp = x1 ;
        x1   = x0 ;
        x0   = Temp ;
    }

    for(; x0 <= x1 ; x0++)
        lcd_set_dot(x0, y, Color) ;
}
//��һ������
void v_Lcd12864DrawLineY_f(unsigned char x, unsigned char y0, unsigned char y1, unsigned char Color)
{
    unsigned char Temp ;

    if(y0 > y1)
    {
        Temp = y1 ;
        y1      = y0 ;
        y0      = Temp ;
    }

    for(; y0 <= y1 ; y0++)
        lcd_set_dot(x, y0, Color)  ;
}

void oled_updata(unsigned char x, unsigned char y)
{
	int y0,temp=0x01,color,x0,lengh=0,yy,shu;
			for(yy=0;yy<=2;yy++)

	{
			for(x0=x;x0<=(x+127);x0++)

		{
	for(y0=y;y0<=(y+7);y0++)
	{
		color = oled_disp_buf[shu][lengh]&temp ;
		lcd_set_dot(x0, y0, color);
		temp*=2;
	}
	lengh++;
	temp=0x01;
	}
		lengh=0;
	y+=8;
	shu++;
}	y=0;
}




void put_out(unsigned char x, unsigned char y, unsigned char *str)
{
	int y0,temp=0x01,color,x0,lengh=0,yy;
			for(yy=0;yy<=4;yy++)

	{
			for(x0=x;x0<=(x+127);x0++)

		{
	for(y0=y;y0<=(y+7);y0++)
	{
		color = str[lengh]&temp ;
		lcd_set_dot(x0, y0, color);
		temp*=2;
	}
	lengh+=2;
	temp=0x01;
	}
		lengh=1;
	y+=8;

}	y=0;
}
/*
*		��һ�����ο�
*/
//void input_rect(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
//{
//	v_Lcd12864DrawLineX_f(x0,y0,x1,1);		//��OLED��ʾ���ϻ���һ����/���ĺ���
//	v_Lcd12864DrawLineX_f(x0,y1,x1,1);		//��OLED��ʾ���ϻ���һ����/���ĺ���
//	v_Lcd12864DrawLineY_f(x0,y0,y1,1);		//����ʾ������ʾһ����/��������
//	v_Lcd12864DrawLineY_f(x1,y0,y1,1);		//����ʾ������ʾһ����/��������
//}

int ABS(int num)
{

    if(num < 0)
        return 0 - num;
    else
        return num;
}








