
 
  
#include "bsp_spi_flash.h"
#include "system_control.h"


static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

OUT_FLASH_BULID Flash_Data;
OUT_FLASH_BULID Flash_Buffer;



/**
  * @brief  SPI_FLASH��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;


  
	/* ʹ��SPIʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/* ʹ��SPI������ص�ʱ�� */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
  //SPI_FLASH_SPI pins: SCK--PB3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  //SPI_FLASH_SPI pins: MISO--PB4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  //SPI_FLASH_SPI pins: MOSI--PB5 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

   //SPI_FLASH_SPI_CS_PIN pin: CS---PB6

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  
  //SPI_FLASH_SPI_CS_PIN pin: CS--PA15
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
  SPI_FLASH_CS_HIGH();

  /* SPI ģʽ���� */
  // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
 // SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//��һ���汾Ϊ4��Ƶ
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPIx , &SPI_InitStructure);

  /* ʹ�� SPI  */
  SPI_Cmd(FLASH_SPIx , ENABLE);
	
}
 /**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* �������� */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* ������������ָ��*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*���Ͳ���������ַ�ĸ�λ*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* ���Ͳ���������ַ����λ */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* ���Ͳ���������ַ�ĵ�λ */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_BulkErase(void)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ���� Erase */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* �����������ָ��*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* дҳдָ��*/
  SPI_FLASH_SendByte(W25X_PageProgram);
  /*����д��ַ�ĸ�λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*����д��ַ����λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*����д��ַ�ĵ�λ*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
  }

  /* д������*/
  while (NumByteToWrite--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    SPI_FLASH_SendByte(*pBuffer);
    /* ָ����һ�ֽ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ�д�����*/
  SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    { 
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*��д����ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				
        WriteAddr +=  count;
        pBuffer += count;
				/*��дʣ�������*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			
			/* ���������ظ���ַ�������� */
      WriteAddr +=  count;
      pBuffer += count;
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();

  /* ���� �� ָ�� */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
	/* ��ȡ���� */
  while (NumByteToRead--) /* while there is data to be read */
  {
    /* ��ȡһ���ֽ�*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* ��ʼͨѶ��CS�͵�ƽ */
  SPI_FLASH_CS_LOW();

  /* ����JEDECָ���ȡID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* ��ȡһ���ֽ����� */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

 /* ֹͣͨѶ��CS�ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}
 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
  * @retval FLASH Device ID
  */
u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


 /**
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(FLASH_SPIx , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

 /**
  * @brief  ʹ��SPI���������ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }
	
  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(FLASH_SPIx , HalfWord);

	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
  /* ͨѶ��ʼ��CS�� */
  SPI_FLASH_CS_LOW();

  /* ����дʹ������*/
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /*ͨѶ������CS�� */
  SPI_FLASH_CS_HIGH();
}

/* WIP(busy)��־��FLASH�ڲ�����д�� */
#define WIP_Flag                  0x01

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� ��״̬�Ĵ��� ���� */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  /* ��FLASHæµ����ȴ� */
  do
  {
		/* ��ȡFLASHоƬ��״̬�Ĵ��� */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}


//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  /* ͨѶ��ʼ��CS�� */
  SPI_FLASH_CS_LOW();

  /* ���� ���� ���� */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /*ͨѶ������CS�� */
  SPI_FLASH_CS_HIGH();
}   

//����
void SPI_Flash_WAKEUP(void)   
{
  /*ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� �ϵ� ���� */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

   /* ֹͣ�ź� FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}   
   

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}



/**
  * @brief  ����SPI FLASH �ĺû�
  * @param  None.
  * @retval None.
  */

TestStatus SPI_FLASH_TEST(void)
{
		TestStatus TransferStatus1 = FAILED;
		uint8_t Tx_Buffer[] = "Hello Passmack!\r\n";
		uint8_t Rx_Buffer[20];
		uint8_t size_value = 0;

		size_value = countof(Tx_Buffer) - 1;


	 
		printf("\r\n FlashID is 0x%X\r\n", SPI_FLASH_ReadID());


		SPI_FLASH_SectorErase(0x00000);	 	 
		
		
		SPI_FLASH_BufferWrite(Tx_Buffer, 0x00000, size_value);		
		printf("\r\n д�������Ϊ�� %s \r\t", Tx_Buffer);
		
	
		SPI_FLASH_BufferRead(Rx_Buffer, 0x00000, size_value);
		printf("\r\n ����������Ϊ �� %s \r\n", Rx_Buffer);
		
	
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, size_value);
		
		if( PASSED == TransferStatus1 )
		{ 
			printf("\r\n�ⲿFLASH���Գɹ�!\n\r");
			return PASSED; 
		}
		else
		{        
			printf("\r\n �ⲿFLASH����ʧ��!\n\r");
			return FAILED; 
		}
}


/**
  * @brief  ��FLASH���ݽ��и��£���0x10000��ַΪ��㣬10W�Σ���ַ����0x01000H,�õ�ַ�ı��洢��0x1000H
  * @param 	sys_flag.Flash_Address.
  * @retval None.
  */

void  SPI_FLASH_DATA_UPDATA(void)
{

	//Ҫ��FLASH ��ַ������ͼ�⣬��ֹ�Ժ�FLASH �𻵣���MCUδ�������
	if(sys_flag.Flash_Address < FLASH_Min_WriteAddress)
		{
			sys_flag.Flash_Address = FLASH_Min_WriteAddress;
			Flash_Buffer.Buffer_Data = sys_flag.Flash_Address;
			
			SPI_FLASH_SectorErase(FLASH_PAGE_Save_Address);	
			SPI_FLASH_BufferWrite((uint8 *)&Flash_Buffer, FLASH_PAGE_Save_Address,sizeof(Flash_Buffer));	
		}

	SysTick_Delay_ms(5);
	//��һ���� �������ݽ�Ҫ��ŵĵ�ַ
	SPI_FLASH_BufferRead((uint8 *)&Flash_Buffer, FLASH_PAGE_Save_Address, sizeof(Flash_Buffer));
	sys_flag.Flash_Address = Flash_Buffer.Buffer_Data;

	//�ڶ����� ������ַ�е����ݣ�����Buffer��
	SPI_FLASH_BufferRead((uint8 *)&Flash_Buffer, sys_flag.Flash_Address, sizeof(Flash_Buffer));
	Flash_Data.ProgramTimes = Flash_Buffer.ProgramTimes;
	
	Flash_Data.ProgramTimes++;  //����д���������ۼ� �����մ洢��������
	
	//�������� �жϴ�ŵĵ�ַ��д�����Ƿ�ϸ�  10W�Σ�һ����������
	if(Flash_Data.ProgramTimes > 100000)
		{
			Flash_Data.ProgramTimes  = 0;//�Լ�������
			memset(&Flash_Buffer,0,sizeof(Flash_Buffer));
			sys_flag.Flash_Address = sys_flag.Flash_Address + 0x01000;//��һҳ�������µ�FLASH��
			Flash_Buffer.Buffer_Data = sys_flag.Flash_Address;
			SPI_FLASH_SectorErase(FLASH_PAGE_Save_Address);	
			SPI_FLASH_BufferWrite((uint8 *)&Flash_Buffer, FLASH_PAGE_Save_Address,sizeof(Flash_Buffer));	
		}

	//���Ĳ��� �����ݴ洢
	SPI_FLASH_SectorErase(sys_flag.Flash_Address);
	SPI_FLASH_BufferWrite((uint8 *)&Flash_Data, sys_flag.Flash_Address,sizeof(Flash_Data));	
		
	//���岽�� ��������ȡ��	
	SPI_FLASH_BufferRead((uint8 *)&Flash_Data, sys_flag.Flash_Address, sizeof(Flash_Data));	
	memset(&Flash_Buffer,0,sizeof(Flash_Buffer));	
}


/**
  * @brief  �Ƚ������ַ����Ƿ����
  * @param  None.
  * @retval None.
  */

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}


