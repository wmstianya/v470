#include "main.h"
#ifndef __USART4_H
#define __USART4_H	  	     
 
#define USART4_MAX_RECV_LEN		255				//�����ջ����ֽ���
#define USART4_MAX_SEND_LEN		255				//����ͻ����ֽ���

#define    Sys_Begin_Check_Page  110  //������������ͨ�ż��ҳ�棬����ͣ���ڸ�ҳ��
#define    Sys_Main_Page    1    //ϵͳ������
#define    Sys_Start_Confirm_Page  10 //ϵͳ����ǰȷ�Ͻ���
#define    Sys_Time_Set_Page  6  //ϵͳʱ���޸Ľ���
#define    Sys_Warning_Page  3  //ϵͳ������ʾ����
#define    Sys_Admin_SetPage  11  //����Ա����



#define    Sys_Manual_Page    5    //�ֶ����Խ���
#define    Sys_Air_Blow_Warning_Page    111    //ϵͳ��ɨ��ʾ����



#define  LAdmin_Main_Page 11 //����Ա����
#define  LAdmin_Sys_Set_Page 4 //ϵͳ���ý���  **
  




//����������ַ
#define  Movie_Start  1
#define  Movie_Close  0

#define   Air_Movie_Address  0x0210
#define   Pump_Movie_Address 0x0212
#define   Flame_Movie_Address 0x0214






#define FRM_H 0x5A
#define FRM_L 0xA5

typedef struct _CMD
{
	uint8_t header_h;
	uint8_t header_l;
	uint8_t data_len;
	uint8_t cmd;
	uint8_t data[32];
}PROTOCOL_COMMAND;


typedef struct _CMD_UNIS
{
	uint16  UnionStartFlag;
	uint16  Need_Numbers;
	uint16  Same_ValueFlag; //ͬ���������ñ�־
	uint16  Target_Value;
	uint16   Stop_Value;
	
	uint16  Start_Value;
	uint16  Big_Pressure;
	uint16  Data2;
	uint16  Data3;
	uint16  Data4;
	
	
}UNION_GG;


typedef struct _SLAVE_G
{
	uint8 Dev_State;
volatile	uint8 Alive_Flag;   //��������״̬�ı�־
volatile	uint8 Ok_Flag;  //�������޹��ϱ�־
volatile	uint16 Send_Flag;   //�������ͱ�־
volatile	uint8 UnBack_time;  //δ����ͨ��ʱ��

	uint8   ForceSend_Count;  //�������������Ĵ���

			uint8 Key_Power;  //ʹ��Ȩ��

volatile	uint8 UnSuccessTalk;

			uint8 Error_Code;
			uint8 Lock_Error;

			uint8 Send_Count;


			

			uint8 Startclose_Sendflag;
			uint8 Startclose_Data;  //˫˫���ʹ��

			uint8 ResetError_Flag;
			uint8 ResetError_Data;

			uint8 AirOpen_Flag;
			uint8 AirOpen_Data;

			uint8 PumpOpen_Flag;
			uint8 PumpOpen_Data;

			uint8 PaiWuOpen_Flag;
			uint8 PaiWuOpen_Data;    

			uint8 LianxuFa_Flag;
			uint8 LianxuFa_Data;

			uint8 AirPower_Flag;
			uint8 AirPower_Data;

			uint8 Paiwu_Flag;
			uint8 Paiwu_Data;
			
			uint8 ResetError_SendFlag;

			uint16 Big_time;
			uint16 Small_time;
			uint16 Zero_time;

			uint32 Work_Time;




			uint8 First_Blow;
			uint8 Last_Blow;
			uint8 Fire_Power;
			uint8 Max_Power;
			uint8 Smoke_Protect;//������������¶�

			uint16 Inside_WenDu_ProtectValue;
	
}SLAVE_GG;//�ӻ��Ŀ�������̨


typedef struct _CCMD_UNIS
{
	uint16  Flame; //ͬ���������ñ�־
	uint16  Error_Code;  //���ϴ���
	uint16  Power;
	uint16  UnionOn_Flag;	
	uint16  StartFlag;
	
	uint16  Error_Reset;		
	uint16  Device_State;
	uint16  Data1;
	uint16  Data2;
	uint16  Data3;

	float   Dpressure;

	
}Slave_GT; //��Ҫ�Ƿ���������ݵ�ͨ�ţ�����飬ģ�黯




typedef union  _SLAVE_MsESS
{  //
	uint8 Datas[24];
	
	Slave_GT Slave_D;
	
}USlave_Struct;



extern PROTOCOL_COMMAND lcd_command;

extern UNION_GG  UnionD;


extern SLAVE_GG  SlaveG[13];
extern USlave_Struct JiZu[12];





///////////////////////////////////////USART2 printf֧�ֲ���//////////////////////////////////
//����2,u2_printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
////////////////////////////////////////////////////////////////////////////////////////////////
void u4_printf(char* fmt, ...);
///////////////////////////////////////USART2 ��ʼ�����ò���//////////////////////////////////	    
//���ܣ���ʼ��IO ����2
//�������
//bound:������
//�������
//��
//////////////////////////////////////////////////////////////////////////////////////////////
void uart4_init(u32 bound);	

void ModBus_Uart4_Local_Communication(void);




void UART4_Server_Cmd_Analyse(void);


uint8 ModuBus4RTU_WriteResponse(uint16 address,uint16 Data16);

uint8 ModuBus4_Write0x10Response(uint16 address,uint16 Data16);




#endif

