/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  helic
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   �̵���Ӧ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  
 
#include "main.h"

SWITCH_STATUS Switch_Inf; 




/**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void RELAYS_GPIO_Config(void)
{		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

	
	//1__________�̵���1��ʼ��
			RCC_APB2PeriphClockCmd( RELAY1_GPIO_CLK, ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
			GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN | RELAY2_GPIO_PIN | RELAY3_GPIO_PIN | RELAY5_GPIO_PIN | RELAY6_GPIO_PIN;	
			
			/*��������ģʽΪͨ���������*/
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	
			/*������������Ϊ50MHz */   
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
			/*���ÿ⺯������ʼ��GPIO*/
			GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStructure);	
	
	
			RCC_APB2PeriphClockCmd( RELAY7_GPIO_CLK, ENABLE);
		
			GPIO_InitStructure.GPIO_Pin = RELAY7_GPIO_PIN | RELAY8_GPIO_PIN | RELAY9_GPIO_PIN | RELAY10_GPIO_PIN;	
	
			
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	
			
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	
			GPIO_Init(RELAY7_GPIO_PORT, &GPIO_InitStructure);	

		
	
			RCC_APB2PeriphClockCmd( RELAY4_GPIO_CLK, ENABLE);
		
			GPIO_InitStructure.GPIO_Pin = RELAY4_GPIO_PIN ; 
	
			
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	
			
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
			
			GPIO_Init(RELAY4_GPIO_PORT, &GPIO_InitStructure);	

	
}




/*
�̵���1�� ��ˮ

�̵���2��  ���

�̵���3����ˮ��ŷ�

�̵���4�� ����

�̵���5�� ȼ������

�̵���6�� �Զ����۷�

�̵���7�� ���

�̵���8�� �������


*/


/**
  * @brief  ���Ƽ̵�����𣬿�ʼ
  * @param  ��
  * @retval ��
  */
void	Dian_Huo_Start(void)
{
//�ڶ�������·�̵��� =============���̵����������������������ļ̵���
		
		Switch_Inf.dian_huo_flag = 1;
		 
	 	
		 
}



/**
  * @brief  ���Ƶ��̵������ر�
  * @param  ��
  * @retval ��
  */
void	Dian_Huo_OFF(void)
{		
	Switch_Inf.dian_huo_flag = 0;
	 
}

//ȼ�����鿪��
void 	Send_Gas_Open(void) //ȼ�����鿪��
{
	//��ȼ������δ������������Ӧ�ļ̵���
	

		Switch_Inf.gas_on_flag = 1;
		 
	
}


//ȼ������ر�
void	Send_Gas_Close(void)//ȼ������ر�
{

//����������·�̵��� =============ȼ����Դ���ء������������������ļ̵���	
	
		Switch_Inf.gas_on_flag = 0;
		
		
}


/**
  * @brief  ���Ʒ����������� 
  * @param  ��
  * @retval ��
  */
void	Send_Air_Open(void)
{
//������·�̵��� =============�����Դ���ء������������������ļ̵���	

		
	
		
 		Switch_Inf.air_on_flag  = 1;

		
}


/**
  * @brief  �������ر�
  * @param  ��
  * @retval ��
  */
void	Send_Air_Close(void)
{
	
			
		//�رշ��������ʾ
	
		
		Switch_Inf.air_on_flag  = 0;
		
		
		PWM_Adjust(0);
		 

		now_percent = 0;
	
		
}


/**
  * @brief  ���Ʒ����ɨ�͵����٣�����50%
  * @param  ��
  * @retval ��
  */

void Feed_First_Level(void)
{
		 
		
	 
		PWM_Adjust(100);
		 
		 
		now_percent = 100;
		 
		
		
	

}









