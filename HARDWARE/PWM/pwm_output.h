#ifndef __PWM_OUTPUT_H
#define	__PWM_OUTPUT_H

#include "stm32f10x.h"




/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1

// ���벶���ܲ��񵽵���С��Ƶ��Ϊ 72M/{ (ARR+1)*(PSC+1) }
#define            ADVANCE_TIM_PERIOD            (1000-1)
#define            ADVANCE_TIM_PSC               (72-1)

// �ж���غ궨��
#define            ADVANCE_TIM_IRQ               TIM1_CC_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_CC_IRQHandler

// TIM1 ���벶��ͨ��1
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

#define            ADVANCE_TIM_IC1PWM_CHANNEL    TIM_Channel_1
#define            ADVANCE_TIM_IC2PWM_CHANNEL    TIM_Channel_2


extern uint16 CCR1_Val;
extern uint16 CCR2_Val;

extern const uint16 Frequence_PWM;//���Ƶ��2Khz ռ�ձȿɵ�

void TIM3_PWM_Init(void);
void PWM_Adjust(uint8 value);







#endif /* __PWM_OUTPUT_H */


