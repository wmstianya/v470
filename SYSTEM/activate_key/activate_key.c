

#include"main.h"


 Key_Struct login_key;//���ڴ����û�����ļ�����
	
//Data_Product Product_Inf ;//��������ʱ�������������к�ʹ�õ���Ϣ�ṹ��



volatile  uint32 CPUIDEncrypt = 0xFFFFFFFF;//ID����



//��Ʒ�ͺ�

const unsigned char product_name[15] ={"PM-S8-1.0ZQ"};

//ϵͳ�汾��
const unsigned char system_version[10] ={"PMS8_3.3.1"};  /*2021��8��31��10:40:27*/








//�����룺1234,4321,5678,8765����������������ʹ�ã����߱�WIFI�����߱����£�ֻ�����е��Ȼ��Ȼ������Ϩ��
const unsigned char Test_key[8] = {12, 34, 43, 21, 56, 78, 87, 65,};










void send_product_information(void)
{
	
	
	SysTick_Delay_ms(1);
	 send_product_name();
	 SysTick_Delay_ms(1);
	 send_system_version();
	 SysTick_Delay_ms(1);
	
		
}

//���Ͳ�Ʒ�ͺ���Ϣ��LCD�ϣ��ڰ���������ʾ
void send_product_name(void)
{
	uint8 temp = 0;
	
		send_byte(0x5A);
		send_byte(0XA5);
		send_byte(3+2*sizeof(product_name));//�ַ�����
		send_byte(0X82);//д������
		send_byte(0X03);
		send_byte(0X90);//��ʼ��ַ

		for(temp = 0; temp < sizeof(product_name);temp++ )
			{
				send_byte(0X00);
				send_byte(product_name[temp]);
			}
		
}

void send_system_version(void)
{
	uint8 temp = 0;
		
		send_byte(0x5A);
		send_byte(0XA5);
		send_byte(3+2*sizeof(system_version));//�ַ�����
		send_byte(0X82);//д������
		send_byte(0X03);
		send_byte(0X60);//��ʼ��ַ

		for(temp = 0; temp < sizeof(system_version);temp++ )
			{
				send_byte(0X00);
				send_byte(system_version[temp]);
			}

		
}






void  Write_First_Flash(void)
{
	FLASH_Unlock();  //�ڲ�FLASH����
	FLASH_ErasePage(LOGIN_KEY_HEAD_ADDRESS);
	FLASH_ProgramWord(LOGIN_OK_ADDRESS,ACTIVATE_DATA);//��ֹ�ڶ���д��
	FLASH_Lock();
}


void  Write_Test_Key_Flash(uint8 data)
{
	FLASH_Unlock();  //�ڲ�FLASH����
	FLASH_ErasePage(TEST_KEY_COUNT_ADDRESS);
	FLASH_ProgramWord(TEST_KEY_COUNT_ADDRESS,data);//��ֹ�ڶ���д��
	FLASH_Lock();
}




void Write_CPUID_Flash(uint32 data)
{
	FLASH_Unlock();  //�ڲ�FLASH����
	FLASH_ErasePage(CPUID_ENCRYPT_ADDRESS);
	FLASH_ProgramWord(CPUID_ENCRYPT_ADDRESS,data);//��ֹ�ڶ���д��
	FLASH_Lock();
}


void Write_CPU_ID_Encrypt(void)
{
	 
	uint32 CPUID[3];
	uint32 EncryptCode = 0;
	
	CPUIDEncrypt  = *(uint32 *)(CPUID_ENCRYPT_ADDRESS);
	if(CPUIDEncrypt ==0xFFFFFFFF)
		{
			CPUID[0] = *(__IO u32 *)(0X1FFFF7F0); // �ߵ�ַ
    		CPUID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
    		CPUID[2] = *(__IO u32 *)(0X1FFFF7E8); // �͵�ַ
    		EncryptCode = (CPUID[0]>>3)+(CPUID[1]>>1)+(CPUID[2]>>2);
			Write_CPUID_Flash(EncryptCode);

			if(*(uint32 *)(CPUID_ENCRYPT_ADDRESS) == EncryptCode)
				printf("\r\nCPU ID ���ܳɹ�������");
		}
}


uint8 Judge_ID_Encrypt(void)
{
	uint32 CPUID[4];

	CPUID[0] = *(__IO u32 *)(0X1FFFF7F0); // �ߵ�ַ
    CPUID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
    CPUID[2] = *(__IO u32 *)(0X1FFFF7E8); // �͵�ַ

	CPUID[3]= (CPUID[0]>>3)+(CPUID[1]>>1)+(CPUID[2]>>2);
	CPUIDEncrypt  = *(uint32 *)(CPUID_ENCRYPT_ADDRESS);

	return(CPUIDEncrypt == CPUID[3]);
	
}



void Flash_Read_Protect(void)
{
	 if(FLASH_GetReadOutProtectionStatus()!=SET)
    { 
        FLASH_Unlock(); //������FALSHҲ�����ö����� 
        FLASH_ReadOutProtection(ENABLE);
        FLASH_Lock();//����
    } 
}


void Flash_Read_Disable(void)
{
	if(FLASH_GetReadOutProtectionStatus()!=RESET)
    { 
     FLASH_Unlock(); 
     FLASH_ReadOutProtection(DISABLE);
     FLASH_Lock(); 
    }
}





