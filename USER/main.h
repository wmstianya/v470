#ifndef __MAIN_H
#define __MAIN_H

typedef unsigned char  uint8;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  int8;                    /* �з���8λ���ͱ���                        */
typedef unsigned short uint16;                  /* �޷���16λ���ͱ���                       */
typedef signed   short int16;                   /* �з���16λ���ͱ���                       */
typedef unsigned long   uint32;                  /* �޷���32λ���ͱ���                       */
typedef signed   long   int32;                   /* �з���32λ���ͱ���                       */
typedef float          fp32;                    /* �����ȸ�������32λ���ȣ�                 */
typedef double         fp64;                    /* ˫���ȸ�������64λ���ȣ� 				*/



#define false 0
#define	true  1

#include "ADS1220.h" 

#include "usr_c210.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "stm32f10x_it.h"
#include "usart2.h"
#include "usart3.h"
#include "usart4.h"
#include "usart5.h"

#include "stdarg.h"
#include "bsp_led.h"
#include "bsp_relays.h"
#include "bsp_adc.h"
#include "bsp_rccclkconfig.h"
#include "bsp_systick.h"
#include "bsp_parallel_serial.h" 
#include "system_control.h"
#include "bsp_rtc.h"
#include "bsp_calendar.h"
#include "bsp_date.h"
#include "internal_flash.h"
#include "pwm_output.h"
#include "bsp_iwdg.h" 
#include	"activate_key.h"
#include "bsp_spi_flash.h"




extern const uint16 Soft_Version; 


#endif
