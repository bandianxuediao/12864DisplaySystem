#include"HanShu.H"

/*************************************************************************************
���������a1��a2��a3��a4�ֱ�Ϊһ��32λ���ݵĸ��ֽڡ��θ��ֽڡ��ε�λ�ֽڡ���λ�ֽ�
����ֵ��32λ��ֵTemp������a1��a2��a3��a4ƴ�Ӷ���
*************************************************************************************/
uint32_t A1A2A3A4_SPELLED_32bitsWord(uint8_t a1,uint8_t a2,uint8_t a3,uint8_t a4)
{
uint32_t Temp;
Temp=((((uint32_t)a1)<<24)|(((uint32_t)a2)<<16)|(((uint32_t)a3)<<8)|(((uint32_t)a4)));
return Temp;
}