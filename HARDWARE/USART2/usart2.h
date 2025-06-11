#include "main.h"
#ifndef __USART2_H
#define __USART2_H	  	     
 






#define             macUSART2                                USART2

typedef struct _UNION_flags
{
volatile	uint8 Big_Timer_Flag;
volatile	uint8 Big_Timer_Count;
volatile	uint8 Small_Timer_Flag;
volatile	uint8 Small_Timer_Count;
	uint8 Zhu_Need_Flag;
	uint8 Cong_Need_Flag;
	uint8 Zhu_Less_Flag;
	uint8 Cong_Less_Flag;
	
}UNION_FLAGS;


typedef union _kdsUnion_Inf
{
	uint8 RxData[14];
	struct{

	uint8 Slave_Address;  //�ֽ�1
	uint8 Cmd;          //�ֽ�2
	uint8 Add_H;     //�ֽ�3
	uint8 Add_L;     //�ֽ�4
	
	uint8 Start_Cmd;//�ֽ�5����������
	uint8 Close_Cmd;//�ֽ�6���ػ�����
	uint8 Error_Reset_Cmd; //�ֽ�7�����ϸ�λ����
	
	uint8 Big_Power;//�ֽ�8�����ʵ�ֵ
	uint8 Small_Power; //�ֽ�9��С���ʵ�ֵ
	uint8 Big_Time; // �ֽ�10������ʱ��
	
	
	uint8 Small_Time; //С����ʱ��
	uint8 unused12;

	
	uint8 check_H;
	uint8 check_L;
	 
			
		
		}True;
	
}	UtoSlave_Info;  //����ӻ����յ���������Э��Ľ���


typedef union _Mod_Inf
{
	uint8 All[30];
	struct{
	uint8 Address;    //�ֽ�1 �� ��ַ
	uint8 Cmd;			//�ֽ�2 �� ����
	uint8 Length;		//�ֽ�3 �� ���ݳ���
	uint8 Dev_State;//�ֽ�4 �������������ǹر���
	uint8 NeedMore;//�ֽ�5 ����ǰ������Ҫ���Ӹ���
	uint8 NeedLess;//�ֽ�6 ����ǰ���ʲ���Ҫ��ô��

	uint8 Work_State;  //�ֽ�7 ������������״̬
	uint8 Error_Code;//�ֽ�8 �����ϴ���
	uint8 Water_state; //�ֽ�9 ��ˮλ״̬
	uint8 Pressure;  //�ֽ�10 ����ǰѹ��
	uint8 Power; //�ֽ�11 ����ǰ����
	uint8 Smoke;  //�ֽ�12 ������
	uint8 Flame;  //�ֽ�13 ������
	uint8 Water_Dev; //�ֽ�14 ����ˮ�̵���1������0 �ر�
	uint8 Air_Dev; //�ֽ�15 �����
	uint8 unused1;  //�ֽ�16 ������δʹ��
	

	uint8 CWork_State;  //�ֽ�17 ���ӻ�������״̬
	uint8 Alive_State;//�ֽ�18 ���ӻ��Ƿ�����    ***************���ⲻͬ
	uint8 CError_Code;//�ֽ�19 �����ϴ���
	uint8 CWater_state; //�ֽ�20 ��ˮλ״̬
	uint8 CPressure;  //�ֽ�21 ����ǰѹ��
	uint8 CPower; //�ֽ�22 ����ǰ����
	uint8 CSmoke;  //�ֽ�23 ������
	uint8 CFlame;  //�ֽ�24 ������
	uint8 CWater_Dev; //�ֽ�25 ����ˮ�̵���1������0 �ر�
	uint8 CAir_Dev; //�ֽ�26 �����
	
	

	uint8 unused5;  //�ֽ�27 ������δʹ��
	uint8 Paiwu;  //�ֽ�28 �����ӻ����۵ı�־
	uint8 check_H;//�ֽ�29 ������δʹ��
	uint8 check_L;//�ֽ�30 ������δʹ��
	
		}True;
	
}	Duble5_Info;  //˫ƴ1��;�ӻ����͸�����������


