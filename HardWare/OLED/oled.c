//=================================================================================
// 文件：oled.c
//---------------------------------------------------------------------------------
// 版权: bangiunion
//---------------------------------------------------------------------------------
// 作者：     时间：       文件版本：V0.0.4	
// 检查：          时间：  
//---------------------------------------------------------------------------------
// 修改历史：
// 修改人：  		 修改时间：			 修改内容：		   检查人：
//---------------------------------------------------------------------------------
// 介绍：oled显示模块函数的定义
//---------------------------------------------------------------------------------
//	BUSCON: Bus Control Register (SFR 9Dh, reset value EBh)
//	CCON0: Clock Control Register (SFR F9h, reset value 10h)	
//---------------------------------------------------------------------------------
// 备注：	
//===================================================================================

#include "includes.h"
#undef code
#define code const
#include "HZDoc12x12.h"
#include "HZDoc32x32.h"
#include "Char6x12.h"
#include "Char16x32.h"
unsigned char  new_front_state;
unsigned char  new_oled_char[512];
unsigned char  new_oled_cn[1024];
unsigned char	 new_oled_buff[64][16];
unsigned char  oled_disp_buf[OLED_PAGE][OLED_X_MAX];      //oled显示器缓存
unsigned int   oled_disp_length = OLED_PAGE * OLED_X_MAX;	//显示缓存大小
unsigned char  oled_disp_buf_pre[OLED_PAGE][OLED_X_MAX];  //oled显示器缓存

//----------函数定义------------------------------------------------------
void oled_print_buff32(unsigned char x, unsigned char y,unsigned char *str);

//用于操作显存数据的共用体
typedef union 	 
{
	unsigned char ucdata[4];
	unsigned long uldata;
} ucldata;

//==================================================================================================
//| 函数名称 | swap
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 交换缓冲区
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | *p1、*p2为要交换的数据存储的地址值
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 0-成功，1－输入参数有误    
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   |      
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
unsigned char swap(unsigned int *p1, unsigned int *p2)
{
	unsigned int c;

	if ((p2 == NULL) || (p1 == NULL)) //判断指针是否正确
		return 1;

	c   = *p1;
	*p1 = *p2;
	*p2 = c;
	return 0;
}

/*******************************************************************************
*函数功能：把要显示点数据放入缓冲区中的对应位置(只操作一列)
*输入参数：(x,y)-显示位置 color = 0不点亮，color ＝ 1点亮
*返回值：	0-成功，1－输入参数有误
*备注：    只操作缓冲区   
********************************************************************************/
/*unsigned char oled_point(unsigned char x,unsigned char y,unsigned char color)
{
   if((x/8>OLED_PAGE)||(y>OLED_X_MAX-1))
   {
      return 1;
   }
   if(color==0)
   {
	  oled_disp_buf[x/8][y] = oled_disp_buf[x/8][y]&(0xFE<<(x%8));
   }
   else
   {
	  oled_disp_buf[x/8][y] = oled_disp_buf[x/8][y]|(0x01<<(x%8))	;
   }
   
   return 0;  
}
*/

//==================================================================================================
//| 函数名称 | oled_hline
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 在OLED显示屏上绘制一条亮/暗的横线
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | swap：交换数据缓冲区
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x0,y0)-起始坐标：0<x0<127,0<y0<63
//|          | x1-结束横坐标：0<x1<127
//|          | color：0表示暗线，1表示亮线
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 |    
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 | oled_disp_buf：oled显示缓存
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区       
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_hline(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char color)
{
	unsigned char  i; //循环控制变量
	unsigned char  data_page ,data_buff; //data_page:页地址   data_buff：计算时的中间变量

	//----判断终点坐标是否大于起点坐标------------
	if (x0 > x1)//x0必须小于x1,否则交换两个的值
	{
		swap((unsigned int *)&x0,(unsigned int *)&x1);
	}

	if ((y0>OLED_Y_MAX-1) || (x0>OLED_X_MAX-1))	   
	{
		return;
	}

	if (x1 > OLED_X_MAX-1)
	{
		x1 = OLED_X_MAX -1;
	}

	//---------------绘制横线-------------------
	data_page = y0/8;  //计算起始点所处的页地址
	if (color == 0)	   //判断是画亮线还是画暗线
	{
		data_buff = 0x01<<(y0%8); //计算要置为0的点在本页中的位置
		data_buff = ~data_buff;	  //取反
		for (i = x0; i < x1+1; i++)
		{
			oled_disp_buf[data_page][i] = oled_disp_buf[data_page][i]&data_buff;//绘制横线（改变OLED显示缓存）
		}
	}
	else
	{
		data_buff = 0x01<<(y0%8); //计算要置为1的点在本页中的位置
		for (i = x0; i < x1+1; i++)
		{
			oled_disp_buf[data_page][i] = oled_disp_buf[data_page][i]|data_buff;	//绘制横线（改变OLED显示缓存）
		}
	}
}

