#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"
#include <stdio.h>

//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
//#define  sFLASH_ID              0XEF4018   //W25Q128
#define  sFLASH_ID              0XEF4017    //W25Q64

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/*�����-��ͷ*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

/* WIP(busy)��־��FLASH�ڲ�����д�� */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
/*�����-��β*******************************/


/*SPI�ӿڶ���-��ͷ****************************/
#define      FLASH_SPIx                        SPI3
#define      FLASH_SPI_APBxClock_FUN          RCC_APB1PeriphClockCmd
#define      FLASH_SPI_CLK                     RCC_APB1Periph_SPI3

//CS(NSS)���� Ƭѡѡ��ͨGPIO����
#define      FLASH_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CS_CLK                  RCC_APB2Periph_GPIOB    
#define      FLASH_SPI_CS_PORT                 GPIOB
#define      FLASH_SPI_CS_PIN                  GPIO_Pin_6

//SCK����
#define      FLASH_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      FLASH_SPI_SCK_CLK                 RCC_APB2Periph_GPIOB  
#define      FLASH_SPI_SCK_PORT                GPIOB   
#define      FLASH_SPI_SCK_PIN                 GPIO_Pin_3
//MISO����
#define      FLASH_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MISO_CLK                RCC_APB2Periph_GPIOB    
#define      FLASH_SPI_MISO_PORT               GPIOB 
#define      FLASH_SPI_MISO_PIN                GPIO_Pin_4
//MOSI����
#define      FLASH_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MOSI_CLK                RCC_APB2Periph_GPIOB   
#define      FLASH_SPI_MOSI_PORT               GPIOB 
#define      FLASH_SPI_MOSI_PIN                GPIO_Pin_5

#define  		SPI_FLASH_CS_LOW()     						GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define  		SPI_FLASH_CS_HIGH()    						GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )

/*SPI�ӿڶ���-��β****************************/

/*�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*��Ϣ���*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)





typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

#define countof(a)      (sizeof(a) / sizeof(*(a)))


#define  FLASH_WriteAddress     0x10000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress


#define  SPI_ERROR_DATA_ADDRESS 0X30000
#define  FLASH_Min_WriteAddress 0x20000  //�涨������ݴ洢�ĵ�ַ
#define  FLASH_PAGE_Save_Address 0x10000


typedef struct _FLASH_1_ //���ڴ洢��Ϣ�����ݸ�ʽ��λ��
{
		__IO uint32_t Buffer_Data;
		__IO uint32_t ProgramTimes;	
		__IO uint32_t sys_time;
		__IO uint32_t small_time;
		__IO uint32_t big_time;
		__IO uint32_t ignition_times;
		__IO uint32_t target_value; 
		
		__IO uint32_t Last_Max_Smoke_Value; //�ϴ���������¶�
		
		__IO uint32_t Start_End_Time_Min; //�ϴ�����ʱ��֮����
		__IO uint32_t Start_End_Time_Hour; //�ϴ�����ʱ��֮Сʱ
		
		__IO uint32_t clean_Year; //�ϴγ���  ��
		__IO uint32_t clean_Month; //�ϴγ�����
		__IO uint32_t clean_Day; //�ϴγ���  ��

		__IO uint32_t Min_Supply_Time;//����ʱ����̲�ˮ���
		__IO uint32_t Max_Supply_Flow_Speed;//����ʱ��߲�ˮ����
		__IO uint32_t Last_Used_Water_value; //�ϴ�ˮ����ֵ
		__IO uint32_t All_Used_Water_Value;//�ۼ�ˮ������
		__IO uint32_t Water_Scale_Value;  //�ϴ����ж�ˮ��ֵ���ж�
		
		
		
 	  
}OUT_FLASH_BULID;






extern OUT_FLASH_BULID Flash_Data;
extern OUT_FLASH_BULID Flash_Buffer;


void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

TestStatus SPI_FLASH_TEST(void);
void  SPI_FLASH_DATA_UPDATA(void);


TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2, uint16_t BufferLength);
u8 Load_Spi_Flash_Error_Data(void);
u8 Save_Spi_Flash_Error_Data(void);



#endif /* __SPI_FLASH_H */

