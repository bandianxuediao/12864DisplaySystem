//=================================================================================
// 版权: bangiunion
//---------------------------------------------------------------------------------
// 作者：     时间：      版本：V0.0.4
//---------------------------------------------------------------------------------
// 修改历史：
// 修改人：  		 修改时间：			 修改内容：	     检查人：
//---------------------------------------------------------------------------------
// 文件：oled.h
//---------------------------------------------------------------------------------
// 介绍：oled显示模块函数的定义
//---------------------------------------------------------------------------------
// 函数列表：
//
//	 	clr_disp_mem:清除显存数据
//	 	oled_clrscr：清屏
//	 	oled_hline：在OLED显示屏上绘制一条亮/暗的横线
//	 	oled_init：OLED初始化
//	 	oled_off：关闭OLED显示屏
//	 	oled_print：字符输出函数
//	 	oled_print_buff：把要显示的汉字/字符数据放入缓冲区中的对应位置
//	 	oled_print_buff32：把要显示的字符的字模数据放入OLED显示器缓存中
//	 	oled_print16x32：数字输出函数（起爆倒计时用）
//	 	oled_print32x32:起爆时输出'起爆'二字
//	 	oled_putchar:单个英文字符的输出
//	 	oled_putchn:汉字输出函数
//	 	oled_rectangle:在显示屏上绘制一个空心矩形框
//	 	oled_rectanglefill:绘制一个有填充颜色的矩形
//	 	oled_rline:在显示屏上显示一条亮/暗的竖线
//	 	oled_updatescr：屏幕刷新
//	 	swap：交换缓冲区
//---------------------------------------------------------------------------------
//	BUSCON: Bus Control Register (SFR 9Dh, reset value EBh)
//	CCON0: Clock Control Register (SFR F9h, reset value 10h)	
//---------------------------------------------------------------------------------
// 备注：	
//=================================================================================

#ifndef _OLED_H_
#define _OLED_H_

#define FRONT_CHAR 0
#define FRONT_CN	 0


#define FIRST_ADDR 0       //定义字符/汉字显示起始位置
extern u8 blank[] ;
extern unsigned char music_sign[];
extern u8 blank2[];
extern unsigned char  new_front_state;


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
  //全局变量定义
  extern unsigned char  oled_disp_buf[OLED_PAGE][OLED_X_MAX];
  extern unsigned int   oled_disp_length;	//显示缓存大小  
  extern unsigned char  oled_disp_buf_pre[OLED_PAGE][OLED_X_MAX];
  
  //函数定义
  extern unsigned char swap(unsigned int *p1, unsigned int *p2);
  //extern unsigned char oled_point(unsigned char x,unsigned char y,unsigned char color);
  extern void oled_hline(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char color);
  extern void oled_rline(unsigned char x0,unsigned char y0,unsigned char y1,unsigned char color);
  extern void oled_rectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color);
  extern void oled_rectanglefill(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color);
  extern void oled_putchn(unsigned int x, unsigned int y, unsigned char *str);
  extern void oled_putchar(unsigned int x, unsigned int y,  unsigned char *str);
  extern void oled_print(unsigned int x, unsigned int y, unsigned char *str);
  extern void oled_print_buff(unsigned char x, unsigned char y,unsigned char *str,unsigned char type);
  extern void oled_print16x32(unsigned int x, unsigned int y, unsigned char num);	  /*显示1-5，16X32的字符*/
  extern void oled_print32x32(unsigned int x, unsigned int y, unsigned char num);     /*显示‘起爆’，32x32字符*/

  extern void clr_disp_mem(void);
  extern void oled_updatescr(unsigned char sl, unsigned char n);
  extern void oled_clrscr(void);
  extern void oled_off(void);
  extern void oled_init(void);
//  extern void oled_reset(void);
	extern void LEDIO_Init(void);
	extern void lcd12864_init(void);
	extern void Lcd_Bus(unsigned char cmd);       //MCU ------> LCD
	extern void write_com(unsigned char cmdcode);
	extern void write_data(unsigned char Dispdata);
	extern void ck_busy(void);
	extern void hzkdis(unsigned char *s);
	extern void clrscreen(void);
	extern void clrgdram(void);
	extern void oled_updatachar(void);
	
#endif