//==================================================================================================
//| 函数名称 | oled_rline
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 在显示屏上显示一条亮/暗的竖线
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | swap：交换数据缓冲区
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x0,y0)-起始坐标：0<x0<127,0<y0<63
//|          | y1-结束横坐标：0<y1<63
//|          | color：0表示暗线，1表示亮线
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |oled_disp_buf：oled显示缓存
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区 ,x范围是0-127,y范围是0-63       
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_rline(unsigned char x0,unsigned char y0,unsigned char y1,unsigned char color)
{
	unsigned char  i;	 //循环控制变量
	unsigned char  start_page,end_page; //计算页地址变量

	//------------整理输入的参数-----------------------------------
	if (y0 > y1)  //如果y0>y1则交换彼此数据
	{
		swap((unsigned int *)&y0,(unsigned int *)&y1);
	}

	if ((y0>OLED_Y_MAX-1) || (x0>OLED_X_MAX-1))//判断输入参数是否超出OLED显示屏的范围	   
	{
		return;
	}

	if (y1 > OLED_Y_MAX-1)
	{
		y1 = OLED_PAGE -1;
	}

	//-----------------绘制竖线-------------------------------------
	start_page = y0/8; //计算起始页地址
	end_page = y1/8;   //计算结束的页地址

	if (start_page == end_page)	//要绘制的线在同一页内，无法优化
	{
		if (color == 0) //绘制暗线
		{
			// oled_disp_buf[start_page][x0] &= ((0xFF>>(y0%8))|(0xFF<<(y1%8)));	//修改((0xFF>>(y0%8))|(0xFF<<(y1%8)))
			oled_disp_buf[start_page][x0] &= ~((0xFF<<(y0%8))&(0xFF<<(7-(y1%8))));       //画线时包括端点
		}
		else
		{
			oled_disp_buf[start_page][x0] |= (0xFF<<(y0%8)&(0xFF>>(7-(y1%8))));
		}
	}
	else
	{
		if (color == 0)	//color == 0代表暗线
		{
			if ((y0%8) != 0) //先绘制头
			{
				oled_disp_buf[start_page][x0] &=~(0xFF<<(y0%8)); //移位，和OLED显示屏缓存对齐
				start_page = start_page + 1;
			} 
			if (y1%8 != 7)	//绘制尾
			{
				oled_disp_buf[end_page][x0] &=~(0xFF>>(7-(y1%8)));//移位，和OLED显示屏缓存对齐
				end_page = end_page -1;
			}
			for (i = start_page; i <= end_page; i++)//绘制中间
			{
				oled_disp_buf[i][x0] =0x00;
			}
		}
		else
		{
			if ((y0%8) != 0) //先绘制头
			{
				oled_disp_buf[start_page][x0] |=(0xFF<<(y0%8));//移位，和OLED显示屏缓存对齐
				start_page = start_page + 1;
			} 
			if (y1%8 != 7)	//绘制尾
			{
				oled_disp_buf[end_page][x0] |=(0xFF>>(7-(y1%8)));//移位，和OLED显示屏缓存对齐
				end_page = end_page - 1;
			}
			for (i=start_page;i<=end_page;i++) //绘制中间
			{
				oled_disp_buf[i][x0] =0xFF;
			}
		}
	}
}

