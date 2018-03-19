/****************************************************************
【文 件 名 称】lcd12864.h
【功 能 描 述】lcd12864 头文件
【 作      者 】shifu
*****************************************************************/

#include "includes.h"


//*****************************************************************
//管脚定义

//#define GPIO_LCD    GPIOC
#define RCC_APB2Periph_GPIO_LCD    RCC_APB2Periph_GPIOD
#define LCD_DATA_IN()  {GPIOD->CRH&=0X00000000;GPIOD->CRH|=0X88888888;GPIOD->BSRR|=0xff00;}
//#define LCD_DATA_IN()  {GPIOD->CRH&=0X00000000;GPIOD->CRH|=0X44444444;}
#define LCD_DATA_OUT() {GPIOD->CRH&=0X00000000;GPIOD->CRH|=0X77777777;}//PD8-15 推挽输出 
//控制线***********************************************************
#define LCD_RS_1    PGout(6)=1
#define LCD_RS_0    PGout(6)=0
//读写
#define LCD_RW_1    PGout(4)=1
#define LCD_RW_0    PGout(4)=0
//使能
#define LCD_EN_1    PGout(5)=1
#define LCD_EN_0    PGout(5)=0
#define  LCD_PSB_   PGout(2)
#define  LCD_RST   PGout(7)


/*****************************************************************
         液晶模块指令集定义
*****************************************************************
      0x01      //清显示指令
      0x06      //设置输入模式
      0x0c      //设置开显控制
      0x30      //功能设定(基本指令)
      0x34      //功能设定(扩充指令)
      0x36      //打开绘图(扩充指令)
*****************************************************************/

#define FIRST_ADDR 0       //定义字符/汉字显示起始位置
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
*	LCD初始化
*
*/
void lcd12864_init(void)
{
    delay_ms(300);
    LCDIO_Init();
    lcdreset();                    //初始化LCD屏
    clrgdram();
	
//Disp_Img(music_sign);
	
	
}
void lcdreset(void)
{
    //delay_ms(100);
    write_com(0x30);

    delay_ms(1);             //选择基本指令集
    write_com(0x30);       //选择8bit数据流
    delay_ms(1);
    write_com(0x0c);       //开显示(无游标、不反白)
    delay_ms(1);
    write_com(0x01);       //清除显示，并且设定地址指针为00H
    delay_ms(11);
    write_com(0x06);       //指定在资料的读取及写入时，设定游标的移动方向及指定显示的移位
    delay_ms(1);

}
void clrgdram(void)
{
    unsigned char x, y;

    for(y = 0; y < 64; y++)
        for(x = 0; x < 16; x++)
        {
            write_com(0x34);
            write_com(y + 0x80);      //行地址
            write_com(x + 0x80);   //列地址
            write_com(0x30);
            write_data(0x00);
            write_data(0x00);
        }
}

