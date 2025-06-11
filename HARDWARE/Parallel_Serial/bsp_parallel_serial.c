/**
  ******************************************************************************
  * @file   bsp_parallel_serial.c
  * @author  helic
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   ��ת����HEF4021B��Ӧ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  
#include "bsp_parallel_serial.h" 


struct HEF_4021B{
		uint8  Din_1[8];
		uint8  Din_2[8];
}HEF_4021B_DATA;


uint8 SER1_Data[8];
uint8 SER2_Data[8];



/**
  * @brief  ��ʼ�����Ʋ�ת����IO
  * @param  ��
  * @retval ��
  */
void PARALLEL_SERIAL_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( SERI1_GPIO_CLK | SERI2_GPIO_PIN , ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = SERI1_GPIO_PIN;	

	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	
	/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(SERI1_GPIO_PORT, &GPIO_InitStructure);	         //SERI1
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = SERI2_GPIO_PIN;                 //SERI2

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(SERI2_GPIO_PORT, &GPIO_InitStructure);
		
		
		
		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( SERCP_GPIO_CLK | SERPL_GPIO_CLK , ENABLE);
		
			/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = SERCP_GPIO_PIN;                 //SERCP 

		/*��������ģʽΪͨ���������*/	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(SERCP_GPIO_PORT, &GPIO_InitStructure);
		
		 	/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = SERPL_GPIO_PIN;                 //SERPL 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(SERPL_GPIO_PORT, &GPIO_InitStructure);
		
			/*ѡ��Ҫ���Ƶ�GPIO����*/
	 
}


void read_serial_data(void)
{
		
	uint8 Value = 0;
	/**************************�����1************************/ 
	IO_Status.Target.water_protect = ! PortPin_Scan(GPIOD,INPUT_PIN_1); // �����1
	if(IO_Status.Target.water_protect)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,1);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,1);
	/**************************�����2************************/ 	
	IO_Status.Target.water_mid = !PortPin_Scan(GPIOD,INPUT_PIN_2);// �����2
	if(IO_Status.Target.water_mid)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,2);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,2);
	/**************************�����3************************/ 	
	IO_Status.Target.water_high = !PortPin_Scan(GPIOD,INPUT_PIN_3);// �����3
	
	if(IO_Status.Target.water_high)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,3);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,3);
	
	
	/**************************�����4************************/ 	
	IO_Status.Target.water_shigh = !PortPin_Scan(GPIOB,INPUT_PIN_4);// �����4
	
	if(IO_Status.Target.water_shigh)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,4);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,4);
		
	
	/**************************�����5************************/ 	
	IO_Status.Target.Flame_Signal = !PortPin_Scan(GPIOB,INPUT_PIN_5);
	
	if(IO_Status.Target.Flame_Signal)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,5);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,5);
		

	/**************************�����6         �����ʹ�ã���Ϊ�ڵ���ѹ�ر���*************************/ 	
	Value = !PortPin_Scan(GPIOB,INPUT_PIN_6);
	IO_Status.Target.XB_Hpress_Ykong = Value;
	if(Sys_Admin.Device_Style == 1 || Sys_Admin.Device_Style == 3)  //��������������
		{
			IO_Status.Target.XB_Hpress_Ykong = Value;
		}
	
	if(Value)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,6);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,6);
	 
	/**************************�����7        ***********************/ 	
	Value = !PortPin_Scan(GPIOB,INPUT_PIN_7); 
	IO_Status.Target.XB_WaterLow = Value;
	if(Sys_Admin.Device_Style == 1 || Sys_Admin.Device_Style == 3)  //��������������
		{
			IO_Status.Target.XB_WaterLow = Value;
		}
	
	if(Value)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,7);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,7);

	/**************************�����8************************/ 	
	Value = !PortPin_Scan(GPIOB,INPUT_PIN_8);// �����8
	IO_Status.Target.XB_WaterHigh = Value;
	if(Value)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,8);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,8);

	//�����ĸ��ź��෴
	/**************************ȼ��ѹ���ź�************************/ 	
	IO_Status.Target.gas_low_pressure  = PortPin_Scan(GPIOB,INPUT_PIN_9); 
	
	if(Value)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,9);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,9);
	/**************************��ѹ�ź�************************/ 	
	IO_Status.Target.Air_Door  = PortPin_Scan(GPIOB,INPUT_PIN_10); 
	
	if(IO_Status.Target.Air_Door)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,10);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,10);
	/**************************�����11************************/ 	
	IO_Status.Target.hot_protect = PortPin_Scan(GPIOE,INPUT_PIN_11);// �����11
	
	if(IO_Status.Target.hot_protect)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,11);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,11);

	/**************************�����12************************/ 	
	IO_Status.Target.hpressure_signal = PortPin_Scan(GPIOE,INPUT_PIN_12);// �����12	
	
	if(IO_Status.Target.hpressure_signal)
    	sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,12);
	else
		sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,12);
	


	

	if(IO_Status.Target.Flame_Signal)
			{
				//�л���ֱ�Ӿʹ����ź�
				sys_flag.flame_state = IO_Status.Target.Flame_Signal;
				sys_flag.Inputs_State = SET_BIT_N(sys_flag.Inputs_State,5);
				sys_flag.FlameFilter = 0;
			}
		else
			{
				//Ϩ��Ӧ�����ӳ�
				
				if(sys_flag.FlameFilter > 15) //������⵽10�Σ�ÿ��100ms
					{
						sys_flag.flame_state = IO_Status.Target.Flame_Signal;
						sys_flag.Inputs_State = CLR_BIT_N(sys_flag.Inputs_State,5);
						sys_flag.FlameFilter = 0;
					}
				else
					{
						
					}

				//sys_flag.flame_state = IO_Status.Target.Flame_Signal;  //ȼ�ջ�����ʹ��
			}
			
	Switch_Inf.Flame_Flag = sys_flag.flame_state;


/**************************************************************************/
//****************ʹ�ò�ѹҺλ���������ж�Һλ���иߵ�
/**************************************************************************/

	if(Sys_Admin.ChaYa_WaterHigh_Enabled)
		{

			IO_Status.Target.water_protect = 0;
			IO_Status.Target.water_mid = 0;
			IO_Status.Target.water_high = 0;
			IO_Status.Target.water_shigh = 0;

			if(sys_flag.ChaYaWater_Value >= Sys_Admin.ChaYa_WaterLow_Set)
				{
					IO_Status.Target.water_protect = OK;
				}

			if(sys_flag.ChaYaWater_Value >= Sys_Admin.ChaYa_WaterMid_Set)
				{
					IO_Status.Target.water_protect = OK;
					IO_Status.Target.water_mid = OK;
				}

			if(sys_flag.ChaYaWater_Value >= Sys_Admin.ChaYa_WaterHigh_Set)
				{
					IO_Status.Target.water_protect = OK;
					IO_Status.Target.water_mid = OK;
					IO_Status.Target.water_high = OK;
				}

		
		}

	

	
}
	
	



 