//==================================================================================================
//| 函数名称 | oled_rectangle
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 在显示屏上绘制一个空心矩形框
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | swap：交换数据缓冲区
//|          | oled_hline：绘制横线
//|          | oled_rline：绘制竖线
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x0,y0)-起始坐标：0<x0<127,0<y0<63
//|          | (x1,y1)-结束坐标：0<x1<127,0<y2<63
//|          | color：0表示暗线，1表示亮线
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区     
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_rectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color)
{
	//------------处理输入参数--------------------------
	if(x0>x1)
	{
		swap((unsigned int *)&x0,(unsigned int *)&x1);
	}
	if(y0>y1)
	{
		swap((unsigned int *)&y0,(unsigned int *)&y1);
	}

	//判断输入参数是否超出OLED显示屏范围
	if((y1>OLED_Y_MAX-1)||(x1>OLED_X_MAX-1))
	{
		return;
	}

	//------------绘制方框-----------------------------
	oled_hline(x0,y0,x1,color);//在OLED显示屏上绘制一条亮/暗的横线
	oled_hline(x0,y1,x1,color);//在OLED显示屏上绘制一条亮/暗的横线
	oled_rline(x0,y0,y1,color);//在OLED显示屏上绘制一条亮/暗的横线
	oled_rline(x1,y0,y1,color);//在OLED显示屏上绘制一条亮/暗的横线

	return;
}

//==================================================================================================
//| 函数名称 | oled_rectanglefill
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 绘制一个有填充颜色的矩形
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | swap：交换数据缓冲区
//|          | oled_rline：绘制竖线
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x0,y0)-(x1,y1)对角坐标
//|          |         0<x0<127,0<y0<63
//|          |         0<x1<127,0<y2<63
//|          | color：0表示暗线，1表示亮线	 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区    
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_rectanglefill(unsigned char x0,unsigned char y0,unsigned char x1,
                        unsigned char y1,unsigned char color)
{
	unsigned char  i;	//循环控制变量

	//------------处理输入参数--------------------------
	if (x0 > x1)
	{
		swap((unsigned int *)&x0,(unsigned int *)&x1);
	}
	if (y0 > y1)
	{
		swap((unsigned int *)&y0,(unsigned int *)&y1);
	}

	//判断输入参数是否超出OLED显示屏范围
	if ((y0>OLED_Y_MAX-1) || (x0>OLED_X_MAX-1))
	{
		return;
	}

	//----绘制一个有填充颜色的矩形----------------------
	for (i = x0; i <= x1; i++)
	{
		oled_rline(i, y0, y1, color);
	}

	return;
}

//==================================================================================================
//| 函数名称 | oled_putchn 
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 汉字输出函数
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | oled_print_buff：把要显示的汉字/字符数据放入缓冲区中的对应位置
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x,y)显示坐标： 0<x<127,0<y<63
//|          | *str：要显示的字符的地址
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区      
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_putchn(unsigned int x, unsigned int y, unsigned char *str)
{
	unsigned int i;	         //循环控制变量
	unsigned char *p = NULL; //地址缓存

	//判断输入的字符是否超出OLED显示屏
	if ((x>OLED_X_MAX-1) || (y>OLED_Y_MAX-1))
	{
	   return;
	}

	//查找字模库中的文字
	for (i = 0; (*str != hzIndex[i]) || (*(str+1) != hzIndex[i+1]); i += 2)
	{
		if (i > (hzNum*2))
			break;
	}

	//把汉字转换成字模数据
	if ( i < hzNum*2 )
		p = (unsigned char*)&hzdot[0] + i*12;
	else 
		return;

		for(i=y;i<=(y+11);i++)
		{
			new_oled_buff[i][2*x]=*p;
			new_oled_buff[i][2*x+1]=*(++p);
			p++;
		}
//			x+=2;
	
	
////	//调用oled_print_buff把字模数据存入OLED显示器缓存中   
////	for (i = 0; i < 12; i++)
////	{
////		oled_print_buff(x+i, y , (unsigned char *)p, 1);
////		p += 2;   
////	}

	return;		   //结构完整
}
void oled_print_buf(unsigned char x, unsigned char y,unsigned char *str,unsigned char type)
{


}

