#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ϵͳ�жϷ������û�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/10
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************  
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}
//����������ƫ�Ƶ�ַ
//NVIC_VectTab:��ַ
//Offset:ƫ����			 
//void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)	 
//{ 	   	 
//	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//����NVIC��������ƫ�ƼĴ���
//	//���ڱ�ʶ����������CODE��������RAM��
//}

////����������ִ���������踴λ!�����������𴮿ڲ�����.		    
////������ʱ�ӼĴ�����λ		  
//void MYRCC_DeInit(void)
//{	
// 	RCC->APB1RSTR = 0x00000000;//��λ����			 
//	RCC->APB2RSTR = 0x00000000; 
//	  
//  	RCC->AHBENR = 0x00000014;  //˯��ģʽ�����SRAMʱ��ʹ��.�����ر�.	  
//  	RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.			   
//  	RCC->APB1ENR = 0x00000000;   
//	RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION	 															 
//	RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
//	RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
//	RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
//	RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
//	RCC->CIR = 0x00000000;     //�ر������ж�		 
//	//����������				  
//#ifdef  VECT_TAB_RAM
//	MY_NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
//#else   
//	MY_NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
//#endif
//}


//ϵͳʱ�ӳ�ʼ������
//pll:ѡ��ı�Ƶ������2��ʼ�����ֵΪ16		 
//void Stm32_Clock_Init(u8 PLL)
//{
//	unsigned char temp=0;   
//	MYRCC_DeInit();		  //��λ������������
// 	RCC->CR|=0x00010000;  //�ⲿ����ʱ��ʹ��HSEON
//	while(!(RCC->CR>>17));//�ȴ��ⲿʱ�Ӿ���
//	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
//	PLL-=2;//����2����λ
//	RCC->CFGR|=PLL<<18;   //����PLLֵ 2~16
//	RCC->CFGR|=1<<16;	  //PLLSRC ON 
//	FLASH->ACR|=0x32;	  //FLASH 2����ʱ����

//	RCC->CR|=0x01000000;  //PLLON
//	while(!(RCC->CR>>25));//�ȴ�PLL����
//	RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��	 
//	while(temp!=0x02)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
//	{   
//		temp=RCC->CFGR>>2;
//		temp&=0x03;
//	}    
//}		


