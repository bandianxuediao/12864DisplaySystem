#ifndef __ST7920_H
#define __ST7920_H			  	 
#include "sys.h"
#include "stdlib.h"	    


#define FIRST_ADDR 0       //定义字符/汉字显示起始位置
extern u8 blank[] ;
extern unsigned char music_sign[];
extern u8 blank2[];


#define nop   delay_us(3)

#define DATAOUT(x) GPIOD->ODR=(GPIOD->ODR&0x00ff)|((x<<8)&0xFF00); //输出 

#define  LCD_PSB_S   LCD_PSB_=1
#define  LCD_PSB_R   LCD_PSB_=0

#define  LCD_CSB_S   LCD_CSB_=1
#define  LCD_CSB_R   LCD_CSB_=0

#define  LCD_CSA_S   LCD_CSA_=1
#define  LCD_CSA_R   LCD_CSA_=0

#define  LCD_EN_S    LCD_EN_=1
#define  LCD_EN_R    LCD_EN_=0

#define  LCD_RW_S    LCD_RW_=1
#define  LCD_RW_R    LCD_RW_=0

#define  LCD_RS_S    LCD_RS_=1
#define  LCD_RS_R    LCD_RS_=0


//#define  LCD_PSB_   PGout(2)

#define  LCD_CSB_   PGout(2)
#define  LCD_CSA_   PGout(3)

#define  LCD_EN_    PGout(5)

#define  LCD_RW_    PGout(4)

#define  LCD_RS_    PGout(6)

  //oled显示屏的范围定义
  #define OLED_X_MAX  128
  #define OLED_Y_MAX  64
  #define OLED_PAGE   8	   

//#define  LCD_RST   PGout(7)


//PBG-15 ,作为数据线
//#define DATAOUT(x) GPIOB->ODR=(GPIOB->ODR&0x00ff)|((x<<8)&0xFF00); //输出 

#define PAGE_0   0xb8          //0xb8--0xbf   0--8 page
#define COL_0    0x40          //0x40--0x7f   0--64 col
#define FirstLine_0  0xc0      //0xc0--0xff   0--64 line

#define DISPZF_END  0x24       //0x24 = '$'
#define DISPHZ_END  0xff 
extern unsigned char gx,gy;//lcd坐标
void Init_LCDIO(void);
void LCDIO_Init(void);
void LcdOn(void);
void LcdOff(void);
void LcdClrScreen(void);
void DispFirstLine(u8);
void display_GLCD_data8(const unsigned char *p);
void display_GLCD_data16(const unsigned char *str);
void gotoxy(unsigned char x,unsigned char y);	 
void display_GLCD_data14(const unsigned char *str);
void display_GLCD_data24(const unsigned char *str);


void lcd12864_init(void);
void lcdreset(void);
void ceshi(void);
void dis_ASCII(char x,char y,char num);
void dis_num(char x,char y,char num);
void write_data(unsigned char Dispdata);
void write_com(unsigned char cmdcode);
void xy_hzkdis(char x,char y,unsigned char *s);
void dis_num32(char x,char y,u32  num);
void dis_num16_3(char x,char y,u16 num);
void dis_num16_4(char x,char y,u16 num);
void clrscreen(void );
void CLR_screen(void );
void Disp_hz16x16(const unsigned char *img,char x,char y);
void Disp_hz8x16(const unsigned char *img,char x,char y);
void clrgdram(void );
void test(void );
void lcdfill(unsigned char disdata);
void lcd_set_dot(unsigned char x, unsigned char y,unsigned char color) ;
u16  lcd_get_dot(unsigned char x, unsigned char y) ;
void  v_Lcd12864DrawLine_f(  unsigned  char  StartX,  unsigned  char  StartY,  unsigned  char  EndX,unsigned char EndY, unsigned char Color );
void Dis_menu(void);
void Disp_Img(unsigned char *img);
void hzkdis(unsigned char *s);
extern void put_out(unsigned char x, unsigned char y, unsigned char *str);

#endif  
	 



