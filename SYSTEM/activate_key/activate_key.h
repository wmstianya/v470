#ifndef __ACTIVATE_KEY_H
#define	__ACTIVATE_KEY_H

#include "main.h"






#define PRODUCT_ZERO_NUMBER  (100)  //�ò�Ʒϵ��0--100Ϊ����ID���ӵ�101��ʼ����

#define PRODUCT_BEGIN_NUMBER  (190110001)  //201��301��ʾ�˴β�Ʒ������ʼ����

#define PRODUCT_END_NUMBER  (190111000)  //300��400��ʾ�˴β�Ʒ������������
//�޶��������Σ�ÿ��50̨������65500̨

#define PRODUCT_DEFINE  (10003)



#define  ACTIVATE_DATA  0xA6A6

#define  TEST_KEY_COUNT_ADDRESS  (0x08000000+114*1024)  //������������������ַ


#define  CPUID_ENCRYPT_ADDRESS  (0x08000000+116*1024)

#define  LOGIN_KEY_HEAD_ADDRESS  (0x08000000+126*1024) //�����ݱ����ڵ�121K��ʼ�ĵط�

//#define  KEY_ONE_ADDRESS   (LOGIN_KEY_HEAD_ADDRESS)  //�ջ�����ǰ 2byte
//#define  KEY_TWO_ADDRESS   (LOGIN_KEY_HEAD_ADDRESS+4)
//#define  KEY_THREE_ADDRESS   (LOGIN_KEY_HEAD_ADDRESS+8)
//#define  KEY_FOUR_ADDRESS   (LOGIN_KEY_HEAD_ADDRESS+12)//�ջ�������� 2byte

//#define  CHECK_KEY_ADDRESS (LOGIN_KEY_HEAD_ADDRESS+16)   //���ڴ洢�Ƿ�õ����������Ϣ

#define  LOGIN_OK_ADDRESS (LOGIN_KEY_HEAD_ADDRESS+20)  //������ɹ����������д��һ��ֵ



typedef struct p_data{
	uint16	product_zero_number ;
	uint16  product_begin_number;
	uint16  product_end_number ;
	}Data_Product;



typedef struct _KEY_STD
{
		uint16  key1_buff;
		uint16  key2_buff;
		uint16  key3_buff;
		uint16  key4_buff;

}Key_Struct;//���ڴ����û�����ļ�����


extern volatile  uint32 CPUIDEncrypt;


extern Data_Product Product_Inf ;//��������ʱ�������������к�ʹ�õ���Ϣ�ṹ��



extern Key_Struct login_key; 





void  Write_First_Flash(void);

void send_product_information(void);
void send_product_name(void);
void send_system_version(void);




void Write_CPU_ID_Encrypt(void);//CPU ID���ܳ���


uint8 Judge_ID_Encrypt(void);//�ж�ID�����Ƿ���ȷ

void Flash_Read_Protect(void);//�ڲ�Flash����������

void Flash_Read_Disable(void);//�ڲ�flash���������
void  Write_Test_Key_Flash(uint8 data);//���ڼ�¼�������Ѿ�ʹ�õĴ���


#endif



