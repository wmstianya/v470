/**
  ******************************************************************************
  * @file    usr_c210.c
  * @author  heic
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   usr_c210 WIFIģ�� �ӿڶ��弰Ӧ�ú���
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "main.h"

extern uint16 USART2_RX_STA;




 WWW_DATA Talk;






 ///////////////////////////////////////�������������////////////////////////////////// 	 
 //���ܣ��Է��������������ݽ��з���
 //�������
 //�������
 //��
 //////////////////////////////////////////////////////////////////////////////////////////////    
 void Server_Cmd_Analyse(void)
 {
 	uint8 Cmd = 0;
	uint8 index = 0;
	uint16 Address = 0;
	uint16 Data = 0;
	//uint16 check_sum = 0;
	
	
	Cmd = U1_Inf.RX_Data[1];
	Address = U1_Inf.RX_Data[2] *256+ U1_Inf.RX_Data[3];
	
	switch (Cmd)
		{
			case 03:  //00 03 01 F3 00 0F F5 D0		 
					if(Address == 0x01F3)
						{
							sys_flag.Access_Data_Flag = OK;
							Mcu_mail_Wifi();
						}
					break;
		
			case 06://�����Ĵ�����д��
					
					switch (Address)
						{
							case 0x0063:////��ַ40100 Ϊ������ ������ͣ�Ĵ���
										Data = U1_Inf.RX_Data[4] *256+ U1_Inf.RX_Data[5];
										if(Data == 0x0022)//��������
										{
											if(sys_data.Data_10H  == 2)//����״̬ ִ�йر�����
												sys_close_cmd();

										}

										if(Data == 0x0033)//��������
										{
											if(sys_data.Data_10H  == 0)//����״̬ ִ��������������й���Ŷ
												sys_start_cmd();
										}
										//����ȷ����
											for(index = 0; index < U1_Inf.RX_Length; index++ )
												U1_Inf.TX_Data[index] = U1_Inf.RX_Data[index];
											
											//Usart_SendStr_length(USART1,U1_Inf.TX_Data,U1_Inf.RX_Length);

											Mcu_mail_Wifi();
										break;

							case 0x0064://��ַ40101 ���ϸ�λ�Ĵ���
										
										
										sys_flag.Error_Code = 0;
										Beep_Data.beep_start_flag = 0;//������־
										sys_flag.Lock_Error = 0; //�����������

										for(index = 0; index < U1_Inf.RX_Length; index++ )
												U1_Inf.TX_Data[index] = U1_Inf.RX_Data[index];
											
										//Usart_SendStr_length(USART1,U1_Inf.TX_Data,U1_Inf.RX_Length);
										Mcu_mail_Wifi();
										break;

							case 0x00C7:
										Data = U1_Inf.RX_Data[4] *256+ U1_Inf.RX_Data[5];
										if(Data != Sys_Admin.Access_Data_Mode)
											{
												if(Data <= 1)
													{
														Sys_Admin.Access_Data_Mode = Data;
														Write_Admin_Flash();
														Mcu_mail_Wifi();
													}
												
											}
											
										break;

							case 0x012C:
										
										
										break;

							default:
									break;
						}
					
					break;

			case 16: //40110--40115����Ĵ�����д��
					if(Address == 0x006D)
					if(U1_Inf.RX_Data[5] == 0x05 &&U1_Inf.RX_Data[6] == 10)
						{
							//Data = U1_Inf.RX_Data[4] *256+ U1_Inf.RX_Data[5];//40110 δʹ��
							//Data = U1_Inf.RX_Data[10] *256+ U1_Inf.RX_Data[11];//40114 δʹ��

							sys_config_data.zhuan_huan_temperture_value = U1_Inf.RX_Data[9] * 256 +U1_Inf.RX_Data[10]  ; 
							sys_config_data.Auto_stop_pressure = U1_Inf.RX_Data[11] * 256 + U1_Inf.RX_Data[12]  ; 
							sys_config_data.Auto_start_pressure = U1_Inf.RX_Data[13] * 256 + U1_Inf.RX_Data[14]; 

							if(sys_config_data.zhuan_huan_temperture_value < 10)
								sys_config_data.zhuan_huan_temperture_value = 10;

							
							if(sys_config_data.zhuan_huan_temperture_value > Max_Pressure_Set)
									sys_config_data.zhuan_huan_temperture_value = Max_Pressure_Set;

							//2�� ��ͣ¯ѹ�����м�⣬Ҫ����Ŀ��ѹ������0.5kg,�����ܳ���8kg
								if(sys_config_data.Auto_stop_pressure <= sys_config_data.zhuan_huan_temperture_value)
									sys_config_data.Auto_stop_pressure = sys_config_data.zhuan_huan_temperture_value + 10;

								if(sys_config_data.Auto_stop_pressure > Stop_Pressure_Set)
									sys_config_data.Auto_stop_pressure = Stop_Pressure_Set;

							//3������¯ѹ�����б���
								if(sys_config_data.Auto_start_pressure >= sys_config_data.zhuan_huan_temperture_value)
									sys_config_data.Auto_start_pressure = 10;

								if(sys_config_data.Auto_start_pressure < 10)
									sys_config_data.Auto_start_pressure = 10;

								
								Write_Internal_Flash();//����ز������б���
								
								copy_to_lcd();

							   	Mcu_mail_Wifi();
						}
					break;


			default:
					//��Чָ��
					break;
		}
 }

 
 //MCU��ʱ��WIFI��������


 uint8  New_Server_Cmd_Analyse(void)
 {
 	uint8 Cmd = 0;
	uint8 index = 0;
	uint8 Length = 0;
	uint16 Value = 0;
	uint16 Address = 0;
	uint16 Data = 0; 
	uint8  Device_ID = 1; //��ݵ�ַ�����ܴ� ************88
	float  value_buffer = 0;
	uint16 check_sum = 0;
	uint16 Data1 = 0;
	uint16 Data2 = 0;
	uint16 Data3 = 0;
	uint32 Data32_Buffer = 0;
	uint32 Int_CharH = 0;
	uint32 Int_CHarL = 0;
	uint8 Code[6] = {0};
	


	 //Float_Int.value  ���ڵ����ȵ�ת��

	
	Cmd = U1_Inf.RX_Data[1];
	Address = U1_Inf.RX_Data[2] *256+ U1_Inf.RX_Data[3];
	Length = U1_Inf.RX_Data[4] *256+ U1_Inf.RX_Data[5];
	Value = U1_Inf.RX_Data[4] *256+ U1_Inf.RX_Data[5];   //06��ʽ�£�value = length

	if(U1_Inf.RX_Data[0] > 1)
		{
			//����ѯ��ַ����1ʱ���Զ��л�Ϊ�Է�ѯ�ʵĵ�ַ
			Device_ID = Sys_Admin.ModBus_Address;

			if(U1_Inf.RX_Data[0] != Sys_Admin.ModBus_Address)
				{
					return 0;   //�Ǳ�����ֱַ���˳�
				}
		}
	
	
	switch (Cmd)
		{
			case 03:  //00 03 01 F3 00 0F F5 D0		

					switch (Address)
						{
							case 100:
									if(Address == 100 && Length == 11)  //��ߵ����ݳ���Ҳ�����ܱ仯�������ֽڳ��ȵı仯
										{
											U1_Inf.TX_Data[0] = Device_ID;
											U1_Inf.TX_Data[1] = 0x03;// 
											U1_Inf.TX_Data[2] = 22; //���ݳ���Ϊ6�� ��������ı�***
									
											U1_Inf.TX_Data[3] = 0x00;
											U1_Inf.TX_Data[4] = sys_data.Data_10H;// 1��ǰ������״̬

											U1_Inf.TX_Data[5] = Temperature_Data.Pressure_Value >> 8 ;
											U1_Inf.TX_Data[6] = Temperature_Data.Pressure_Value & 0x00FF;//2��ǰ������ѹ��ֵ��ǰ��ˮλ״̬
											
											U1_Inf.TX_Data[7] = 0x00;
											U1_Inf.TX_Data[8] = Temperature_Data.steam_value;//3��ǰ�������¶�

											U1_Inf.TX_Data[9] = 0x00;
											U1_Inf.TX_Data[10] = LCD10D.DLCD.Water_State;//4��ǰˮλ��״̬

											U1_Inf.TX_Data[11] = 0x00;
											U1_Inf.TX_Data[12] = Temperature_Data.Smoke_Tem /10;//5�������¶�

											U1_Inf.TX_Data[13] = sys_flag.Protect_WenDu >> 8;
											U1_Inf.TX_Data[14] = sys_flag.Protect_WenDu & 0x00FF;//6�ڲ��������¶�

											U1_Inf.TX_Data[15] = 0;
											U1_Inf.TX_Data[16] = sys_flag.Error_Code;//7���ϴ���

											U1_Inf.TX_Data[17] = 0;
											U1_Inf.TX_Data[18] = Switch_Inf.air_on_flag;//8�������״̬

											U1_Inf.TX_Data[19] = 0;
											U1_Inf.TX_Data[20] = LCD10D.DLCD.Air_Power;//9�������

											U1_Inf.TX_Data[21] = 0;
											U1_Inf.TX_Data[22] = Switch_Inf.water_switch_flag;//10ˮ��״̬

											U1_Inf.TX_Data[23] = 0;
											U1_Inf.TX_Data[24] = sys_flag.flame_state;//11����״̬
											

											
										 
											check_sum  = ModBusCRC16(U1_Inf.TX_Data,27);   //����������ֽ����ı�
											U1_Inf.TX_Data[25]  = check_sum >> 8 ;
											U1_Inf.TX_Data[26]  = check_sum & 0x00FF;
											
											Usart_SendStr_length(USART1,U1_Inf.TX_Data,27);

											
											
										}

									break;
							case 120:  //��ȡ����ѹ��������
									value_buffer  = Temperature_Data.Pressure_Value;
									Float_Int.value = value_buffer / 100;
									if(Address == 120 && Length == 2)
										{
											U1_Inf.TX_Data[0] = Device_ID;
											U1_Inf.TX_Data[1] = 0x03;// 
											U1_Inf.TX_Data[2] = 4; //���ݳ���Ϊ �� ��������ı�***

											U1_Inf.TX_Data[3] = Float_Int.byte4.data_HH;
											U1_Inf.TX_Data[4] = Float_Int.byte4.data_HL;// 1��ǰ������״̬

											U1_Inf.TX_Data[5] = Float_Int.byte4.data_LH;
											U1_Inf.TX_Data[6] = Float_Int.byte4.data_LL;

											check_sum  = ModBusCRC16(U1_Inf.TX_Data,9);   //����������ֽ����ı�
											U1_Inf.TX_Data[7]  = check_sum >> 8 ;
											U1_Inf.TX_Data[8]  = check_sum & 0x00FF;
											
											Usart_SendStr_length(USART1,U1_Inf.TX_Data,9);
											
										}

									break;
							case 150:  //����ѹ����������
									
										if( Length == 3)  
											{
												U1_Inf.TX_Data[0] = Device_ID;
												U1_Inf.TX_Data[1] = 0x03;// 
												U1_Inf.TX_Data[2] = 6; //���ݳ���Ϊ �� ��������ı�***

												U1_Inf.TX_Data[3] = sys_config_data.zhuan_huan_temperture_value >> 8;
												U1_Inf.TX_Data[4] = sys_config_data.zhuan_huan_temperture_value & 0x00FF;// 1Ŀ������ѹ��

												U1_Inf.TX_Data[5] = sys_config_data.Auto_stop_pressure >> 8;
												U1_Inf.TX_Data[6] = sys_config_data.Auto_stop_pressure & 0x00FF;

												U1_Inf.TX_Data[7] = sys_config_data.Auto_start_pressure >> 8;
												U1_Inf.TX_Data[8] = sys_config_data.Auto_start_pressure & 0x00FF;

												check_sum  = ModBusCRC16(U1_Inf.TX_Data,11);   //����������ֽ����ı�
												U1_Inf.TX_Data[9]  = check_sum >> 8 ;
												U1_Inf.TX_Data[10]  = check_sum & 0x00FF;
												
												Usart_SendStr_length(USART1,U1_Inf.TX_Data,11);
											}
									break;

							case 160:
									if( Length == 8)  
											{
												U1_Inf.TX_Data[0] = Device_ID;
												U1_Inf.TX_Data[1] = 0x03;// 
												U1_Inf.TX_Data[2] = 16;
												
												U1_Inf.TX_Data[3] = 0;
												U1_Inf.TX_Data[4] = sys_flag.Wifi_Lock_System;

												U1_Inf.TX_Data[5] = sys_flag.wifi_Lock_Year >> 8;
												U1_Inf.TX_Data[6] = sys_flag.wifi_Lock_Year & 0x00FF;

												U1_Inf.TX_Data[7] = 0;
												U1_Inf.TX_Data[8] = sys_flag.wifi_Lock_Month;

												U1_Inf.TX_Data[9] = 0;
											//	sys_flag.Wifi_Lock_Day = 0;
												U1_Inf.TX_Data[10] = sys_flag.Wifi_Lock_Day;

												Data32_Buffer = Char_to_Int_6(Login_D.Login_KeyH[6],Login_D.Login_KeyL[6]);
												Data1 = Data32_Buffer >> 16;
												U1_Inf.TX_Data[11] = Data1 >> 8;
												U1_Inf.TX_Data[12] = Data1 & 0x00FF;
												Data1 =Data32_Buffer & 0x0000FFFF;
												U1_Inf.TX_Data[13] = Data1 >> 8;
												U1_Inf.TX_Data[14] = Data1 & 0x00FF;

												Data32_Buffer = Char_to_Int_6(Login_D.Login_KeyH[5],Login_D.Login_KeyL[5]);
												Data1 = Data32_Buffer >> 16;
												U1_Inf.TX_Data[15] = Data1 >> 8;
												U1_Inf.TX_Data[16] = Data1 & 0x00FF;
												Data1 =Data32_Buffer & 0x0000FFFF; 
												U1_Inf.TX_Data[17] = Data1 >> 8;
												U1_Inf.TX_Data[18] = Data1 & 0x00FF;

												check_sum  = ModBusCRC16(U1_Inf.TX_Data,21);   //����������ֽ����ı�
												U1_Inf.TX_Data[19]  = check_sum >> 8 ;
												U1_Inf.TX_Data[20]  = check_sum & 0x00FF;
												
												Usart_SendStr_length(USART1,U1_Inf.TX_Data,21);
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
										 
										if(sys_data.Data_10H  == 0  && Value == 2)//����״̬ ִ��������������й���Ŷ
											{
												sys_start_cmd();
												ModuBusRTU_WriteResponse(Address,Value);
												
												
											}


										if(sys_data.Data_10H == 2  && Value == 0)//����״̬ ִ��������������й���Ŷ
											{
												sys_close_cmd();
												
												ModuBusRTU_WriteResponse(Address,Value);
												
											}
												
										
										break;

							case 0x006A:  //���ϸ�λ����
										
										sys_flag.Error_Code = 0;
										Beep_Data.beep_start_flag = 0;//������־
										sys_flag.Lock_Error = 0; //�����������

										ModuBusRTU_WriteResponse(Address,Value);

										
											
										break;

							case 150:  //Ŀ������ѹ������
										
										if(Value < sys_config_data.Auto_stop_pressure)
											{
												if(Value >= 20)
													{
														sys_config_data.zhuan_huan_temperture_value = Value;
														Write_Internal_Flash();//����ز������б���
														ModuBusRTU_WriteResponse(Address,Value);
														
														
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
																ModuBusRTU_WriteResponse(Address,Value);
																
															}
													}

											
										
										break;
							case 152: //��������ѹ��
										
												if(Value < sys_config_data.Auto_stop_pressure)
													{
														sys_config_data.Auto_start_pressure = Value;
														Write_Internal_Flash();//����ز������б���
														ModuBusRTU_WriteResponse(Address,Value);
														
															
													}

										break;
							case 160:
										if( Value <= 1)//ִ���������������
											{
												sys_flag.Wifi_Lock_System = Value; 

												Write_Internal_Flash();

												ModuBusRTU_WriteResponse(Address,Value);
											}

										break;

								case 161:
										//ִ�����������
										if( Value >= 0)
											{
												sys_flag.wifi_Lock_Year = Value; 

												Write_Internal_Flash();

												ModuBusRTU_WriteResponse(Address,Value);

												//�ñ���������
												
											}

										break;
								case 162:
										//ִ���������·�
										if( Value >= 0 && Value <= 12)
											{
												sys_flag.wifi_Lock_Month = Value; 

												Write_Internal_Flash();
												ModuBusRTU_WriteResponse(Address,Value);

												
											}

										break;
								case 163:
										//ִ������������
										if( Value >= 0 && Value <= 30)
											{
												sys_flag.Wifi_Lock_Day = Value; 

												Write_Internal_Flash();

												ModuBusRTU_WriteResponse(Address,Value);

												
											}
										break;
								

							default:
									break;
						}
					
					break;

			case 0x10:
						Length = U1_Inf.RX_Data[4] * 256 + U1_Inf.RX_Data[5];
						Data1 = U1_Inf.RX_Data[7]*256 + U1_Inf.RX_Data[8];
						Data2 = U1_Inf.RX_Data[9]*256 + U1_Inf.RX_Data[10];
						//01  10  00 A4  00 02  04  00 0F 3D DD 18 EE д32λ����ָ���ʽ
						//��Ӧ�� 01 10 00 A4    00 02 crc
						switch (Address)
							{
								case 164: //һ������
										if(Length == 2)
											{
												Data32_Buffer = Data1 *65536 + Data2;
												if(Data32_Buffer >= 100000 && Data32_Buffer <=999999) //����6λ��
													{
														
														ModuBusRTU_Write0x10Response(Address,Length);
														
													}
												
												
												
											}

											

										break;
								case 166:// ��������
										if(Length == 2)
											{
												Data32_Buffer = Data1 *65536 + Data2;
												if(Data32_Buffer >= 100000 && Data32_Buffer <=999999) //����6λ��
													{
														
														
														ModuBusRTU_Write0x10Response(Address,Length);
													
													}
												
											}

										break;

								default:

										break;
							}


					break;

			


			default:
					//��Чָ��
					break;
		}


	return 0 ;
 }

 void Mcu_mail_Wifi(void)
 {
 	uint16 checksum = 0;

	
	
	//ͬ��С�����������������ѹ��
	sys_data.Data_1EH = sys_config_data.zhuan_huan_temperture_value; 
	memset(&Talk,0,sizeof(Talk));
	
	Talk.Mess.Address = 0x00;
	Talk.Mess.Cmd = 0x03;
	Talk.Mess.Length = sizeof(Talk) - 5;

	 
	Talk.Mess.Data_29 =0xFD ;
	Talk.Mess.Data_30 =0XDF  ;//1����ʽ����������ʶ����
   	//1��ϵͳ����״̬
	Talk.Mess.Data_01 = sys_data.Data_10H;
	//2�����ϴ���
	Talk.Mess.Data_02 = sys_flag.Error_Code;
	//3��������״̬1
	
	
	//4��������״̬2
	Talk.Mess.Data_04 = 0;
	//5���̵���״̬1
	Talk.Mess.Data_05 = sys_data.Data_l3H;
	//6���̵���״̬2
	Talk.Mess.Data_06 = 0;
	//7������δʹ��
	Talk.Mess.Data_07 = 0;
	//8������δʹ��
	Talk.Mess.Data_08 = 0;
	//9�������¶ȸ�λ
	Talk.Mess.Data_09 = Temperature_Data.Smoke_Tem >> 8;
	//10�������¶ȵ�λ
	Talk.Mess.Data_10 = Temperature_Data.Smoke_Tem & 0x00FF;
	//11����ǰ����ѹ����λ
	Talk.Mess.Data_11 = Temperature_Data.Pressure_Value >> 8;
	//12����ǰ����ѹ����λ
	Talk.Mess.Data_12 = Temperature_Data.Pressure_Value & 0x00FF;
	//13����ǰ�����¶ȸ�λ
	Talk.Mess.Data_13 = Temperature_Data.steam_value >> 8;
	//14����ǰ�����¶ȵ�λ
	Talk.Mess.Data_14 = Temperature_Data.steam_value & 0x00FF;
	//15����ǰ����Ĺ���
	Talk.Mess.Data_15 = LCD10D.DLCD.Air_Power;
	
	//20�����趨��Ŀ������ѹ��ֵ
	Talk.Mess.Data_20 = sys_config_data.zhuan_huan_temperture_value;
	//21�����趨��ͣ¯����ѹ��ֵ
	Talk.Mess.Data_21 = sys_config_data.Auto_stop_pressure;
	//22�����趨����¯����ѹ��ֵ
	Talk.Mess.Data_22 = sys_config_data.Auto_start_pressure;
	
	//27�����ڻ�ȡ����ģʽ��0�򱻶���1������
	Talk.Mess.Data_27 = Sys_Admin.Access_Data_Mode;
	//28��ϵͳԶ�̿���״̬
	
	
	checksum = ModBusCRC16(Talk.ALL,sizeof(Talk));
	Talk.Mess.Crc_H = checksum >> 8;
	Talk.Mess.Crc_L = checksum & 0x00FF;

	
//	Usart_SendStr_length(USART1,(uint8 *)&Talk,sizeof(Talk));
			
	
 
 
 }


uint8 ModuBusRTU_WriteResponse(uint16 address,uint16 Data16)
{
	uint16 check_sum = 0;
	uint8 Device_ID = 1;

		if(U1_Inf.RX_Data[0] > 1)
		{
			//����ѯ��ַ����1ʱ���Զ��л�Ϊ�Է�ѯ�ʵĵ�ַ
			Device_ID = Sys_Admin.ModBus_Address;
		}
	U1_Inf.TX_Data[0] = Device_ID;
	 
	U1_Inf.TX_Data[1]= 0x06;

	
	U1_Inf.TX_Data[2] = address >> 8;    // ��ַ���ֽ�
	U1_Inf.TX_Data[3] = address & 0x00FF;  //��ַ���ֽ�
	

	U1_Inf.TX_Data[4] = Data16 >> 8;  //���ݸ��ֽ�
	U1_Inf.TX_Data[5] = Data16 & 0x00FF;   //���ݵ��ֽ�

	check_sum  = ModBusCRC16(U1_Inf.TX_Data,8);   //����������ֽ����ı�
	U1_Inf.TX_Data[6]  = check_sum >> 8 ;
	U1_Inf.TX_Data[7]  = check_sum & 0x00FF;

	Usart_SendStr_length(USART1,U1_Inf.TX_Data,8);

	return 0;
}



uint8 ModuBusRTU_Write0x10Response(uint16 address,uint16 Data16)
{
	uint16 check_sum = 0;
	uint8 Device_ID = 1;

	if(U1_Inf.RX_Data[0] > 1)
		{
			//����ѯ��ַ����1ʱ���Զ��л�Ϊ�Է�ѯ�ʵĵ�ַ
			Device_ID = Sys_Admin.ModBus_Address;
		}

	U1_Inf.TX_Data[0] = Device_ID; 
	U1_Inf.TX_Data[1]= 0x10;
	
	U1_Inf.TX_Data[2] = address >> 8;    // ��ַ���ֽ�
	U1_Inf.TX_Data[3] = address & 0x00FF;  //��ַ���ֽ�

	U1_Inf.TX_Data[4] = Data16 >> 8;  // 
	U1_Inf.TX_Data[5] = Data16 & 0X00FF;    

	check_sum  = ModBusCRC16(U1_Inf.TX_Data,8);   //����������ֽ����ı�
	U1_Inf.TX_Data[6]  = check_sum >> 8 ;
	U1_Inf.TX_Data[7]  = check_sum & 0x00FF;

	Usart_SendStr_length(USART1,U1_Inf.TX_Data,8);


	

		return 0;
}



uint8 Boss_Lock_Function(void)
 	{
		
			
 			return 0;
 	}



void NewSHUANG_PIN_Server_Cmd_Analyse(void)
{
   uint8 Cmd = 0;
   uint8 index = 0;
   uint8 Length = 0;
   uint16 Address = 1; //��ݵ�ַ�����ܴ�
   uint16 Value = 0;	
   uint16 Data = 0; 
   float value_buffer = 0;
   uint8  Device_ID = 1; //��ݵ�ַ�����ܴ� ************88
   uint16 check_sum = 0;
   uint16 Data1 = 0;
   uint16 Data2 = 0;
   uint16 Data3 = 0;
   
   Cmd = U1_Inf.RX_Data[1];
   Address = U1_Inf.RX_Data[2] *256+ U1_Inf.RX_Data[3];
   Length = U1_Inf.RX_Data[4] *256+ U1_Inf.RX_Data[5];
   Value = U1_Inf.RX_Data[4] *256+ U1_Inf.RX_Data[5];;	 //06��ʽ�£�value = length
   
   if(U1_Inf.RX_Data[0] > 1)
		{
			//����ѯ��ַ����1ʱ���Զ��л�Ϊ�Է�ѯ�ʵĵ�ַ
			Device_ID = Sys_Admin.ModBus_Address;
		}
   switch (Cmd)
	   {
		   case 03:  //00 03 01 F3 00 0F F5 D0		

				   switch (Address)
					   {
						   case 100:
								   if(Address == 100 && Length == 18)
									   {
										   U1_Inf.TX_Data[0] = Device_ID;
										   U1_Inf.TX_Data[1] = 0x03;// 
										   U1_Inf.TX_Data[2] = 36; //���ݳ���Ϊ6�� ��������ı�***
								   
										   U1_Inf.TX_Data[3] = 0x00;
										   U1_Inf.TX_Data[4] = UnionD.UnionStartFlag;//1��������״̬

										   U1_Inf.TX_Data[5] = 0x00;
										   U1_Inf.TX_Data[6] = sys_data.Data_10H;//2����������״̬

										   U1_Inf.TX_Data[7] = 0x00;
										   U1_Inf.TX_Data[8] = 0x00;//3�������¶�,  ��Ҫ���⴦��һ��

										   U1_Inf.TX_Data[9] = 0x00;
										   U1_Inf.TX_Data[10] = LCD10D.DLCD.Water_State;//4����ˮλ״̬

										   Data =  Temperature_Data.Smoke_Tem / 10;
										   U1_Inf.TX_Data[11] = Data >> 8;
										   U1_Inf.TX_Data[12] = Data & 0x00FF;//5���������¶�

										    Data3 =LCD10D.DLCD.LuNei_WenDu;
										   U1_Inf.TX_Data[13] = Data3 >> 8;
										   U1_Inf.TX_Data[14] = Data3 & 0X00ff;//6����������

										   U1_Inf.TX_Data[15] = 0;
										   U1_Inf.TX_Data[16] = LCD10D.DLCD.Error_Code;//7���������ϴ���

										   U1_Inf.TX_Data[17] = 0;
										   U1_Inf.TX_Data[18] = LCD10D.DLCD.Air_Power;//8�������������

										   U1_Inf.TX_Data[19] = 0;
										   U1_Inf.TX_Data[20] = LCD10D.DLCD.Pump_State;
										   //9��ˮ��״̬

										   U1_Inf.TX_Data[21] = 0;
										   U1_Inf.TX_Data[22] = LCD10D.DLCD.Flame_State;//10������״̬

										   U1_Inf.TX_Data[23] = 0;
										   U1_Inf.TX_Data[24] = LCD10JZ[2].DLCD.Water_State;//11����ˮλ״̬

										   Data3 = LCD10JZ[2].DLCD.Smoke_WenDu;
										   U1_Inf.TX_Data[25] = Data3 >> 8;
										  
										   U1_Inf.TX_Data[26] = Data3 & 0x00FF;//12��������
											
										   Data3 = LCD10JZ[2].DLCD.LuNei_WenDu;
										   U1_Inf.TX_Data[27] = Data3 >> 8;
										   U1_Inf.TX_Data[28] = Data3 & 0x00FF;//13��������

										   U1_Inf.TX_Data[29] = 0;
										   U1_Inf.TX_Data[30] = LCD10JZ[2].DLCD.Error_Code;//14���ӹ�����

										   U1_Inf.TX_Data[31] = 0;
										   U1_Inf.TX_Data[32] = LCD10JZ[2].DLCD.Air_Power;//15���ӷ������

										   
										   U1_Inf.TX_Data[33] = 0;
										   U1_Inf.TX_Data[34] = LCD10JZ[2].DLCD.Pump_State ;//16����ˮ��״̬

										   U1_Inf.TX_Data[35] = 0;
										   U1_Inf.TX_Data[36] = LCD10JZ[2].DLCD.Flame_State ;//17���ӻ���״̬


										   U1_Inf.TX_Data[37] = 0;
										   U1_Inf.TX_Data[38] = LCD10JZ[2].DLCD.Device_State ;//18���ӹ���״̬


										   //*******************
										   
										   check_sum  = ModBusCRC16(U1_Inf.TX_Data,41);   //���11�������ֽ����ı�
										   U1_Inf.TX_Data[39]  = check_sum >> 8 ;
										   U1_Inf.TX_Data[40]  = check_sum & 0x00FF;
										   
										   Usart_SendStr_length(USART1,U1_Inf.TX_Data,41);
										   
									   }
								   break;
						   case 120:  //��ȡ����ѹ��������
								   
								   if(Address == 120 && Length == 4)
									   {
										   U1_Inf.TX_Data[0] = Device_ID;
										   U1_Inf.TX_Data[1] = 0x03;// 
										   U1_Inf.TX_Data[2] = 8; //���ݳ���Ϊ �� ��������ı�***

										  
										   Float_Int.value = LCD10JZ[1].DLCD.Steam_Pressure;
										   U1_Inf.TX_Data[3] = Float_Int.byte4.data_HH;
										   U1_Inf.TX_Data[4] = Float_Int.byte4.data_HL;//  

										   U1_Inf.TX_Data[5] = Float_Int.byte4.data_LH;
										   U1_Inf.TX_Data[6] = Float_Int.byte4.data_LL;
										   
											   
											   
										   Float_Int.value = LCD10JZ[2].DLCD.Steam_Pressure;
										   U1_Inf.TX_Data[7] = Float_Int.byte4.data_HH;
										   U1_Inf.TX_Data[8] = Float_Int.byte4.data_HL;//  

										   U1_Inf.TX_Data[9] = Float_Int.byte4.data_LH;
										   U1_Inf.TX_Data[10] = Float_Int.byte4.data_LL;

										   check_sum  = ModBusCRC16(U1_Inf.TX_Data,13);   //����������ֽ����ı�
										   U1_Inf.TX_Data[11]  = check_sum >> 8 ;
										   U1_Inf.TX_Data[12]  = check_sum & 0x00FF;
										   
										   Usart_SendStr_length(USART1,U1_Inf.TX_Data,13);
										   
									   }

								   break;
						  case 130:  //��ȡ���ӻ���ѹ�࣬����ѹ��������
								   
								   if(Address == 130 && Length == 4)
									   {
										   U1_Inf.TX_Data[0] = Device_ID;
										   U1_Inf.TX_Data[1] = 0x03;// 
										   U1_Inf.TX_Data[2] = 8; //���ݳ���Ϊ �� ��������ı�***

										  
										   Float_Int.value = LCD10D.DLCD.Inside_High_Pressure;
										   U1_Inf.TX_Data[3] = Float_Int.byte4.data_HH;
										   U1_Inf.TX_Data[4] = Float_Int.byte4.data_HL;//  

										   U1_Inf.TX_Data[5] = Float_Int.byte4.data_LH;
										   U1_Inf.TX_Data[6] = Float_Int.byte4.data_LL;
										   
											   
											   
										   Float_Int.value = LCD10JZ[2].DLCD.Inside_High_Pressure;
										   U1_Inf.TX_Data[7] = Float_Int.byte4.data_HH;
										   U1_Inf.TX_Data[8] = Float_Int.byte4.data_HL;//  

										   U1_Inf.TX_Data[9] = Float_Int.byte4.data_LH;
										   U1_Inf.TX_Data[10] = Float_Int.byte4.data_LL;

										   check_sum  = ModBusCRC16(U1_Inf.TX_Data,13);   //����������ֽ����ı�
										   U1_Inf.TX_Data[11]  = check_sum >> 8 ;
										   U1_Inf.TX_Data[12]  = check_sum & 0x00FF;
										   
										   Usart_SendStr_length(USART1,U1_Inf.TX_Data,13);
										   
									   }

								   break;

							 
						   case 150:  //����ѹ����������
								   
									   if( Length == 3)  
										   {
											   U1_Inf.TX_Data[0] = Device_ID;
											   U1_Inf.TX_Data[1] = 0x03;// 
											   U1_Inf.TX_Data[2] = 6; //���ݳ���Ϊ �� ��������ı�***

											   U1_Inf.TX_Data[3] = sys_config_data.zhuan_huan_temperture_value >> 8;
											   U1_Inf.TX_Data[4] = sys_config_data.zhuan_huan_temperture_value & 0x00FF;// 1Ŀ������ѹ��

											   U1_Inf.TX_Data[5] = sys_config_data.Auto_stop_pressure >> 8;
											   U1_Inf.TX_Data[6] = sys_config_data.Auto_stop_pressure & 0x00FF;

											   U1_Inf.TX_Data[7] = sys_config_data.Auto_start_pressure >> 8;
											   U1_Inf.TX_Data[8] = sys_config_data.Auto_start_pressure & 0x00FF;

											   check_sum  = ModBusCRC16(U1_Inf.TX_Data,11);   //����������ֽ����ı�
											   U1_Inf.TX_Data[9]  = check_sum >> 8 ;
											   U1_Inf.TX_Data[10]  = check_sum & 0x00FF;
											   
											   Usart_SendStr_length(USART1,U1_Inf.TX_Data,11);
										   }
								   break;
							case 160:
									if( Length == 4)  
											{
												U1_Inf.TX_Data[0] = Device_ID;
												U1_Inf.TX_Data[1] = 0x03;// 
												U1_Inf.TX_Data[2] = 8 ;    //�����ֵҲҪ���ű�
												
												U1_Inf.TX_Data[3] = 0;
												U1_Inf.TX_Data[4] = sys_flag.Wifi_Lock_System;

												U1_Inf.TX_Data[5] = sys_flag.wifi_Lock_Year >> 8;
												U1_Inf.TX_Data[6] = sys_flag.wifi_Lock_Year & 0x00FF;

												U1_Inf.TX_Data[7] = 0;
												U1_Inf.TX_Data[8] = sys_flag.wifi_Lock_Month;

												U1_Inf.TX_Data[9] = 0;
												U1_Inf.TX_Data[10] = sys_flag.Wifi_Lock_Day;
												
												check_sum  = ModBusCRC16(U1_Inf.TX_Data,13);   //����������ֽ����ı�
												U1_Inf.TX_Data[11]  = check_sum >> 8 ;
												U1_Inf.TX_Data[12]  = check_sum & 0x00FF;
												
												Usart_SendStr_length(USART1,U1_Inf.TX_Data,13);
											}


									break;


						   default:

								   break;
					   }
				   
				   break;
		   case 06://�����Ĵ�����д��
				   
				   switch (Address)
					   {
						   case 0x0064:////��ַ40100 Ϊ������ ������ͣ�Ĵ���
									   Data = U1_Inf.RX_Data[4] *256+ U1_Inf.RX_Data[5];
									   if(Data == 0x00)//��������
									   {
										   if(UnionD.UnionStartFlag) 
											   {
												   UnionD.UnionStartFlag = FALSE;//ȡ������ָ��
												  
												   
												 if(sys_data.Data_10H == 2)
													sys_close_cmd();
								
								
												SlaveG[2].Startclose_Sendflag = 3;
												SlaveG[2].Startclose_Data = 0;

												ModuBusRTU_WriteResponse(Address,Value);
												
												
											   }
									   }

									   if(Data == 0x0001)//��������
									   {
										   //����δ���ã��ӻ�Ҳû���ӣ���������
										    ModuBusRTU_WriteResponse(Address,Value);
										   if(LCD10JZ[1].DLCD.YunXu_Flag == 0 && LCD10JZ[2].DLCD.YunXu_Flag == 0)
											   break;

										   
										   if(LCD10JZ[1].DLCD.YunXu_Flag  || LCD10JZ[2].DLCD.YunXu_Flag )
											   if(UnionD.UnionStartFlag  == 0) //ϵͳ���ڴ���״̬���ֶ��򱨾�
												   {
													  UnionD.UnionStartFlag = OK; //������״̬���л�

													 
												   }
										   
									   }
									  

										   
									   break;

						   case 0x006A:// ���ϸ�λ�Ĵ���,�������ϸ�λ���
										
									   if((sys_flag.Error_Code)||LCD10JZ[2].DLCD.Error_Code)
										   { 
											   LCD10JZ[2].DLCD.Error_Code = 0;
												SlaveG[2].ResetError_Flag = 3;
												SlaveG[2].ResetError_Data = 0;
											   sys_flag.Error_Code = 0;
											   Beep_Data.beep_start_flag = 0;

											  

											   Beep_Data.beep_start_flag = 0;//������־
											   sys_flag.Lock_Error = 0; //���������������
												
											  ModuBusRTU_WriteResponse(Address,Value);

											   
										   }
									   
									   break;

						   case 0x0071: //�ӻ��Ĺ��ϸ�λ��������ִ��ͬ���Ĺ���
									   if(sys_flag.Error_Code ||LCD10JZ[2].DLCD.Error_Code)
										   { 
											   LCD10JZ[2].DLCD.Error_Code = 0;
												SlaveG[2].ResetError_Flag = 3;
												SlaveG[2].ResetError_Data = 0;
											   sys_flag.Error_Code = 0;
											   Beep_Data.beep_start_flag = 0;
											  
												ModuBusRTU_WriteResponse(Address,Value);
											  
												
											   
										   }

									   break;

						   case 150:  //Ŀ������ѹ������
									   
											   if(Value < sys_config_data.Auto_stop_pressure)
												   {
													   if(Value >= 20)
														   {
															   sys_config_data.zhuan_huan_temperture_value = Value;
															   Write_Internal_Flash();//����ز������б���

															   ModuBusRTU_WriteResponse(Address,Value);
															  
															   
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
														   ModuBusRTU_WriteResponse(Address,Value);
														   
													   }
											   }

										  
									   
								   
								   break;
						   case 152: //��������ѹ��
									   
											   if(Value < sys_config_data.Auto_stop_pressure)
												   {
													   sys_config_data.Auto_start_pressure = Value;
													   Write_Internal_Flash();//����ز������б���

													   ModuBusRTU_WriteResponse(Address,Value);
												   }
									   break;
						 	case 160:
										if( Value <= 1)//ִ���������������
											{
												sys_flag.Wifi_Lock_System = Value; 
												
												Write_Internal_Flash();

												ModuBusRTU_WriteResponse(Address,Value);
											}

										break;

							case 161:
									//ִ�����������
									if( Value >= 0)
										{
											sys_flag.wifi_Lock_Year = Value; 

											Write_Internal_Flash();

											ModuBusRTU_WriteResponse(Address,Value);

											//�ñ���������
											
										}

									break;
							case 162:
									//ִ���������·�
									if( Value >= 0 && Value <= 12)
										{
											sys_flag.wifi_Lock_Month = Value; 

											Write_Internal_Flash();
											ModuBusRTU_WriteResponse(Address,Value);
										}

									break;
							case 163:
									//ִ������������
									if( Value >= 0 && Value <= 30)
										{
											sys_flag.Wifi_Lock_Day = Value; 

											Write_Internal_Flash();

											ModuBusRTU_WriteResponse(Address,Value);
										}
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


uint32 Char_to_Int_6(uint32 number_H,uint32 number_L) 
{
    uint8 Code[6] ={0};
	uint32 Return_Value = 0;



	Code[0] = number_L & 0x000000FF - 0x30;
	number_L  = number_L >> 8;
	Code[1] = number_L & 0x000000FF - 0x30;
	 
	Code[2] = number_H & 0x000000FF - 0x30;
	number_H  = number_H >> 8;
	Code[3] = number_H & 0x000000FF - 0x30;
	number_H  = number_H >> 8;
	Code[4] = number_H & 0x000000FF - 0x30;
	number_H  = number_H >> 8;
	Code[5] = number_H & 0x000000FF - 0x30;
	
	 Return_Value =Code[0] + Code[1]* 10 + Code[2]* 100 + Code[3]* 1000 + Code[4]* 10000 + Code[5]* 100000;
	
	
	
    return Return_Value; // ����ת�����BCD��
}






