/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  helic
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   ledӦ�ú����ӿںͷ������ӿڶ���
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   


SPI_InitType SPI_Gpio_Init =
{
	{GPIOE,GPIOE,GPIOE,GPIOE},
	{GPIO_Pin_8,GPIO_Pin_7,GPIO_Pin_9,GPIO_Pin_10}
};


#define SPI_CS_H GPIO_WriteBit(SPI_Gpio_Init.port[CS],SPI_Gpio_Init.pin[CS],Bit_SET)
#define SPI_CS_L GPIO_WriteBit(SPI_Gpio_Init.port[CS],SPI_Gpio_Init.pin[CS],Bit_RESET)

#define SPI_CLK_H GPIO_WriteBit(SPI_Gpio_Init.port[SCLK],SPI_Gpio_Init.pin[SCLK],Bit_SET)
#define SPI_CLK_L GPIO_WriteBit(SPI_Gpio_Init.port[SCLK],SPI_Gpio_Init.pin[SCLK],Bit_RESET)

#define SPI_DIN_H GPIO_WriteBit(SPI_Gpio_Init.port[DIN],SPI_Gpio_Init.pin[DIN],Bit_SET)
#define SPI_DIN_L GPIO_WriteBit(SPI_Gpio_Init.port[DIN],SPI_Gpio_Init.pin[DIN],Bit_RESET)

#define SPI_MISO_DOUT GPIO_ReadInputDataBit(SPI_Gpio_Init.port[DOUT],SPI_Gpio_Init.pin[DOUT])


Beep_Message Beep_Data;

//uint16 beep_time_set = 0;
//uint8  beep_start_flag = 0; //���������������� �� ����
//uint8	 beep_on_flag  = 0;  //��������
//uint8  beep_period_flag =0;//���ڱ�־
//uint8  beep_period_time = 200 ;//�趨����ʱ��
uint8		re4_time		 = 0;	

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	
//===================BEEP====================================
	
		RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE);
	 
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	

		 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	 
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);	

//===================����LED====================================

		
	 	RCC_APB2PeriphClockCmd( WORK_LED_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = WORK_LED_GPIO_PIN;	

		 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	 
		GPIO_Init(WORK_LED_GPIO_PORT, &GPIO_InitStructure);	
		


//===================���ÿ��Ź� WDI ����====================================
		RCC_APB2PeriphClockCmd( WDI_GPIO_CLK, ENABLE);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = WDI_GPIO_PIN;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(WDI_GPIO_PORT, &GPIO_InitStructure);	




//================���뿪�ص�ַ���ö˿�====================================
	
		RCC_APB2PeriphClockCmd(SLAVE_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin =  SLAVE_PIN_2 | SLAVE_PIN_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(SLAVE_ADDRESS_PORT, &GPIO_InitStructure);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin =  SLAVE_PIN_3 | SLAVE_PIN_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA ,&GPIO_InitStructure);
	



//=================����������-------------------

		
		RCC_APB2PeriphClockCmd(INPUT_GPIO_CLK, ENABLE);
		GPIO_InitStructure.GPIO_Pin = INPUT_PIN_1 | INPUT_PIN_2 |INPUT_PIN_3 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������ģʽ
		GPIO_Init(INPUT_ADDRESS_PORT, &GPIO_InitStructure);

		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

		
		//IO9 ��IO10 �������ź��෴
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = INPUT_PIN_4 | INPUT_PIN_5 |INPUT_PIN_6|INPUT_PIN_7|INPUT_PIN_8|INPUT_PIN_9|INPUT_PIN_10 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������ģʽ
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = INPUT_PIN_11 | INPUT_PIN_12  ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������ģʽ
		GPIO_Init(GPIOE, &GPIO_InitStructure);


	//==============��������ź����ż�⣬PB0             ��������============		

	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	 
}



uint8 IO_Interrupt_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure ;
	  EXTI_InitTypeDef EXTI_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	 // ʹ��PD�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
 
    // ����PD12Ϊ���� 
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ���Ը���������������
    GPIO_Init(GPIOD, &GPIO_InitStructure);
 
    // ʹ��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 
    // ����PD12���ж���·
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource12);
 
    // �����жϴ�����ʽ
   
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // �����������ô�����ʽ
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
 
    // ����NVIC
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);



	/**********************PWM s�����ؼ��*********************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ���Ը���������������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // �����������ô�����ʽ
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);



	return 0;
}

void  BEEP_TIME(uint16 nms)
{
		
		Beep_Data.beep_time_set = nms;//������ʱ���趨
		Beep_Data.beep_on_flag = 1;//������������־

}






uint8 PortPin_Scan(GPIO_TypeDef *GPIOx,uint16 GPIO_Pin)
{
	if( GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == PIN_HIGH )
	{
		return PIN_HIGH;
	}
	else return PIN_LOW;
}


uint8 New_Flame_Scan(void)
{
	




	return 0 ;
}



void Delay_us(uint16 Value)
{
	uint8 i = 0;
  while(Value--)
	{ 
		for( i=0;i<20;i++)
		{
		   
		}
	}
}



/**
  * @brief  Delay_us(uint16_t Value) 1us
  * @param ???��us????
  * @retval int
  */