//==================================================================================================
//| 函数名称 | oled_putchar
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 单个英文字符的输出
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | oled_print_buff：把字模数据放入OLED显示缓存，
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x,y)显示坐标： 0<x<127,0<y<63
//|          | *str：要显示的字符的地址
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区     
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_putchar(unsigned int x, unsigned int y,  unsigned char *str)
{
	unsigned int i;	         //循环控制变量
	unsigned char *p = NULL; //地址缓冲变量

	//判断输入的字符是否超出OLED显示屏
	if ((x>OLED_X_MAX-1) || (y>OLED_Y_MAX-1))
	{
		return;
	}

	//调整ASIC码为了和字模库对应，因为字模库中的第一个字模为ASIC中的
	//第32个字符
	i = (*str - 0x20);

	//判断要显示的字符是否为可显示的字符，如果是则给出在字模库中的地址
	if (i < CHAR_NUMBER)
		p = (unsigned char*)&chardot[0] + i*12;
	else
		return; 

	if(new_front_state==1)
	{
		for (i = 0; i < 12; i++)
		{
			new_oled_buff[i+3][2*x]=*p;
//			new_oled_buff[i][2*x+1]=*(++p);
			p++;
		}
	}
	if(new_front_state==0)
	{
		for (i = 0; i < 12; i++)
		{
//			new_oled_buff[i][2*x]=chardot[dot_index];
			new_oled_buff[i+3][2*x+1]=*p;
//			dot_index++;
			p++;
		}
	}
	
	
//	//显示字符   
//	for (i = 0; i < 6; i++)
//	{
//		oled_print_buff(x+i,y, (unsigned char *)p,0);
//		p +=2;
//	}

	return;
}

//==================================================================================================
//| 函数名称 | oled_print 
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 字符输出函数。调用底层的字符输出函数和汉字输出函数进行字符的输出 
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | oled_putchar：输出英文字符
//|          | oled_putchn：输出汉字字符
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x,y)显示坐标： 0<x<127,0<y<63
//|          | *str：要显示的字符的地址
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区     
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_print(unsigned int x, unsigned int y, unsigned char *str)
{
	unsigned int bakx, baky;//坐标缓存变量

	bakx = x; 
	baky = y;
	if (str == NULL)//判断地址是否为空
		return;

	//setup_font_color(); 
	while (*str != '\0')//只要字符串没有结束，就继续循环
	{
		//判断坐标是否超出OLED显示范围
		if(bakx > (OLED_X_MAX-1)) return;	
		if(baky > OLED_Y_MAX - 1)      return;

		//判断是汉字还是英文字符
		if ((*str & 0x80) != 0)
		{
			oled_putchn(bakx, baky, str);  //汉字
			new_front_state=0;
			str += 2;
			bakx++;
		}
		else
		{
			if(new_front_state==1)
			{
				bakx--;
				new_front_state=0;
			}else{new_front_state=1;}//暂时只允许连续两个西文
			oled_putchar(bakx, baky, str); //字符
			
			str++;
			bakx++ ;
		}
	}
	return;
}

//==================================================================================================
//| 函数名称 | oled_print16x32
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数字输出函数（起爆倒计时用）
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | oled_print_buff32：把要显示的汉字/字符数据放入缓冲区中的对应位置模块
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x,y)显示坐标： 0<x<127,0<y<63
//|          | num：要输出的数字
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区     
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_print16x32(unsigned int x, unsigned int y, unsigned char num)
{
    unsigned char i;        //循环控制变量
    unsigned char *p = NULL;//地址缓存

	//判断输入参数是否有误
	if ((x>OLED_X_MAX-1) || (y>OLED_Y_MAX-1))
	{
	   return;
	}

	//只有1-5这几个数字，
	if ((num>5) && (num<=0))	
	{
	   return;
	}
    p = (unsigned char *)chardot16x32 + (num-1)*16*4;//数字字模地址
      
    //把字符输出到OLED显存中
	for (i = 0; i < 16; i++)
    {
        oled_print_buff32(x+i, y, p);
		p += 4;
    }
	return;    
}