uint8 Dian_Huo_Air_Level(void)
{
		 
		uint8 value = 0;

		value =  *(uint32 *)(DIAN_HUO_POWER_ADDRESS);

		if(value < Min_Dian_Huo_Power)
			value = Min_Dian_Huo_Power;

		if(value > Max_Dian_Huo_Power)
			value = Max_Dian_Huo_Power;    //TIM3->CCR1 = 500 - ��value*5��

		
	 	
		PWM_Adjust(value);

		now_percent =value;
		 
		
		 
	
		 
		return OK;

}

/**
  * @brief  ���Ʒ������ٶȣ�
  * @param  ��
  * @retval ��
  */

void Feed_test_begin(void)
{
		//����ģʽ�У����100%�ķ�����ɨ
		
		sys_data.Data_1FH = 0X64;
		

		PWM_Adjust(100);


}



 void Feed_test_over(void)
{
	 
	
	
	PWM_Adjust(0);
	now_percent = 0;
	 
	 
}



/**
  * @brief  ���Ƹ�ˮ���á����� 
  * @param  ��
  * @retval ��
  */
uint8  Feed_Main_Pump_ON(void)
{
	//���ֶ�ģʽ������Ƶ������������
	if(sys_flag.WaterOut_Time <= 2)
		return 0;

	

	Switch_Inf.water_switch_flag = 1;	

	
		
	return 0;
}



uint8 Logic_Pump_On(void)
{
	//��̨�豸����һ���õĴ���
	//RELAY5_ON;
	
	Switch_Inf.water_switch_flag = 1;


	 
	 
	return 0;
}

uint8 Logic_Pump_OFF(void)
{
	//RELAY5_OFF;
		
		//����ˮ�ö�����ʾ
			
		Switch_Inf.water_switch_flag = 0;	
	
	

		return 0;
}


/**
  * @brief  ���Ƹ�ˮ���á���CLOSE
  * @param  ��
  * @retval ��
  */
uint8 Feed_Main_Pump_OFF(void)
{
		
		//����ˮ�ö�����ʾ
		//	RELAY5_OFF;

		sys_flag.Water_Percent = 0;
		Switch_Inf.water_switch_flag = 0;
		

		return 0;
		
}


uint8 Special_Water_Open(void)
{
	
	return 0;
}



uint8 Special_Water_OFF(void)
{
	
	return 0;
}



uint8  Second_Water_Valve_Open(void)
{
	//���ڲ�ˮ��ŷ�����ʹ��,��ŷ�������󣬲�ˮ�ÿ�
	//RELAY4_ON;
	Switch_Inf.Water_Valve_Flag = OK;
	Water_State.Zstate_Flag = OK;

	return 0;
}

uint8  Second_Water_Valve_Close(void)
{
	//���ڲ�ˮ��ŷ� �ر�ʹ�ã���ˮ�ùر�����󣬵�ŷ���
	

	switch (Sys_Admin.Device_Style)
		{
			case 0:
			case 1:
					if(sys_flag.WaterClose_Time > 1) 
						{
							Switch_Inf.Water_Valve_Flag = FALSE;
							Water_State.Zstate_Flag = 0;
						}

					break;
			case 2:
			case 3:
					Switch_Inf.Water_Valve_Flag = FALSE;
					Water_State.Zstate_Flag = 0;

					break;
			default:
				Sys_Admin.Device_Style = 0;

					break;
		}
		
	//if(sys_flag.WaterClose_Time <= 1)  //ˮ�ùر�С��2�룬���ܹرղ�ˮ��ŷ�
	//	return 0;
		
	
	
	
	
	return 0;
}



void  Pai_Wu_Door_Open(void)
{

	
	RELAY9_ON;
	
	Switch_Inf.pai_wu_flag =1;
	
	
}


void  Pai_Wu_Door_Close(void)
{
	
	RELAY9_OFF;
	
	Switch_Inf.pai_wu_flag = 0;
	
}


void WTS_Gas_One_Open(void)
{
	//����ʽ�£��Ż�ʹ��
	 Switch_Inf.MingHuo_Flag = OK;
	//RELAY2_ON;
}