static void SPI_delay_us(uint16 Value)
{
	uint8 i=0;
  while(Value--)
	{ 
		for(i=0;i<10;i++)
		{
			 __NOP;
		}
	}
}

	

void SPI_Config_Init(void)
{
	uint8 i = 0;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  for(i=0;i<3;i++)
  {
		GPIO_InitStruct.GPIO_Pin = SPI_Gpio_Init.pin[i];
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(SPI_Gpio_Init.port[i], &GPIO_InitStruct);

  }
  for(i=3;i<4;i++)
  {
		GPIO_InitStruct.GPIO_Pin = SPI_Gpio_Init.pin[i];
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(SPI_Gpio_Init.port[i], &GPIO_InitStruct);
  }
  SPI_CS_H;
  SPI_CLK_L; 
}


void SpiCsStatus( uint8_t state)
{
  switch(state)
  {
	  case SPI_DISABLE:
			SPI_CS_H;
	  break;
	  case SPI_ENABLE:
			SPI_CS_L;
	  break;
  }
}
uint8 SPI_RW_Data_MODE0(uint8 sendData)
{
	uint8 n = 0;
	uint8 revData = 0;
	for(n=0;n<8;n++)
	{
		if(sendData&0x80)
			SPI_DIN_H;
		else 
			SPI_DIN_L;
		sendData <<= 1;
		SPI_delay_us(100);
		SPI_CLK_H;
		revData<<=1;
		if(SPI_MISO_DOUT)
			revData ++;
		SPI_delay_us(100);
		SPI_CLK_L;
	}
	return revData;
}


uint8  SpiReadData(void)
{
	uint8 ReciveData[5] = {0};
	uint8 DataCRC = 0;
	//uint8 TxData[5] = {0x68,0x00,0x64,0x01,0xCD};
	uint8 TxData[5] = {0};
	uint16 Alarm_Value = 0;
	static uint16 Alarm_Count = 0;

	static uint8 Unconnect_Count = 0;
	uint8  i=0;
	//����: 0x68   (�趨���¶� HH LL)      (ʹ�� 0x01)   (��У��ȡ��λ)

	//����: 0x68   (ʵʱ�¶� HH LL)      (ʹ�� 0x01)   (��У��ȡ��λ)
	if(sys_flag.SPI_ReadRX_Flag == FALSE) //ÿ����һ��
		return 0;

	sys_flag.SPI_ReadRX_Flag = FALSE;

/******************��������׼���׶�*******************/
	TxData[0] = 0x68; //�̶�ͷ
	TxData[1] = Sys_Admin.Inside_WenDu_ProtectValue >> 8;   //�趨ֵ��λ
	TxData[2] = Sys_Admin.Inside_WenDu_ProtectValue & 0x00FF;   //�趨ֵ��λ
	TxData[3] = Sys_Admin.YuRe_Enabled;  //ʹ�ܿ���
	TxData[4] = TxData[0] + TxData[1] + TxData[2] + TxData[3];  //��У��


	if(Sys_Admin.Inside_WenDu_ProtectValue < 3)
		Sys_Admin.Inside_WenDu_ProtectValue = 200; //��ֹ������ֵ��С�����±߽紦��

	Alarm_Value = Sys_Admin.Inside_WenDu_ProtectValue - 3;  //��ǰ����Ԥ��

	
	if(Sys_Admin.YuRe_Enabled)
		{
			if(sys_flag.Protect_WenDu >= Sys_Admin.Inside_WenDu_ProtectValue)
				{
					Alarm_Count ++;
					if(Alarm_Count >= 4)  //��ֹ�¶�˲�����������˲�
						sys_flag.Error_Code = Error14_BenTiValueVBad;
				}
			else
				Alarm_Count = 0;
		}
	else
		Alarm_Count = 0;
	
	
	SpiCsStatus(SPI_ENABLE);
	for( i=0;i<5;i++)
	{
			ReciveData[i] = 0;
			
			ReciveData[i] = SPI_RW_Data_MODE0(TxData[i]);
			
	}
	DataCRC = ReciveData[0] + ReciveData[1] + ReciveData[2] + ReciveData[3]; //��У��

	
	
	if(DataCRC == ReciveData[4] && ReciveData[0] == 0x68)
		{
				//У����ȷ�󣬽����ݸ�ֵ������
				sys_flag.Protect_WenDu = ReciveData[1]*256 + ReciveData[2];
				Unconnect_Count = 0;
				sys_flag.Find_Host = 0;
		}
	else
		{
			
			if(Sys_Admin.YuRe_Enabled)
				{
					Unconnect_Count ++;
					sys_flag.Find_Host = Unconnect_Count;
					if(Unconnect_Count > 15) //ÿ��һ�Σ������ۼ�30�Σ��򱨾�
						{
							sys_flag.Error_Code = Error17_OutWenKong_TxBad;
							sys_flag.Protect_WenDu = 0; //��������ã���ֵΪ��
						}
						
				}
				
		}
	

	if(sys_flag.Protect_WenDu > 390) //����400�ȣ�����Ϊ�豸����ʹ��
		sys_flag.Protect_WenDu = 999;


	LCD10D.DLCD.LuNei_WenDu = sys_flag.Protect_WenDu; //����ʾ��
	
	SpiCsStatus(SPI_DISABLE);


	return 0;
}







/*********************************************END OF FILE**********************/