//==================================================================================================
//| 函数名称 | oled_print32x32
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 起爆时输出'起爆'二字
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | oled_print_buff32：把要显示的汉字/字符数据放入缓冲区中的对应位置模块
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x,y)显示坐标： 0<x<127,0<y<63
//|          | num：要输出的字符
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 0-成功，1－输入参数有误或找不到索引， 2—超出范围  
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区     
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_print32x32(unsigned int x, unsigned int y, unsigned char num)
{
	unsigned char i;	     //循环控制变量
	const unsigned char *p = NULL; //地址缓存

	//判断输入参数是否有误
	if ((x>OLED_X_MAX-1) || (y>OLED_Y_MAX-1))
	{
		return;
	}

	//只有"起爆"2个字
	if (num > 1)	
	{
		return;
	}
	p = (unsigned char*)&chardot32x32[0] + num*32*4;	//取得字模地址

	//把字符输出到OLED显存中  
	for(i=0; i<32; i++)
	{
		oled_print_buff32(x+i,y, (unsigned char *)p);
		p += 4;
	}

	return;
}

//==================================================================================================
//| 函数名称 | oled_print_buff32 
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 把要显示的字符的字模数据放入OLED显示器缓存中
//|          |   1.先清空要放入数据的缓存
//|          |   2.把字模的值放入缓存中
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x,y)显示坐标： 0<x<127,0<y<63
//|          | *str：要显示的字符的地址
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 | oled_disp_buf：oled显示器缓存
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区     
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_print_buff32(unsigned char x, unsigned char y,unsigned char *str)
{
   unsigned char data_l ;
   unsigned char start_page ;
   unsigned char i ;
   ucldata udtdata ;	/*用于操作显存数据*/

   start_page =  y/8 ;//找出要存放缓存的起始页

   if(y>OLED_X_MAX-1)
   {
      return  ;
   }

   for(i=0;i<4;i++)
   {
	   data_l = *(str+i) ;
	   /*需要显示的部分清零*/
	   udtdata.uldata = 0 ;
	   udtdata.ucdata[3] = 0xFF ;		/*把低位放入共用体*/
	   udtdata.uldata <<= (y%8) ;		/*移位，与显存数据对齐*/
	/*   oled_disp_buf[start_page][x]更改为oled_disp_buf[start_page%8][x]可以实现循环显示*/
	   oled_disp_buf[start_page+i][x] &= (~udtdata.ucdata[3]) ;  /*前边已经对显示缓存作了判断，此处不再作判断*/
	   if(start_page+1<OLED_PAGE)
	   {
	       oled_disp_buf[start_page+i+1][x] &= (~udtdata.ucdata[2]) ;
	   }
	    /*写入显示数据，使用循环体可以灵活控制显示字的大小*/
	   udtdata.uldata = 0 ;
	   udtdata.ucdata[3] = data_l ;		/*把低位放入共用体*/
	   udtdata.uldata <<= (y%8) ;		/*移位，与显存数据对齐*/
	 /*  oled_disp_buf[start_page][x]更改为oled_disp_buf[start_page%8][x]可以实现循环显示*/
	   oled_disp_buf[start_page+i][x] |= udtdata.ucdata[3] ;		/*前边已经对显示缓存作了判断，此处不再作判断*/
	   if(start_page+1>OLED_PAGE-1)		/*这样写在显示不下的时候，能显示半个字符*/
	   {
	       	return  ;
	   }
	   oled_disp_buf[start_page+i+1][x] |= udtdata.ucdata[2] ; 
	   
	   //start_page = start_page + i ;
   }
         
   return  ;
}