typedef struct _LCD_UNIS
{
	
	
	uint16  Start_Close_Flag;
	uint16  Error_Code;
	uint16  Air_Power; 
	uint16  Flame_State;
	uint16  Pump_State;
	
	uint16  Water_State;
	uint16  Paiwu_State;
	uint16  Air_State;
	uint16  lianxuFa_State;
	uint16  Air_Speed;   //20���ֽ�

	float   Steam_Pressure;
	float   Inside_High_Pressure;
	float   Pressure3;
	float   Steam_WenDu;
	float   Smoke_WenDu;
	float   LuNei_WenDu;//44���ֽ�

	uint16  Year;
	uint16  Month;
	uint16  Day;
	uint16  Hour;
	uint16  Min;  //LCD �� �� �� ʱ ��   //54���ֽ�

	uint16  First_Blow_Time;
	uint16  Last_Blow_Time;
	uint16  Dian_Huo_Power;
	uint16  Max_Work_Power;
	uint16  Danger_Smoke_Value;

	uint16  Fan_Speed_Check;
	uint16  Fan_Fire_Value;
	uint16  Paiwu_Flag; //���۱�־
	uint16  Water_BianPin_Enabled; //��Ƶ����
	uint16  Water_Max_Percent;  //���ڷ���󿪶�

	uint16  YuRe_Enabled;  //�����¶ȱ���ʹ��
	uint16  Inside_WenDu_ProtectValue; //�����¶ȱ���ֵ
	uint16  LianXu_PaiWu_DelayTime;  //������������ʱ��
	uint16  LianXu_PaiWu_OpenSecs; //��������ÿ�ο���ʱ��
	uint16  ModBus_Address;

	uint16  Fan_Pulse_Rpm;  //ÿת���������
	uint16  Input_Data;  //����״̬
	uint16  ChaYa_Water_Enabled;
	uint16  ChaYa_WaterLow_Set;
	uint16  ChaYa_WaterMid_Set;

	uint16  ChaYa_WaterHigh_Set;
	uint16  Zhu_WaterHigh;
	uint16  Cong_WaterHigh;
	uint16  System_Lock;
	uint16  Balance_Big_Time; //��������Ӧʱ��


	uint16  Balance_Small_Time;  //С������Ӧʱ��
	uint16  Device_State; //���ʱ���豸״̬
	uint16  Work_Time; //��������ʱ��
	uint16  YunXu_Flag;
	uint16  Device_Style; //�豸���0���浥��������1���������
	uint16  System_Version;

	float   Target_Value;  
	float   Stop_Value;  
	float   Start_Value;  
	float   Max_Pressure; //�ѹ��  
	float   buffer_float1;  
	

}LCD_GG;
typedef union  _LCD10_MsESS
{  //
	uint8 Datas[136]; //����Ҫ�ܹ���4��������������
	
	LCD_GG DLCD;
	
}LCD10Struct;


typedef struct _LCDJZ_UNIS
{
	//���ڸ��ӻ���������ʾ
	
	uint16  Device_State;  //�豸״̬
	uint16  Error_Code;
	uint16  Air_Power; 
	uint16  Flame_State;
	uint16  Pump_State;   //10���ֽ�
	
	uint16  Water_State;
	uint16  Paiwu_State;
	uint16  Air_State;
	uint16  lianxuFa_State;
	uint16  Air_Speed;   //20���ֽ�

	
	uint16  First_Blow_Time;
	uint16  Last_Blow_Time;
	uint16  Dian_Huo_Power;
	uint16  Max_Work_Power;
	uint16  Danger_Smoke_Value;  //30���ֽ�

	uint16  YuRe_Enabled;  //�����¶ȱ���ʹ��
	uint16  Inside_WenDu_ProtectValue; //�����¶ȱ���ֵ   
	uint16  YunXu_Flag;  //��̨�豸ʱ      �豸ʹ�ܱ�־
	uint16  Work_Time;  //�ۼ�����ʱ��
	uint16  InPut_Data; //���16λIO������  
	
	uint16  Data4;  
	uint16  Data5;
	uint16  Data6;
	uint16  Data7;               //48���ֽ�
/**********************************************************/


	float   Steam_Pressure;
	float   Inside_High_Pressure;
	float   Pressure3;
	float   Pressure4;
	float   Smoke_WenDu;
	float   LuNei_WenDu;//72���ֽ�

	
	
	
	

	
	

}LCD_JZ;
typedef union  _LCD10JZ_MsESS
{  //
	uint8 Datas[72]; //����Ҫ�ܹ���4��������������
	
	LCD_JZ DLCD;
	
}LCD10_JZ_Struct;

typedef struct _LCD_WRUNIS
{
	


	uint16  Year;
	uint16  Month;
	uint16  Day;
	uint16  Hour;
	uint16  Min;  //LCD �� �� �� ʱ ��   //54���ֽ�

	uint16  Data1;
	uint16  Data2;
	uint16  Data3;
	uint16  Data4;
	uint16  Data5;
	
	



	
	
}LCD_WR ;  //ֻд





extern UtoSlave_Info UtoSlave;

extern Duble5_Info Double5;

extern UNION_FLAGS Union_Flag;

extern LCD10Struct LCD10D;
extern LCD10_JZ_Struct LCD10JZ[13];  //���ڻ����������ʾ

extern LCD_WR  LCD10WR;

//////////////////////////////////////////////////////////////////////////////////////////////
void uart2_init(u32 bound);	
///////////////////////////////////////USART2 DMA�������ò���//////////////////////////////////	   		    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//�������
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ�����
//�������
//��
/////////////////////////////////////////////////////////////////////////////////////////////// 
void UART_DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
//////////////////////////////////////////����һ��DMA����//////////////////////////////////////
//���ܣ�����һ��DMA����
//�������
//DMA_CHx:DMAͨ��CHx
//cndtr:���ݴ�����
//�������
//��
////////////////////////////////////////////////////////////////////////////////////////////////


void u2_printf(char* fmt,...)  ;

void U2_send_byte(u8 data);
void U2_send_str(u8 *str,u8 s);








void ModBus2LCD_Communication(void);


uint8 ModuBus2LCD_Write0x10Response(uint16 address,uint16 Data16);

uint8 ModuBus2LCD_WriteAdress0x0000Response(uint16 Buffer_Data16);






#endif













