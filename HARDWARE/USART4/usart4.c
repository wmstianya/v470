#include "usart4.h"
#include "stdarg.h"

//���ڷ��ͻ����� 	
uint8 USART4_TX_BUF[256]; 	             //���ͻ���,���USART3_MAX_SEND_LEN�ֽ�	 1024 






PROTOCOL_COMMAND lcd_command;

//RECE_LDATA	Rece_Lcd_Data;

uint8_t CMD_RXFRMOK = 0;

UNION_GG  UnionD;


SLAVE_GG  SlaveG[13];

USlave_Struct JiZu[12];



uint8 cmd_get_time[] ={0x5A, 0xA5, 0x03,0x81,0x20,0x10};//�����ڿ���ʱ�������LCD��ͬ��ʱ����Ϣ

uint8 cmd_get_set_time[] ={0x5A,0XA5,0X04,0X83,0X00,0XB0,0X0B};//���ڶ�ȡ�û�����ʱ�����
///////////////////////////////////////USART4 printf֧�ֲ���//////////////////////////////////
//����2,u2_printf ����
//ȷ��һ�η������ݲ�����USART4_MAX_SEND_LEN�ֽ�
////////////////////////////////////////////////////////////////////////////////////////////////
void u4_printf(char* fmt,...)  
{  
  int len=0;
	int cnt=0;
//	unsigned  char i;
//	unsigned  char Frame_Info[5]; //ָ���
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART4_TX_BUF,fmt,ap);
	va_end(ap);
	len = strlen((const char*)USART4_TX_BUF);
	while(len--)
	  {
	  while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=1); //�ȴ����ͽ���
	  USART_SendData(UART4,USART4_TX_BUF[cnt++]);
	  }
}






//����4����s���ַ�

///////////////////////////////////////USART2 ��ʼ�����ò���//////////////////////////////////	    
//���ܣ���ʼ��IO ����2
//�������
//bound:������
//�������
//��
//////////////////////////////////////////////////////////////////////////////////////////////	  
void uart4_init(u32 bound)
{  	 		 
	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��USART4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��GPIOCʱ��
	USART_DeInit(UART4);  //��λ����4

     //USART4_TX   PC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //USART4_RX	  PC.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);  

    //Usart4 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	        //����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
    //USART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;   //һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;  //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	  //�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure);   //��ʼ������
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);  //�����ж�
    USART_Cmd(UART4, ENABLE);                      //ʹ�ܴ��� 
	
//	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);    //ʹ�ܴ���2��DMA����
//	UART_DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF,1000);  //DMA1ͨ��7,����Ϊ����2,�洢��ΪUSART2_TX_BUF,����1000. 										  	
}



void ModBus_Uart4_Local_Communication(void)
{
		
		uint8  i = 0;	
		
		uint16 checksum = 0;

		if(Sys_Admin.Device_Style == 2 || Sys_Admin.Device_Style == 3) //�����ͻ��ƴ���ͣ�������ʧ���������
			{
				if(sys_flag.Address_Number > 1)
					{
						if(sys_flag.UnTalk_Time > 15) //15��
							{
								if(sys_data.Data_10H == 2)
									{
										sys_close_cmd();
										
									}
								if(sys_data.Data_10H == 3)
									{
										 sys_data.Data_10H = 0;
										 
										
									}
								
								GetOut_Mannual_Function();
								
							}
					}
			}

	
		if(U4_Inf.Recive_Ok_Flag)
			{
				U4_Inf.Recive_Ok_Flag = 0;//������Ŷ
				 //�ر��ж�
				USART_ITConfig(UART4, USART_IT_RXNE, DISABLE); 
				 
				checksum  = U4_Inf.RX_Data[U4_Inf.RX_Length - 2] * 256 + U4_Inf.RX_Data[U4_Inf.RX_Length - 1];
				
			//�ϰ�������豸���кŽ����޸�
				if( checksum == ModBusCRC16(U4_Inf.RX_Data,U4_Inf.RX_Length))
					{
						//u4_printf("\n*��ַ����= %d\n",Sys_Admin.ModBus_Address);
						
						if(sys_flag.Address_Number == 0)
							{
								//������Ļ�趨��ַ
								if(U4_Inf.RX_Data[0] ==Sys_Admin.ModBus_Address)
									UART4_Server_Cmd_Analyse();
							}
						else
							{
								//�������岦���ַ
								if(U4_Inf.RX_Data[0] == sys_flag.Address_Number)
									{
										sys_flag.UnTalk_Time = 0;
										UART4_Server_Cmd_Analyse();
									}
									
							}
						
						
					
					}
					
			 
				
			//�Խ��ջ���������
				for( i = 0; i < 100;i++ )
					U4_Inf.RX_Data[i] = 0x00;
			
			//���¿����ж�
				USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); 
				
			}
}



