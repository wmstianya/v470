#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ϵͳ�жϷ������û�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/10
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************  
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}





/**
  * @brief  ����PVD.
  * @param  None
  * @retval None
  */
void PVD_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /*ʹ�� PWR ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* ʹ�� PVD �ж� */
  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
      
  /* ���� EXTI16��(PVD ���) �����������½����ж�*/
  EXTI_ClearITPendingBit(EXTI_Line16);
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* ����PVD����PWR_PVDLevel_2V6 (PVD����ѹ����ֵΪ2.6V��VDD��ѹ����2.6Vʱ����PVD�ж�) */
	/*���弶������Լ���ʵ��Ӧ��Ҫ������*/
  PWR_PVDLevelConfig(PWR_PVDLevel_2V6);

  /* ʹ��PVD��� */
  PWR_PVDCmd(ENABLE);
}

