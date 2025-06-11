
	  
#include "main.h"

 



#if 1
#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 


FILE __stdout;
// FILE __stdin;
// FILE __stderr;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 

//_ttywrch(int ch)
//{
//	ch =ch;
//}

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}




#endif 


 
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  


RTU_DATA Rtu_Data ;




//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

void u1_printf(char* fmt,...)  
{  
  	int len=0;
	int cnt=0;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)U1_Inf.TX_Data,fmt,ap);
	va_end(ap);
	len = strlen((const char*)U1_Inf.TX_Data);
	while(len--)
	  {
	  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=1); //�ȴ����ͽ���
	  USART_SendData(USART1,U1_Inf.TX_Data[cnt++]);
	  }
}


static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  ָ�����ȵķ����ַ��� **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0; 
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}



//�����ⲿ��ȡ�ڲ�������Ϣ
void ModBus_Communication(void)
{
		
		uint8  i = 0;	
		
		uint16 checksum = 0;
		uint16 Address = 0;

		 
		if(U1_Inf.Recive_Ok_Flag)
			{
				U1_Inf.Recive_Ok_Flag = 0;//������Ŷ
				 //�ر��ж�
				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); 
				 
				checksum  = U1_Inf.RX_Data[U1_Inf.RX_Length - 2] * 256 + U1_Inf.RX_Data[U1_Inf.RX_Length - 1];
				
			//�ϰ�������豸���кŽ����޸�
				if(checksum == ModBusCRC16(U1_Inf.RX_Data,U1_Inf.RX_Length))
					{
						//��ȡ���صĵ�ַ��Ϣ����ʱ�׵�ַΪ254��0x03ָ��   �����ݵ�ַΪ1000��ֻҪ�����������ݣ��ͻ�
						if(U1_Inf.RX_Data[0] == 254 && U1_Inf.RX_Data[1] == 0x03)
							{
								Address = U1_Inf.RX_Data[2] *256+ U1_Inf.RX_Data[3];
								if(Address == 1000)
									{
										U1_Inf.TX_Data[0] = U1_Inf.RX_Data[0];
										U1_Inf.TX_Data[1] = 0x03;// 
										U1_Inf.TX_Data[2] = 2; //���ݳ���Ϊ2�� ��������ı�***

										U1_Inf.TX_Data[3] = 0;
										U1_Inf.TX_Data[4] =  Sys_Admin.ModBus_Address;
										checksum  = ModBusCRC16(U1_Inf.TX_Data,7);   //����������ֽ����ı�
										U1_Inf.TX_Data[5]  = checksum >> 8 ;
										U1_Inf.TX_Data[6]  = checksum & 0x00FF;
										Usart_SendStr_length(USART1,U1_Inf.TX_Data,7);
										
									}
							}
					
						switch (Sys_Admin.Device_Style)
							{
								case 0:
								case 1:
										New_Server_Cmd_Analyse();	
										break;
								case 2:
								case 3:

										NewSHUANG_PIN_Server_Cmd_Analyse();


										break;
								default:
									 

										break;
							}
						
					}
					
			
				
			//�Խ��ջ���������
				for( i = 0; i < 100;i++ )
					U1_Inf.RX_Data[i] = 0x00;
			
			//���¿����ж�
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
				
			}
}








uint16 ModBusCRC16(unsigned char *ptr,uint8 size)
{
		uint16 a,b,temp;
		uint16 CRC16;
		uint16 checksum ;

		CRC16 = 0XFFFF;	

		for(a = 0; a < (size -2) ; a++ )
			{
				CRC16 = *ptr ^ CRC16;
				for(b = 0;b < 8;b++)
					{
						temp = CRC16 & 0X0001;
						CRC16 = CRC16 >> 1;
						if(temp)
							CRC16 = CRC16 ^ 0XA001;	
					}

				*ptr++;
			}

		checksum = ((CRC16 & 0x00FF) << 8) |((CRC16 & 0XFF00) >> 8);


		return checksum;
}

uint16 Lcd_CRC16(unsigned char *ptr,uint8 size)
{
		uint16 a,b,temp;
		uint16 CRC16;
		uint16 checksum ;

		CRC16 = 0XFFFF;	

		
		for(a = 0; a < size ; a++ )
			{
				CRC16 = *ptr ^ CRC16;
				for(b = 0;b < 8;b++)
					{
						temp = CRC16 & 0X0001;
						CRC16 = CRC16 >> 1;
						if(temp)
							CRC16 = CRC16 ^ 0XA001;	
					}

				*ptr++;
			}

		checksum = ((CRC16 & 0x00FF) << 8) |((CRC16 & 0XFF00) >> 8);


		return checksum;
}



void RTU_Server_Cmd_Analyse(void)
{
   
}

void RTU_Mcu_mail_Wifi(void)
{
   
	

}