void UART4_Server_Cmd_Analyse(void)
 {
 	uint8 Cmd = 0;
	uint8 index = 0;
	uint8 Length = 0;
	uint16 Value = 0;
	uint16 Address = 0;
	uint16 Data = 0; 
	uint8  Device_ID = Sys_Admin.ModBus_Address; //��ݵ�ַ�����ܴ� ************88
	float  value_buffer = 0;
	uint16 check_sum = 0;
	uint16 Data1 = 0;
	uint16 Data2 = 0;
	uint16 Data3 = 0;
	uint16 Data_Address = 0;
	uint16 Buffer_Data16 = 0;
	uint16 Data_Length = 0;

	uint8    Save_Flag1 = 0;
	uint8    Save_Flag2 = 0;
	


	 //Float_Int.value  ���ڵ����ȵ�ת��
	if(sys_flag.Address_Number == 0)
		Device_ID = Sys_Admin.ModBus_Address;
	else
		Device_ID = sys_flag.Address_Number;
	
	Cmd = U4_Inf.RX_Data[1];
	Address = U4_Inf.RX_Data[2] *256+ U4_Inf.RX_Data[3];
	Length = U4_Inf.RX_Data[4] *256+ U4_Inf.RX_Data[5];
	Value = U4_Inf.RX_Data[4] *256+ U4_Inf.RX_Data[5]; //06��ʽ�£� 
	
	
	switch (Cmd)
		{
			case 03:  //00 03 01 F3 00 0F F5 D0		

					switch (Address)
						{
							case 100:
									if( Length == 18)  //��ߵ����ݳ���Ҳ�����ܱ仯�������ֽڳ��ȵı仯
										{
											
											U4_Inf.TX_Data[0] = Device_ID;
											U4_Inf.TX_Data[1] = 0x03;// 
											U4_Inf.TX_Data[2] = 36; //���ݳ���Ϊ6�� ��������ı�***
									
											U4_Inf.TX_Data[3] = 0x00;
											U4_Inf.TX_Data[4] = sys_data.Data_10H;// 1��ǰ������״̬

											U4_Inf.TX_Data[5] = Temperature_Data.Pressure_Value >> 8 ;
											U4_Inf.TX_Data[6] = Temperature_Data.Pressure_Value & 0x00FF;//2��ǰ������ѹ��ֵ��ǰ��ˮλ״̬
											
											U4_Inf.TX_Data[7] = 0x00;
											U4_Inf.TX_Data[8] = Temperature_Data.steam_value;//3��ǰ�������¶�

											U4_Inf.TX_Data[9] = 0x00;
											U4_Inf.TX_Data[10] = LCD10D.DLCD.Water_State;//4��ǰˮλ��״̬

											Buffer_Data16 = Temperature_Data.Smoke_Tem /10;
											U4_Inf.TX_Data[11] = Buffer_Data16 >> 8;
											U4_Inf.TX_Data[12] = Buffer_Data16 & 0x00FF;//5�������¶�

											U4_Inf.TX_Data[13] = sys_flag.Protect_WenDu >> 8;
											U4_Inf.TX_Data[14] = sys_flag.Protect_WenDu & 0x00FF;//6�ڲ��������¶�

											U4_Inf.TX_Data[15] = 0;
											U4_Inf.TX_Data[16] = sys_flag.Error_Code;//7���ϴ���

											U4_Inf.TX_Data[17] = 0;
											U4_Inf.TX_Data[18] = sys_flag.flame_state;//8����״̬ 

											U4_Inf.TX_Data[19] = sys_flag.Fan_Rpm >> 8;
											U4_Inf.TX_Data[20] = sys_flag.Fan_Rpm & 0x00FF;//9���ת��

											U4_Inf.TX_Data[21] = Temperature_Data.Inside_High_Pressure >> 8;
											U4_Inf.TX_Data[22] = Temperature_Data.Inside_High_Pressure & 0x00FF;      //10 �ڲ�ѹ��

 											U4_Inf.TX_Data[23] = 0;
 											U4_Inf.TX_Data[24] = LCD10D.DLCD.Air_Power;//11������� 

											U4_Inf.TX_Data[25] = 0;
											U4_Inf.TX_Data[26] = Switch_Inf.air_on_flag; //12�������״̬


											U4_Inf.TX_Data[27] = 0;
											U4_Inf.TX_Data[28] = Switch_Inf.Water_Valve_Flag;//13��ˮ��

											U4_Inf.TX_Data[29] = 0;
											U4_Inf.TX_Data[30] = Switch_Inf.pai_wu_flag;//14���۷�״̬

											U4_Inf.TX_Data[31] = 0;
											U4_Inf.TX_Data[32] = Switch_Inf.LianXu_PaiWu_flag;//15�������۷�״̬
											
											
											U4_Inf.TX_Data[33] = 0;
											U4_Inf.TX_Data[34] = Water_State.ZSignal;//16  ��ˮ������
											
											U4_Inf.TX_Data[35] = sys_flag.ChaYaWater_Value >> 8;
											U4_Inf.TX_Data[36] = sys_flag.ChaYaWater_Value & 0x00FF;//17  ��ѹҺλ��ʵ��ֵ

											U4_Inf.TX_Data[37] = sys_flag.Inputs_State >> 8;
											U4_Inf.TX_Data[38] = sys_flag.Inputs_State & 0x00FF;//18 

											
											
										
										 
											check_sum  = ModBusCRC16(U4_Inf.TX_Data,41);   //����������ֽ����ı�
											U4_Inf.TX_Data[39]  = check_sum >> 8 ;
											U4_Inf.TX_Data[40]  = check_sum & 0x00FF;
											
											Usart_SendStr_length(UART4,U4_Inf.TX_Data,41);
											
										}

									break;
							case 120:  //��ȡ����ѹ��������
									value_buffer  = Temperature_Data.Pressure_Value;
									Float_Int.value = value_buffer / 100;
									if(Address == 120 && Length == 2)
										{
											U4_Inf.TX_Data[0] = Device_ID;
											U4_Inf.TX_Data[1] = 0x03;// 
											U4_Inf.TX_Data[2] = 4; //���ݳ���Ϊ �� ��������ı�***

											U4_Inf.TX_Data[3] = Float_Int.byte4.data_HH;
											U4_Inf.TX_Data[4] = Float_Int.byte4.data_HL;// 1��ǰ������״̬

											U4_Inf.TX_Data[5] = Float_Int.byte4.data_LH;
											U4_Inf.TX_Data[6] = Float_Int.byte4.data_LL;

											check_sum  = ModBusCRC16(U4_Inf.TX_Data,9);   //����������ֽ����ı�
											U4_Inf.TX_Data[7]  = check_sum >> 8 ;
											U4_Inf.TX_Data[8]  = check_sum & 0x00FF;
											
											Usart_SendStr_length(UART4,U4_Inf.TX_Data,9);
											
										}

									break;
							case 150:  //����ѹ����������
									
										if( Length == 3)  
											{
												U4_Inf.TX_Data[0] = Device_ID;
												U4_Inf.TX_Data[1] = 0x03;// 
												U4_Inf.TX_Data[2] = 6; //���ݳ���Ϊ �� ��������ı�***

												U4_Inf.TX_Data[3] = sys_config_data.zhuan_huan_temperture_value >> 8;
												U4_Inf.TX_Data[4] = sys_config_data.zhuan_huan_temperture_value & 0x00FF;// 1Ŀ������ѹ��

												U4_Inf.TX_Data[5] = sys_config_data.Auto_stop_pressure >> 8;
												U4_Inf.TX_Data[6] = sys_config_data.Auto_stop_pressure & 0x00FF;

												U4_Inf.TX_Data[7] = sys_config_data.Auto_start_pressure >> 8;
												U4_Inf.TX_Data[8] = sys_config_data.Auto_start_pressure & 0x00FF;

												check_sum  = ModBusCRC16(U4_Inf.TX_Data,11);   //����������ֽ����ı�
												U4_Inf.TX_Data[9]  = check_sum >> 8 ;
												U4_Inf.TX_Data[10]  = check_sum & 0x00FF;
												
												Usart_SendStr_length(UART4,U4_Inf.TX_Data,11);
											}
										if(Length == 18)
										{
											U4_Inf.TX_Data[0] = Device_ID;
											U4_Inf.TX_Data[1] = 0x03;// 
											U4_Inf.TX_Data[2] = 36; //���ݳ���Ϊ6�� ��������ı�***
									
											U4_Inf.TX_Data[3] = sys_config_data.zhuan_huan_temperture_value >> 8;
											U4_Inf.TX_Data[4] = sys_config_data.zhuan_huan_temperture_value & 0x00FF;// 1Ŀ������ѹ��

											U4_Inf.TX_Data[5] = sys_config_data.Auto_stop_pressure >> 8;
											U4_Inf.TX_Data[6] = sys_config_data.Auto_stop_pressure & 0x00FF;  //2 ֹͣѹ��

											U4_Inf.TX_Data[7] = sys_config_data.Auto_start_pressure >> 8;
											U4_Inf.TX_Data[8] = sys_config_data.Auto_start_pressure & 0x00FF; //3 ����ѹ��

											U4_Inf.TX_Data[9] = 0x00;
											U4_Inf.TX_Data[10] = Sys_Admin.Dian_Huo_Power;//4 ����� ������һ��

											U4_Inf.TX_Data[11] = 0x00;
											U4_Inf.TX_Data[12] = Sys_Admin.Max_Work_Power;//5 ������й��� ������һ��

											U4_Inf.TX_Data[13] = Sys_Admin.Inside_WenDu_ProtectValue>> 8;
											U4_Inf.TX_Data[14] = Sys_Admin.Inside_WenDu_ProtectValue & 0x00FF;//6���±���ֵ��������һ��

											U4_Inf.TX_Data[15] =  Sys_Admin.Danger_Smoke_Value >> 8;
											U4_Inf.TX_Data[16] =  Sys_Admin.Danger_Smoke_Value & 0x00FF;//7�����¶ȱ���ֵ ������һ��

											U4_Inf.TX_Data[17] = 0;
											U4_Inf.TX_Data[18] = Sys_Admin.Water_Max_Percent;//8 ��Ƶ��ˮ��󿪶ȣ����л���һ��

											U4_Inf.TX_Data[19] = Sys_Admin.LianXu_PaiWu_DelayTime;  //�����������ڷ���
											U4_Inf.TX_Data[20] = Sys_Admin.LianXu_PaiWu_OpenSecs;//�������ۿ������룬������л���һ��

											U4_Inf.TX_Data[21] = 0 ;  // 
											U4_Inf.TX_Data[22] =0 ;// 
											
 											U4_Inf.TX_Data[23] = 0;
 											U4_Inf.TX_Data[24] = 0;//11  

											U4_Inf.TX_Data[25] = 0;
											U4_Inf.TX_Data[26] = 0; //12 


											U4_Inf.TX_Data[27] = 0;
											U4_Inf.TX_Data[28] = 0;//13 

											U4_Inf.TX_Data[29] = 0;
											U4_Inf.TX_Data[30] = Sys_Admin.Fan_Speed_Check;//14 

											U4_Inf.TX_Data[31] = Sys_Admin.Water_BianPin_Enabled;
											U4_Inf.TX_Data[32] = Sys_Admin.YuRe_Enabled;//15  ���ֿ��صı�־λ�������¶ȱ�������Ƶ��ˮ���������ټ��
											
											
											U4_Inf.TX_Data[33] = 0;
											U4_Inf.TX_Data[34] = Sys_Admin.DeviceMaxPressureSet;//16  �����ѹ��
											
											U4_Inf.TX_Data[35] = 0;
											U4_Inf.TX_Data[36] = Sys_Admin.Device_Style ;//17  �豸����

											U4_Inf.TX_Data[37] = 0;
											U4_Inf.TX_Data[38] = sys_config_data.Sys_Lock_Set;//18   �豸����
										
										 
											check_sum  = ModBusCRC16(U4_Inf.TX_Data,41);   //����������ֽ����ı�
											U4_Inf.TX_Data[39]  = check_sum >> 8 ;
											U4_Inf.TX_Data[40]  = check_sum & 0x00FF;
											
											Usart_SendStr_length(UART4,U4_Inf.TX_Data,41);
										}


										
									break;
							
								

							default :
									break;
						}
					

					//���ѹ����������ת��
					
					break;
			case 06://�����Ĵ�����д��
					
					switch (Address)
						{
							
							case 0x0064://�豸������رգ�����ֵ������
										 
										if(sys_data.Data_10H  == 0  && Value == 1)//����״̬ ִ��������������й���Ŷ
											{
												 
												sys_start_cmd();

												//ModuBus4RTU_WriteResponse(Address,Value);
												
											}

										if(sys_data.Data_10H  == 0  && Value == 2)//����״̬ ִ��������������й���Ŷ
											{
												sys_start_cmd();

												//ModuBus4RTU_WriteResponse(Address,Value);
											}

										if(sys_data.Data_10H  == 0  && Value == 3)//����״̬ Ҫ�����ֶ�ģʽ
											{
												 
												sys_data.Data_10H  = 3;
												GetOut_Mannual_Function();

												//ModuBus4RTU_WriteResponse(Address,Value);
											}

										if(sys_data.Data_10H  == 3  && Value == 0)//�ֶ�״̬ Ҫ�������ģʽ
											{
												 
												sys_data.Data_10H  = 0;
												GetOut_Mannual_Function();

												//ModuBus4RTU_WriteResponse(Address,Value);
											}


										if(sys_data.Data_10H == 2  && Value == 0)//����״̬ ִ�йرմ������ 
											{
												sys_close_cmd();
												//ModuBus4RTU_WriteResponse(Address,Value);
												
											}
										ModuBus4RTU_WriteResponse(Address,Value);
												
										
										break;

							case 0x006B:  //���ϸ�λ����
										
										ModuBus4RTU_WriteResponse(Address,0);
										
										sys_flag.Error_Code = 0;
										Beep_Data.beep_start_flag = 0;//������־
										sys_flag.Lock_Error = 0; //�����������

											
										break;
							case 0x6F:  //�ֶ�ģʽ�¹��ʵĿ�����ر�
										if(Value <= 100)
											{
												PWM_Adjust(Value);
											}
										 ModuBus4RTU_WriteResponse(Address,Value);

										break;

							case 0x70:  //�ֶ�ģʽ�·���Ŀ�����ر�
										if(Value == 0)
											{
												Send_Air_Close();//�رշ��
											}

										if(Value == 1)
											{
												Send_Air_Open();//�������
												PWM_Adjust(100);
											}
										ModuBus4RTU_WriteResponse(Address,Value);

										break;
							case 0x71:  //�ֶ�ģʽ��ˮ�õĿ�����رգ�����Ǳ�Ƶģʽ�����ǿ���
										//sys_data.Data_10H = 3;
										//˫ƴģʽ�£��ӻ���ˮ��ŷ��Ŀ���Ȩ�����������ϣ������ֶ����Զ�
										if(Value == 0)
											{
												Feed_Main_Pump_OFF();
												sys_flag.WaterClose_Time = 2;  //����˳�����Ⱥ��ֶ�ģʽ��ǿ�ƿ�����ر�
												Second_Water_Valve_Close();
												sys_flag.Water_Percent = 0;
											}

										if(Value == 1)
											{
												sys_flag.WaterOut_Time = 3;
												Feed_Main_Pump_ON();
												Second_Water_Valve_Open();
												sys_flag.Water_Percent = 50;
												 
											}
										ModuBus4RTU_WriteResponse(Address,Value);

										break;
							case 0x72:  //�ֶ�ģʽ�����۷��Ŀ�����رգ�
										sys_data.Data_10H = 3;
										if(Value == 0)
											{
												Pai_Wu_Door_Close();
											}

										if(Value == 1)
											{
												Pai_Wu_Door_Open();
												 
											}
										ModuBus4RTU_WriteResponse(Address,Value);

										break;
							case 0x73:  //�ֶ�ģʽ���������۷��Ŀ�����رգ�
										sys_data.Data_10H = 3;
										if(Value == 0)
											{
												LianXu_Paiwu_Close();
											}

										if(Value == 1)
											{
												LianXu_Paiwu_Open();
												 
											}
										ModuBus4RTU_WriteResponse(Address,Value);

										break;
							case 0x75:  //�Զ�������һ��
										
										if(Value == 1)
											{
												sys_flag.Paiwu_Flag = OK;
											}
										ModuBus4RTU_WriteResponse(Address,Value);

										break;
							

							case 150:  //Ŀ������ѹ������
										
										if(Value < sys_config_data.Auto_stop_pressure)
											{
												if(Value >= 20)
													{
														sys_config_data.zhuan_huan_temperture_value = Value;
														Write_Internal_Flash();//����ز������б���
														ModuBus4RTU_WriteResponse(Address,Value);
														
														
													}
											}
										
										break;
							case 151:  //ͣ������ѹ������
										
												if(Value < Sys_Admin.DeviceMaxPressureSet)
													{
														if(Value > sys_config_data.zhuan_huan_temperture_value)
															{
																sys_config_data.Auto_stop_pressure = Value;
																Write_Internal_Flash();//����ز������б���
																ModuBus4RTU_WriteResponse(Address,Value);
																
															}
													}
										
										break;
							case 152: //��������ѹ��
										
												if(Value < sys_config_data.Auto_stop_pressure)
													{
														sys_config_data.Auto_start_pressure = Value;
														Write_Internal_Flash();//����ز������б���

														ModuBus4RTU_WriteResponse(Address,Value);
	
													}
											

										break;
							

							default:
									break;
						}
					
					break;
			case 0x10:   //����Ĵ���д��
						Data_Address = U4_Inf.RX_Data[2] * 256 + U4_Inf.RX_Data[3];
						Data_Length = U4_Inf.RX_Data[4] * 256 + U4_Inf.RX_Data[5];
						Buffer_Data16 = U4_Inf.RX_Data[7] *256 + U4_Inf.RX_Data[8];  //�ߵ��ֽڵ�˳��ߵ�
						switch (Data_Address)
							{
							case 150:   //������������д������
										
										//�Աȸ������ݣ��洢�Ĵ洢
										if(sys_config_data.zhuan_huan_temperture_value !=  Buffer_Data16)
											{
												sys_config_data.zhuan_huan_temperture_value = Buffer_Data16;
												Save_Flag1 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[9] *256 + U4_Inf.RX_Data[10]; 
										if(sys_config_data.Auto_stop_pressure !=  Buffer_Data16)
											{
												sys_config_data.Auto_stop_pressure = Buffer_Data16;
												Save_Flag1 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[11] *256 + U4_Inf.RX_Data[12]; 
										if(sys_config_data.Auto_start_pressure !=  Buffer_Data16)
											{
												sys_config_data.Auto_start_pressure = Buffer_Data16;
												Save_Flag1 = OK;
											}

										Buffer_Data16 = U4_Inf.RX_Data[13] *256 + U4_Inf.RX_Data[14]; 
										if(Sys_Admin.Dian_Huo_Power !=  Buffer_Data16)
											{
												Sys_Admin.Dian_Huo_Power = Buffer_Data16;
												Save_Flag2 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[15] *256 + U4_Inf.RX_Data[16]; 
										if(Sys_Admin.Max_Work_Power !=  Buffer_Data16)
											{
												Sys_Admin.Max_Work_Power = Buffer_Data16;
												Save_Flag2 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[17] *256 + U4_Inf.RX_Data[18]; 
										if(Sys_Admin.Inside_WenDu_ProtectValue !=  Buffer_Data16)
											{
												Sys_Admin.Inside_WenDu_ProtectValue = Buffer_Data16;
												Save_Flag2 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[19] *256 + U4_Inf.RX_Data[20]; 
										Buffer_Data16 = Buffer_Data16 * 10;
										if(Sys_Admin.Danger_Smoke_Value !=  Buffer_Data16)
											{
												Sys_Admin.Danger_Smoke_Value = Buffer_Data16 ;
												Save_Flag2 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[21] *256 + U4_Inf.RX_Data[22]; 
										if(Sys_Admin.Water_Max_Percent !=  Buffer_Data16)
											{
												Sys_Admin.Water_Max_Percent = Buffer_Data16;
												Save_Flag2 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[23] ; 
										if(Sys_Admin.LianXu_PaiWu_DelayTime !=  Buffer_Data16)
											{
												Sys_Admin.LianXu_PaiWu_DelayTime = Buffer_Data16;
												Save_Flag2 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[24] ; 
										if(Sys_Admin.LianXu_PaiWu_OpenSecs !=  Buffer_Data16)
											{
												Sys_Admin.LianXu_PaiWu_OpenSecs = Buffer_Data16;
												Save_Flag2 = OK;
											}

										Buffer_Data16 = U4_Inf.RX_Data[26] ; 
										if(Sys_Admin.ChaYa_WaterHigh_Enabled != Buffer_Data16)
											{
												Sys_Admin.ChaYa_WaterHigh_Enabled = Buffer_Data16;
												Save_Flag2 = OK;
											}

										Buffer_Data16 = U4_Inf.RX_Data[27] *256 + U4_Inf.RX_Data[28]; 
										if(Sys_Admin.ChaYa_WaterLow_Set != Buffer_Data16)
											{
												Sys_Admin.ChaYa_WaterLow_Set = Buffer_Data16;
												Save_Flag2 = OK;
											}

										Buffer_Data16 = U4_Inf.RX_Data[29] *256 + U4_Inf.RX_Data[30]; 
										if(Sys_Admin.ChaYa_WaterMid_Set != Buffer_Data16)
											{
												Sys_Admin.ChaYa_WaterMid_Set = Buffer_Data16;
												Save_Flag2 = OK;
											}

										Buffer_Data16 = U4_Inf.RX_Data[31] *256 + U4_Inf.RX_Data[32]; 
										if(Sys_Admin.ChaYa_WaterHigh_Set != Buffer_Data16)
											{
												Sys_Admin.ChaYa_WaterHigh_Set = Buffer_Data16;
												Save_Flag2 = OK;
											}

										 


										Buffer_Data16 = U4_Inf.RX_Data[33] *256 + U4_Inf.RX_Data[34]; 
										if(Sys_Admin.Fan_Speed_Check !=  Buffer_Data16)
											{
												Sys_Admin.Fan_Speed_Check = Buffer_Data16;
												Save_Flag2 = OK;
											}
										Buffer_Data16 = U4_Inf.RX_Data[35] *256 + U4_Inf.RX_Data[36]; 
										if(Sys_Admin.Water_BianPin_Enabled !=  Buffer_Data16)
											{
												Sys_Admin.Water_BianPin_Enabled = Buffer_Data16;
												Save_Flag2 = OK;
											}

										Buffer_Data16 = U4_Inf.RX_Data[37] *256 + U4_Inf.RX_Data[38]; 
										if(Sys_Admin.DeviceMaxPressureSet !=  Buffer_Data16)
											{
												Sys_Admin.DeviceMaxPressureSet = Buffer_Data16;
												Save_Flag2 = OK;
											}

										Buffer_Data16 = U4_Inf.RX_Data[39] *256 + U4_Inf.RX_Data[40]; 
										if(Sys_Admin.Device_Style !=  Buffer_Data16)
											{
												Sys_Admin.Device_Style = Buffer_Data16;
												Save_Flag2 = OK;
											}

										Buffer_Data16 = U4_Inf.RX_Data[41] *256 + U4_Inf.RX_Data[42]; 
										if(sys_config_data.Sys_Lock_Set !=  Buffer_Data16)
											{
												sys_config_data.Sys_Lock_Set = Buffer_Data16;
												Save_Flag1 = OK;
											}
										

										if(Save_Flag1)
											{
												Write_Internal_Flash();
												BEEP_TIME(50); 
											}
										if(Save_Flag2)
											{
												Write_Admin_Flash();
												BEEP_TIME(50); 
											}
										
										ModuBus4_Write0x10Response(Data_Address,Data_Length);
										break;
							default:

									break;
							}

						break;

			


			default:
					//��Чָ��
					break;
		}
 }


uint8 ModuBus4RTU_WriteResponse(uint16 address,uint16 Data16)
{
	uint16 check_sum = 0;
	
	U4_Inf.TX_Data[0] = U4_Inf.RX_Data[0];
	U4_Inf.TX_Data[1]= 0x06;

	
	U4_Inf.TX_Data[2] = address >> 8;    // ��ַ���ֽ�
	U4_Inf.TX_Data[3] = address & 0x00FF;  //��ַ���ֽ�
	

	U4_Inf.TX_Data[4] = Data16 >> 8;  //���ݸ��ֽ�
	U4_Inf.TX_Data[5] = Data16 & 0x00FF;   //���ݵ��ֽ�

	check_sum  = ModBusCRC16(U4_Inf.TX_Data,8);   //����������ֽ����ı�
	U4_Inf.TX_Data[6]  = check_sum >> 8 ;
	U4_Inf.TX_Data[7]  = check_sum & 0x00FF;

	Usart_SendStr_length(UART4,U4_Inf.TX_Data,8);

	return 0;
}







uint8 ModBus4_RTU_Read03(uint8 Target_Address,uint16 Data_Address,uint8 Data_Length )
{
		uint16 Check_Sum = 0;
		U4_Inf.TX_Data[0] = Target_Address;
		U4_Inf.TX_Data[1] = 0x03;

		U4_Inf.TX_Data[2]= Data_Address >> 8;
		U4_Inf.TX_Data[3]= Data_Address & 0x00FF;

		
		U4_Inf.TX_Data[4]= Data_Length >> 8;
		U4_Inf.TX_Data[5]= Data_Length & 0x00FF;

		
		Check_Sum = ModBusCRC16(U4_Inf.TX_Data,8);
		U4_Inf.TX_Data[6]= Check_Sum >> 8;
		U4_Inf.TX_Data[7]= Check_Sum & 0x00FF;
		
		Usart_SendStr_length(UART4,U4_Inf.TX_Data,8);


	return 0;
}

uint8 ModBus4_RTU_Write06(uint8 Target_Address,uint16 Data_Address,uint16 Data16)
{
		uint16 Check_Sum = 0;
		U4_Inf.TX_Data[0] = Target_Address;
		U4_Inf.TX_Data[1] = 0x06;

		U4_Inf.TX_Data[2]= Data_Address >> 8;
		U4_Inf.TX_Data[3]= Data_Address & 0x00FF;

		U4_Inf.TX_Data[4]= Data16 >> 8;
		U4_Inf.TX_Data[5]= Data16 & 0x00FF;

		Check_Sum = ModBusCRC16(U4_Inf.TX_Data,8);
		U4_Inf.TX_Data[6]= Check_Sum >> 8;
		U4_Inf.TX_Data[7]= Check_Sum & 0x00FF;

		Usart_SendStr_length(UART4,U4_Inf.TX_Data,8);

		

		return 0;
}

uint8 ModuBus4_Write0x10Response(uint16 address,uint16 Data16)
{
	uint16 check_sum = 0;
	
	U4_Inf.TX_Data[0] = 0x01;
	U4_Inf.TX_Data[1]= 0x10;

	
	U4_Inf.TX_Data[2] = address >> 8;    // ��ַ���ֽ�
	U4_Inf.TX_Data[3] = address & 0x00FF;  //��ַ���ֽ�
	

	U4_Inf.TX_Data[4] = Data16 >> 8;  //���ݸ��ֽ�
	U4_Inf.TX_Data[5] = Data16 & 0x00ff;   //���ݵ��ֽ�

	check_sum  = ModBusCRC16(U4_Inf.TX_Data,8);   //����������ֽ����ı�
	U4_Inf.TX_Data[6]  = check_sum >> 8 ;
	U4_Inf.TX_Data[7]  = check_sum & 0x00FF;

	Usart_SendStr_length(UART4,U4_Inf.TX_Data,8);

	return 0;
}