void LCDIO_Init(void)
{
    //Init_LCDIO();
    RCC->APB2ENR |= 1 << 8; //使能PORTG时钟

    GPIOG->CRL &= 0X000000FF;	//PG234567 推挽输出 	  RRD
    GPIOG->CRL |= 0X33333300;

    RCC->APB2ENR |= 1 << 5; //使能PORTD时钟
    GPIOD->CRH &= 0X00000000;	//PD8-15 推挽输出 	WR
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





/*初始化 port c 为12864 需要的输出功能
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
//数据线(单向)*******************************************************
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
    delay_us(22);////////////////////在数据写入的时候加入适当的延时
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
    // delay_us(8);///////////////////在数据写入的时候加入适当的延时
    Lcd_Bus(Dispdata);
    delay_us(1);
    LCD_EN_0;
    delay_us(1);
}



unsigned char ST7920_ReadData(void)
{
    u16 uchRevData;
    u8 i;
    ck_busy();   //忙碌检测
    //设置为输入
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

#define BASIC_SET 0x00 //基本指令集，后面的数字查数据手册，下同。??
#define EXTEND_SET 0x34 //扩展指令集??
#define DRAW_ON 0x36 //绘图显示开??
#define DRAW_OFF 0x30 //绘图显示关?

void lcd_set_dot(unsigned char x, unsigned char y, unsigned char color)
{
    unsigned char x_byte, x_bit; //在横坐标的哪一个字节，哪一个位
    unsigned char y_byte, y_bit;

    unsigned char tmph, tmpl; //定义两个临时变量，用于存放读出来的数据

    x &= 0x7F;
    y &= 0x3F;

    x_byte = x / 16; //算出它在哪一个字节(地址)  //注意一个地址是16位的

    x_bit = x & 0x0F; //算出它在哪一个位

    y_byte = y / 32; //y是没在哪个字节这个说法  //这里只是确定它在上半屏还是下半屏  //0:上半屏 1:下半屏

    y_bit = y & 0x3F; //y_bit确定它是在第几行

    if(y_bit > 31)
    {
        y_bit = y_bit - 32;
    }

    write_com(0X34); //关闭绘图显示
    write_com(0X36);
    write_com(0x80 + y_bit); //先写垂直地址(最高位必须为1)  //具体参照数据手册
    write_com(0x80 + x_byte + 8 * y_byte);
//水平坐标
//下半屏的水平坐标起始地址为0x88
//(+8*y_byte)就是用来确定
//在上半屏还是下半屏

    ST7920_ReadData(); //先空读一次
    tmph = ST7920_ReadData(); //读高位
    tmpl = ST7920_ReadData();
    write_com(0x80 + y_bit); //读操作会改变AC，所以重新设置一次
    write_com(0x80 + x_byte + 8 * y_byte);

    if(x_bit < 8) //如果x_bit位数小于8
    {
        if(color)
        {
            write_data(tmph | (0x01 << (7 - x_bit)));  //写高字节。因为坐标是从左向右的//而GDRAM高位在左，底位在右
            write_data(tmpl); //原数据送回
        }
        else
        {
            write_data(tmph & (~(0x01 << (7 - x_bit))));  //写高字节。因为坐标是从左向右的//而GDRAM高位在左，底位在右
            write_data(tmpl); //原数据送回
        }
    }
    else
    {
        if(color)
        {
            write_data(tmph); //原数据送回
            write_data(tmpl | (0x01 << (15 - x_bit)));
        }
        else
        {
            write_data(tmph); //原数据送回
            write_data(tmpl & (~(0x01 << (15 - x_bit))));
        }
    }

    ck_busy();

    write_com(0X30);


}

u16  lcd_get_dot(unsigned char x, unsigned char y)
{
    unsigned char x_byte, x_bit; //在横坐标的哪一个字节，哪一个位
    unsigned char y_byte, y_bit;

    unsigned char tmph, tmpl; //定义两个临时变量，用于存放读出来的数据

    u8 data;
    x &= 0x7F;
    y &= 0x3F;

    x_byte = x / 16; //算出它在哪一个字节(地址)  //注意一个地址是16位的

    x_bit = x & 0x0F; //算出它在哪一个位

    y_byte = y / 32; //y是没在哪个字节这个说法  //这里只是确定它在上半屏还是下半屏  //0:上半屏 1:下半屏

    y_bit = y & 0x3F; //y_bit确定它是在第几行

    if(y_bit > 31)
    {
        y_bit = y_bit - 32;
    }

    write_com(0X34); //关闭绘图显示
    write_com(0X36);
    write_com(0x80 + y_bit); //先写垂直地址(最高位必须为1)  //具体参照数据手册
    write_com(0x80 + x_byte + 8 * y_byte);
//水平坐标
//下半屏的水平坐标起始地址为0x88
//(+8*y_byte)就是用来确定
//在上半屏还是下半屏

    ST7920_ReadData(); //先空读一次
    tmph = ST7920_ReadData(); //读高位
    tmpl = ST7920_ReadData();

//write_com(0x80 + y_bit); //读操作会改变AC，所以重新设置一次
//write_com(0x80 + x_byte + 8 * y_byte);
    if(x_bit < 8) //如果x_bit位数小于8
    {
        data = tmph & (0x01 << (7 - x_bit));  //写高字节。因为坐标是从左向右的//而GDRAM高位在左，底位在右

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

//显示一组汉字
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
                write_com(0x36);//功能设置---8BIT控制界面，扩充指令集
                write_com(y + 0x80);      //行地址
                write_com(x + 0x80 + i); //列地址
                write_com(0x30);
                write_data(music_sign[j++]);
                write_data(music_sign[j++]);
            }
}


//顺向逐行式
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
            write_com(y + 0x80);      //行地址
            write_com(x + 0x80);   //列地址
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
            write_com(y + 0x80);    //行地址
            write_com(x + 0x80);   //列地址
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
                write_com(0x36);//功能设置---8BIT控制界面，扩充指令集
                write_com(y + 0x80);      //行地址
                write_com(x + 0x80 + i); //列地址
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
*		画一条横线，
*				X0:横坐标
*				X1：长度
*				Y :纵坐标
*		 color：1显示，0不显示
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
//画一条竖线
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
*		画一个矩形框
*/
//void input_rect(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
//{
//	v_Lcd12864DrawLineX_f(x0,y0,x1,1);		//在OLED显示屏上绘制一条亮/暗的横线
//	v_Lcd12864DrawLineX_f(x0,y1,x1,1);		//在OLED显示屏上绘制一条亮/暗的横线
//	v_Lcd12864DrawLineY_f(x0,y0,y1,1);		//在显示屏上显示一条亮/暗的竖线
//	v_Lcd12864DrawLineY_f(x1,y0,y1,1);		//在显示屏上显示一条亮/暗的竖线
//}

int ABS(int num)
{

    if(num < 0)
        return 0 - num;
    else
        return num;
}