//==================================================================================================
//| 函数名称 | oled_print_buff
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 把要显示的汉字/字符数据放入缓冲区中的对应位置
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | Boot_Flash_Read：从FLASH中读取数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | (x,y)显示坐标： 0<x<127,0<y<63
//|          | *str：要显示的字符的地址
//|          | type：1-汉字，其他-字符
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 | oled_disp_buf：oled显示器缓存
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 只操作缓冲区 ,汉字为12×12的点阵     
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_print_buff(unsigned char x, unsigned char y,unsigned char *str,unsigned char type)
{
	unsigned char data_l = 0,data_h = 0;
	unsigned char start_page;
	
	//udtdata共用体用于操作显存数据
	ucldata udtdata;
	
	//找出要存放缓存的起始页
	start_page =  y/8;
	
	//取出字模中的数据
	data_l = *str;
	data_h = *(str + 1); 
	data_h &= 0x0F;
	
	//-------需要更新的数据区域的缓存清零--------------------
	udtdata.uldata = 0 ;
	udtdata.ucdata[0] = 0xFF ;		//把低位放入共用体
	udtdata.ucdata[1] = 0x0F ;		//把高位放入共用体
	udtdata.uldata <<= (y%8) ;		//移位，与显存数据对齐
	
	//把已经转换好的数据存放如OLED数据缓存
	oled_disp_buf[start_page][x] &= (~udtdata.ucdata[0]); 
	oled_disp_buf[start_page+1][x] &= (~udtdata.ucdata[1]);
	
	//判断是否超出屏幕范围，没有超出则清空缓存
	if(start_page+2<OLED_PAGE)
	{
		oled_disp_buf[start_page+2][x] &= (~udtdata.ucdata[2]);	
	}
	
	//-------写入显示数据，使用循环体可以灵活控制显示字的大小-------
	udtdata.uldata = 0;
	udtdata.ucdata[0] = data_l;		//把低位放入共用体
	udtdata.ucdata[1] = data_h;		//把高位放入共用体
	udtdata.uldata <<= (y%8);		//移位，与显存数据对齐
	
	//oled_disp_buf[start_page][x]更改为oled_disp_buf[start_page%8][x]可以实现循环显示
	oled_disp_buf[start_page][x] |= udtdata.ucdata[0];
	oled_disp_buf[start_page+1][x] |= udtdata.ucdata[1];
	
	//如果判断显示不下的时候，返回1，表示输入参数有误
	if(start_page+2<OLED_PAGE)
	{
		oled_disp_buf[start_page+2][x] |= udtdata.ucdata[2];	
	}

	return ;	
}

//==================================================================================================
//| 函数名称 | clr_disp_mem
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 清除显存数据
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 |    
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 | oled_disp_buf：oled显示器缓存
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   |      
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void clr_disp_mem(void)
{
	memset(oled_disp_buf,0,sizeof(oled_disp_buf));

}

//==================================================================================================
//| 函数名称 | oled_updatescr
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 屏幕刷新
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | s1-刷新起始行（0-63），n-要刷新的行数（1-64）
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 |    
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   | 举例，(0,23)是刷新0-22行，包括0和22
//|          | 刷新方式实际转换为按页刷新    
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_updatachar(void)
{
	unsigned char oled_line,j=0,oled_h_dot;
	unsigned char s,h,dot_line,temp=0;
	unsigned long  dot_index=0;

for(dot_line=0;dot_line<=4;dot_line++)
{	
	for(oled_h_dot=0;oled_h_dot<=7;oled_h_dot++)
	{			
		for(oled_line=dot_line*12;oled_line<=(dot_line*12+11);oled_line++)
		{
			new_oled_buff[oled_line][2*oled_h_dot]=chardot[dot_index];
			new_oled_buff[oled_line][2*oled_h_dot+1]=chardot[dot_index+12];
			dot_index++;
		}
		dot_index+=12;
	}	
}
	for(s=0;s<=64;s++)	
	{
		if(s>31)
		{
			temp=8;
				for(h=temp;h<temp+8;h++)
			{
        write_com(0x36);
        write_com(0x80+s-32);		//往下走1格
        write_com(0x80+h);  //横着往后走16格
        write_com(0x30);
        write_data(new_oled_buff[s][(h-temp)*2]);
        write_data(new_oled_buff[s][(h-temp)*2+1]);
			}			
		}
		else{
	for(h=temp;h<temp+8;h++)
	{
        write_com(0x36);
        write_com(0x80+s);		//往下走1格
        write_com(0x80+h);  //横着往后走16格
        write_com(0x30);
        write_data(new_oled_buff[s][h*2]);
        write_data(new_oled_buff[s][h*2+1]);
	}
}

}
}

