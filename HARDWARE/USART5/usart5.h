#include "main.h"
#ifndef __USART5_H
#define __USART5_H	  	     
 







typedef struct LCDM_B
{
	uint8 Air_Value;
	uint8 Pump_Value;
	uint8 Flame_Value;
	uint8 Auto_Paiwu_Value;
	uint8 Normal_Value;  //���涯����ֵ
	
	
}LCD_MB;








typedef struct _LCD_GG_
{
	
	uint8 Lcd_Cmd;
	uint8 Start_Address_H;
	uint8 Start_Address_L;

	uint8 Data_00H;
	uint8 Data_00L;// ���������ַ
	uint8 Data_01H;
	uint8 Data_01L;//
	
	uint8 Data_02H;
	uint8 Data_02L;//ˮ�ö�����ַ
	uint8 Data_03H;
	uint8 Data_03L;//

	uint8 Data_04H;
	uint8 Data_04L;//���涯����ַ
	uint8 Data_05H;
	uint8 Data_05L;//

	uint8 Data_06H;
	uint8 Data_06L;//�Զ����۱���δʹ��
	uint8 Data_07H;
	uint8 Data_07L;// 

	
	uint8 Data_08H;
	uint8 Data_08L;	//�����棬����ʱ��������ʾ	
	uint8 Data_09H;
	uint8 Data_09L;// 
	
	
	
}LCD_SDATA;








///////////////////////////////////////USART2 printf֧�ֲ���//////////////////////////////////
void U5_send_byte(u8 data);
void U5_send_str(u8 *str,u8 s);



////////////////////////////////////////////////////////////////////////////////////////////////
void U5_Printf(char* fmt,...);
///////////////////////////////////////USART2 ��ʼ�����ò���//////////////////////////////////	    
//���ܣ���ʼ��IO ����2
//�������
//bound:������
//�������
//��
//////////////////////////////////////////////////////////////////////////////////////////////
void uart5_init(u32 bound);	

void send_byte(u8 data);
void send_str(u8 *str,u8 s);







#endif