void WTS_Gas_One_Close(void)
{
	//����ʽ�£��Ż�ʹ��
	Switch_Inf.MingHuo_Flag = 0;
	//RELAY2_OFF;
}


uint8 LianXu_Paiwu_Open(void)
{
	//RELAY10_ON;	
	 
	Switch_Inf.LianXu_PaiWu_flag = OK;
	
	return 0;
}

uint8 LianXu_Paiwu_Close(void)
{
	//RELAY10_OFF;	

	Switch_Inf.LianXu_PaiWu_flag = FALSE;
	 
	return 0;
}


uint8 Alarm_Out_Function(void)
{

	if(Sys_Admin.Device_Style == 0 || Sys_Admin.Device_Style == 1)
		{
			if(sys_flag.Error_Code)
				{
					if(sys_flag.Sys_1_Sec)
						{
							sys_flag.Sys_1_Sec = 0;
							if(sys_flag.Alarm_Out)
								{
									RELAY1_ON;
								}
								
						}	
				}
			else
				{
					RELAY1_OFF;
				}
				
		}


	if(Sys_Admin.Device_Style == 2 || Sys_Admin.Device_Style == 3)
		{
			if(sys_flag.Error_Code || LCD10JZ[2].DLCD.Error_Code  )
				{
					if(sys_flag.Sys_1_Sec)
						{
							sys_flag.Sys_1_Sec = 0;
							if(sys_flag.Alarm_Out)
								{
									RELAY1_ON;
								}
								
						}	
				}
			else
				{
					RELAY1_OFF;
				}
				
		}
	 
	
	

	return 0;
}

uint8 Relays_NoInterrupt_ON_OFF(void)
{
	if(sys_flag.Interrupt_Alive == FALSE)
		{
			if(Switch_Inf.air_on_flag)
	  		 	{
	  		 		RELAY6_ON;  //�ͷ�������
					RELAY3_ON; //ֱ���̵����ſ���
	  		 	}
			 else
			 	{
			 		RELAY6_OFF;  //�ͷ�������
					RELAY3_OFF; //ֱ���̵����ſ���
			 	}

			 if(Switch_Inf.gas_on_flag)
			 	{
			 		RELAY7_ON;
			 	}
			 else
			 	{
			 		RELAY7_OFF;
			 	}

			 if(Switch_Inf.dian_huo_flag)
			 	{
			 		RELAY8_ON;
			 	}
			 else
			 	{
			 		RELAY8_OFF;
			 	}


			 if(Switch_Inf.water_switch_flag)
			 	{
			 		RELAY5_ON;
			 	}
			 else
			 	{
			 		RELAY5_OFF;
			 	}

			 if(Switch_Inf.Water_Valve_Flag)
			 	{
			 		RELAY4_ON;
			 	}
			 else
			 	{
			 		RELAY4_OFF;
			 	}

			 if(Switch_Inf.pai_wu_flag)
			 	{
			 		RELAY9_ON;
			 	}
			 else
			 	{
			 		RELAY9_OFF;
			 	}

			 if(Switch_Inf.MingHuo_Flag)
			 	{
			 		RELAY2_ON;
			 	}
			 else
			 	{
			 		RELAY2_OFF;
			 	}

			  if(Switch_Inf.LianXu_PaiWu_flag)
			 	{
			 		RELAY10_ON;
			 	}
			 else
			 	{
			 		RELAY10_OFF;
			 	}

			
		}


	return 0;
}

/*********************************************END OF FILE**********************/

void Close_All_Realys_Function(void)
{
	RELAY1_OFF;
	RELAY2_OFF;
	RELAY3_OFF;
	RELAY4_OFF;
	RELAY5_OFF;
	RELAY6_OFF;
	RELAY7_OFF;
	RELAY8_OFF;
	RELAY9_OFF;
	RELAY10_OFF;
}





