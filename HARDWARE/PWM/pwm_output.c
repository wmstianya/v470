/**********************************************
 * �ļ���  ��pwm_output.c
 * ����    ��         
 *  
 * Ӳ�����ӣ�---------------------
 *          |  PA.6: (TIM3_CH1)  |
 *           			
**********************************************************************************/
#include "main.h"


 const uint16 Frequence_PWM = 500; //���Ƶ��2Khz ռ�ձȿɵ�

 uint16 CCR1_Val = 495;  //       
 uint16 CCR2_Val = 400;  
/*
 * ��������TIM5_GPIO_Config
 * ����  ������TIM5�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;	 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	/* TIM3ʱ��ʹ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); /* GPIOB ʱ��ʹ�� */

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    //  PB1 ��ΪPWMʹ�����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


}

/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */

	
/* -----------------------------------------------------------------------
		�������¹�ʽ����
    Frequency= TIMxCLK/( TIM_Prescaler +1)/ (TIM_Period + 1)
    TIMx Channelx duty cycle = (TIMx_Pulse/ ( TIM_Prescaler +1))* 100%
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = TIM3CLK/(Prescaler+1)=72M
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 72 KHz.
    TIM3 Channel1 duty cycle = 50%
   
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 500;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	

  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
 

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

/*
 * ��������TIM5_PWM_Init
 * ����  ��TIM5 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM5���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIM3_PWM_Init(void)
{
	TIM3_GPIO_Config();	//TIM3��ʱ��IO������
	TIM3_Mode_Config();	//TIM3��ʱ��ģʽ����
}



void PWM_Adjust(uint8 value)
{
	//ע��ͨ��4 CRR���������Ҫ�ĳ�4
	if(value > 100)
		value = 100;
	
	TIM3->CCR4 = (Frequence_PWM + 1) - (5 * value);
		
	sys_data.Data_1FH = value;
	LCD10D.DLCD.Air_Power = value;
	now_percent =value;

	//ȡ������������е�ʱ�䣬Ȼ����������ڵ������еĴ�ɨʱ�����
	 
}






