#include "usart5.h"
#include "stdarg.h"







void U5_send_byte(u8 data)
{
	 USART_SendData(UART5,data);
	 while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
}

//����5����s���ַ�
void U5_send_str(u8 *str,u8 s)
{
	u8 i;
	for(i=0;i<s;i++)
	{
		U5_send_byte(*str);
		str++;
	}
}


///////////////////////////////////////USART5 printf֧�ֲ���//////////////////////////////////
//����2,u2_printf ����
//ȷ��һ�η������ݲ�����USART4_MAX_SEND_LEN�ֽ�
////////////////////////////////////////////////////////////////////////////////////////////////
void U5_Printf(char* fmt,...)  
{  
  int len=0;
	int cnt=0;

	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)U5_Inf.TX_Data,fmt,ap);
	va_end(ap);
	len = strlen((const char*)U5_Inf.TX_Data);
	while(len--)
	  {
	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=1); //�ȴ����ͽ���
	  	USART_SendData(UART5,U5_Inf.TX_Data[cnt++]);
	  }
}










///////////////////////////////////////USART2 ��ʼ�����ò���//////////////////////////////////	    
//���ܣ���ʼ��IO ����2
//�������
//bound:������
//�������
//��
//////////////////////////////////////////////////////////////////////////////////////////////	  
void uart5_init(u32 bound)
{  	 		 
	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//ʹ��USART5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD, ENABLE);	//ʹ��GPIOC��GPIODʱ��
	 
	USART_DeInit(UART5);  //��λ����5

     //UART5_TX   PC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //UART5_RX	  PD.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOD, &GPIO_InitStructure);  

   

	
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	        //����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
    //USART5 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;   // 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;  //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	  //�շ�ģʽ

    USART_Init(UART5, &USART_InitStructure);   //��ʼ������
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);  //�����ж�
    USART_Cmd(UART5, ENABLE);                      //ʹ�ܴ��� 
	
//	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);    //ʹ�ܴ���2��DMA����
//	UART_DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF,1000);  //DMA1ͨ��7,����Ϊ����2,�洢��ΪUSART2_TX_BUF,����1000. 										  	
}










//MCU�����¹������ݷ���LCD


void send_byte(u8 data)
{
	 USART_SendData(USART2,data);
	 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
}
void send_str(u8 *str,u8 s)
{
	u8 i;
	for(i=0;i<s;i++)
	{
		send_byte(*str);
		str++;
	}
}







