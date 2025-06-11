#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"
#include "main.h"

#define N 50    //����50��
#define M 4     //ͨ����Ϊ3



#define ABS(X)  ((X)>0? (X): -(X))


// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA


/*******************�¶�ADC����**************************/

#define    ADC_GPIO_APBxClock_FUN1        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK1                 RCC_APB2Periph_GPIOC  
#define    ADC_PORT1                      GPIOC






/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ


/* �������IO�ĺ� */





// ת��ͨ������
#define    NOFCHANEL	 4

#define    ADC_PIN1                      GPIO_Pin_0
#define    ADC_CHANNEL1                  ADC_Channel_0

#define    ADC_PIN2                      GPIO_Pin_1
#define    ADC_CHANNEL2                  ADC_Channel_1



#define    ADC_PIN3                      GPIO_Pin_0
#define    ADC_CHANNEL3                  ADC_Channel_10

#define    ADC_PIN4                      GPIO_Pin_1
#define    ADC_CHANNEL4                 ADC_Channel_11




// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


//��·���½ṹ��
typedef struct _TEMPURE_
{
		  uint16 Main_Tem ;
		  uint16 Out_Water_Tem ;
		  uint16 Back_Water_Tem ;
		  uint16 Smoke_Tem ;
			uint16 Pressure_Value;//ϵͳѹ��
			uint16 steam_value; //�����¶�
			uint16 inside_water_value;  //�ڲ�ˮѭ�����¶�
			uint16 Inside_High_Pressure;  //�ڲ�
}TEM_VALUE;

extern TEM_VALUE Temperature_Data;

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[N][M];

extern float ADC_ConvertedValueLocal[NOFCHANEL];
extern	uint16 ADC_Convert_true[NOFCHANEL];



extern uint16 Main_Tem ;
extern uint16 Sub_Tem ;
extern uint16 Back_Water_Tem ;
extern uint16 Smoke_Tem ;


/**************************��������********************************/
void 	ADCx_Init(void);
void filter(void);
uint16 GetVolt(uint16 advalue);
uint16 Pressure25KG_GetVolt(uint16 advalue) ;

uint8  ADC_Process(void);
void Send_Adc_Lcd(void);
uint16 get_dot(uint16 value,uint16 percent);
uint8 pressure_to_temperature(uint8 pressure);

uint8 Check_ADS_Unconnect(uint16 New_Value);

uint16 find_true_temperature(uint16 value);

uint16 Adc_to_temperature(uint16 value);
uint16 go_00_process(uint16 value);

uint16 go_10_process(uint16 value);

uint16 go_20_process(uint16 value);
uint16 go_30_process(uint16 value);
uint16 go_40_process(uint16 value);
uint16 go_50_process(uint16 value);
uint16 go_60_process(uint16 value);
uint16 go_70_process(uint16 value);
uint16 go_80_process(uint16 value);
uint16 go_90_process(uint16 value);
uint16 go_100_process(uint16 value);
uint16 go_110_process(uint16 value);
uint16 go_120_process(uint16 value);
uint16 go_130_process(uint16 value);
uint16 go_140_process(uint16 value);
uint16 go_150_process(uint16 value);
uint16 go_160_process(uint16 value);
uint16 go_170_process(uint16 value);
uint16 go_180_process(uint16 value);
uint16 go_190_process(uint16 value);
uint16 go_200_process(uint16 value);
uint16 go_210_process(uint16 value);



#endif /* __ADC_H */