uint8 Load_Spi_Flash_Error_Data(void)
{
	uint8 sstemp1[6] = {0x5A,0XA5,0X09,0X82,0x00,0X80};
	//��һ������ȡ��ʷ������Ϣ
		if(SPI_FLASH_TEST())
			{
				SPI_FLASH_BufferRead((uint8 *)&SPI_Error_Data, SPI_ERROR_DATA_ADDRESS, sizeof(SPI_Error_Data));
				//�ڶ�������SPI FLASH�е�������Ϣ��ֵ�����еĹ��Ͻṹ�壬��8������
			  Err_Lcd_Memory[0] = SPI_Error_Data.Error1;
			  Err_Lcd_Memory[1] = SPI_Error_Data.Error2;
			  Err_Lcd_Memory[2] = SPI_Error_Data.Error3;
			  Err_Lcd_Memory[3] = SPI_Error_Data.Error4;
			  Err_Lcd_Memory[4] = SPI_Error_Data.Error5;
			  Err_Lcd_Memory[5] = SPI_Error_Data.Error6;
			  Err_Lcd_Memory[6] = SPI_Error_Data.Error7;
			  Err_Lcd_Memory[7] = SPI_Error_Data.Error8;
			}
		else
			{
				//FLASH���Զ�ȡʧ��ʱ�����Ͻ����������
				memset(& Err_Lcd_Memory[0],0,sizeof( Err_Lcd_Memory[0]));
				Err_Lcd_Memory[1] = Err_Lcd_Memory[0];
				Err_Lcd_Memory[2] = Err_Lcd_Memory[0];
				Err_Lcd_Memory[3] = Err_Lcd_Memory[0];
				Err_Lcd_Memory[4] = Err_Lcd_Memory[0];
				Err_Lcd_Memory[5] = Err_Lcd_Memory[0];
				Err_Lcd_Memory[6] = Err_Lcd_Memory[0];
				Err_Lcd_Memory[7] = Err_Lcd_Memory[0];
			}
			

	//������������Ϣ���͸���ʾ��		  	
			  	 sstemp1[5] = 0x80;
				send_str(sstemp1,6);  //���Ͱ�ͷ
				send_str((uint8 *)&Err_Lcd_Memory[0],sizeof(Err_Lcd_Memory[0]));

				sstemp1[5] = 0x83;
				send_str(sstemp1,6);  //���Ͱ�ͷ
				send_str((uint8 *)&Err_Lcd_Memory[1],sizeof(Err_Lcd_Memory[1]));

				sstemp1[5] = 0x86;
				send_str(sstemp1,6);  //���Ͱ�ͷ
				send_str((uint8 *)&Err_Lcd_Memory[2],sizeof(Err_Lcd_Memory[2]));

				sstemp1[5] = 0x89;
				send_str(sstemp1,6);  //���Ͱ�ͷ
				send_str((uint8 *)&Err_Lcd_Memory[3],sizeof(Err_Lcd_Memory[3]));

				sstemp1[5] = 0x8C;
				send_str(sstemp1,6);  //���Ͱ�ͷ
				send_str((uint8 *)&Err_Lcd_Memory[4],sizeof(Err_Lcd_Memory[4]));

				sstemp1[5] = 0x8F;
				send_str(sstemp1,6);  //���Ͱ�ͷ
				send_str((uint8 *)&Err_Lcd_Memory[5],sizeof(Err_Lcd_Memory[5]));

				sstemp1[5] = 0x92;
				send_str(sstemp1,6);  //���Ͱ�ͷ
				send_str((uint8 *)&Err_Lcd_Memory[6],sizeof(Err_Lcd_Memory[6]));

				sstemp1[5] = 0x95;
				send_str(sstemp1,6);  //���Ͱ�ͷ
				send_str((uint8 *)&Err_Lcd_Memory[7],sizeof(Err_Lcd_Memory[7]));

	return 0 ;
}


uint8 Save_Spi_Flash_Error_Data(void)
{
	if(SPI_FLASH_TEST())
		{
			SPI_Error_Data.Error1 = Err_Lcd_Memory[0];
			SPI_Error_Data.Error2 = Err_Lcd_Memory[1];
			SPI_Error_Data.Error3 = Err_Lcd_Memory[2]; 
			SPI_Error_Data.Error4 = Err_Lcd_Memory[3]; 
			SPI_Error_Data.Error5 = Err_Lcd_Memory[4]; 
			SPI_Error_Data.Error6 = Err_Lcd_Memory[5]; 
			SPI_Error_Data.Error7 = Err_Lcd_Memory[6]; 
			SPI_Error_Data.Error8 = Err_Lcd_Memory[7]; 
			SPI_FLASH_SectorErase(SPI_ERROR_DATA_ADDRESS);
			SPI_FLASH_BufferWrite((uint8 *)&SPI_Error_Data, SPI_ERROR_DATA_ADDRESS,sizeof(SPI_Error_Data));	
		}
	return 0 ;
}

/*********************************************END OF FILE**********************/