void oled_updatescr(unsigned char sl, unsigned char n)
{
	unsigned char oled_line,j=0,oled_h_dot;
	unsigned char s,h,dot_line,temp=0;
	unsigned long  dot_index=0;
//for(dot_line=0;dot_line<=4;dot_line++)
//{	
//	for(oled_h_dot=0;oled_h_dot<=7;oled_h_dot++)
//	{			
//		for(oled_line=dot_line*12;oled_line<=(dot_line*12+11);oled_line++)
//		{
//			new_oled_buff[oled_line][2*oled_h_dot]=chardot[dot_index];
//			new_oled_buff[oled_line][2*oled_h_dot+1]=chardot[++dot_index];
//			dot_index++;
//		}
//	}	
//}


for(s=0;s<=64;s++)	
	{
		if(s>31)
		{
			temp=8;
				for(h=temp;h<temp+8;h++)
			{
        write_com(0x36);
        write_com(0x80+s-32);		//往下走1格
        write_com(0x80+h);  //横着往后走16格
        write_com(0x30);
        write_data(new_oled_buff[s][(h-temp)*2]);
        write_data(new_oled_buff[s][(h-temp)*2+1]);
			}			
		}
		else{
	for(h=temp;h<temp+8;h++)
	{
        write_com(0x36);
        write_com(0x80+s);		//往下走1格
        write_com(0x80+h);  //横着往后走16格
        write_com(0x30);
        write_data(new_oled_buff[s][h*2]);
        write_data(new_oled_buff[s][h*2+1]);
	}
}
	}



											//26
//	unsigned char i,j,aaa,bbb,x=0,y=0,k=0;
//for(j=0;j<2;j++)
//	{
//	for(i = 0; i < 12; i++)
//    {
//			aaa=x + 0x80 + i;
//			bbb=j + 0x80;
//			if(x+i>=32)
//			{
//			if(k>=6){k=0;}
//			aaa=0x80+k;k++;
//			bbb=j+0x80+8;
//			}
//        write_com(0x36);
//        write_com(aaa);
//        write_com(bbb);
//        write_com(0x30);
//        write_data(hzdot[2 * (i+j*12)]);
//        write_data(hzdot[2 * (i+j*12) + 1]);
//    }
//	}

}

//==================================================================================================
//| 函数名称 | oled_clrscr
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 清屏
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 |    
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   |      
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_clrscr(void)
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

//==================================================================================================
//| 函数名称 | oled_off
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 关闭OLED显示屏
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | oled_clrscr：清屏
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 |    
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   |      
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_off(void)
{
    oled_clrscr() ;
//	OLED_RST() ;               //复位OLED
	delay_ms(50);
//    LCD_WR_REG(OLED_DIS_OFF); //关闭OLED显示
}

//==================================================================================================
//| 函数名称 | oled_init
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | OLED初始化，主控芯片为SSD1305
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | mode：0代表命令 ，1代表清屏
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 |    
//|----------|--------------------------------------------------------------------------------------       
//| 全局变量 |
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 编写人：     时间：   
//|----------|-------------------------------------------------------------------------------------- 
//|   备注   |      
//|----------|-------------------------------------------------------------------------------------- 
//| 修改记录 | 修改人：          时间：         修改内容： 
//==================================================================================================
void oled_init(void)
{
//		delay_ms(100);
//    write_com(0x30);
//    delay_ms(1);             //Ñ¡Ôñ»ù±¾Ö¸Áî¼¯
//    write_com(0x30);       //Ñ¡Ôñ8bitÊý¾ÝÁ÷
//    delay_ms(1);
//    write_com(0x0c);       //¿ªÏÔÊ¾(ÎÞÓÎ±ê¡¢²»·´°×)
//    delay_ms(1);
//    write_com(0x01);       //Çå³ýÏÔÊ¾£¬²¢ÇÒÉè¶¨µØÖ·Ö¸ÕëÎª00H
//    delay_ms(11);
//    write_com(0x06);       //Ö¸¶¨ÔÚ×ÊÁÏµÄ¶ÁÈ¡¼°Ð´ÈëÊ±£¬Éè¶¨ÓÎ±êµÄÒÆ¶¯·½Ïò¼°Ö¸¶¨ÏÔÊ¾µÄÒÆÎ»
//    delay_ms(1);
}
