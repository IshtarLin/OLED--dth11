#include <STC8G.H>
#include <CK.H>
#include "OLED.h"
#include <stdio.H>
#include "intrins.h"
//���ù�������
#define CK_Bti 	115200    	//���ô��ڲ�����
#define	CK_FOSC  11059200	//��Ƭ������Ƶ��
#define CK_IntStrLen 20		//�������޸���
#define CK_FloatStrLen 20	//���������޸���
#define CK_Bufferlength 16  //��������С
#define CK_BRT  (65536 - CK_FOSC/ CK_Bti /4)
unsigned char temp;
bit CK_busy;			//��æ��־
char CK_rjs;			//���ջ������?
char CK_ryjs;			//�����Ķ�����
char CK_buffer[CK_Bufferlength]={9};		//���ջ���

//���ڳ�ʼ��
void CK_Init()
{
	SCON=0x50;		//����1���ƼĴ���			//����1����ģʽ_ģʽ1:�ɱ䲨����8λ���ݷ�ʽ and �������ڽ�������
	TMOD=0x00;		//��ʱ��ģʽ�Ĵ���		//��ʱ��/������1����ģʽ:16λ�Զ�����ģʽ
	TL1=CK_BRT;		//��ʱ��1�����Ĵ�������λ��
	TH1=CK_BRT>>8;	//��ʱ��1�����Ĵ�������λ��
	TR1=1;			//��ʱ��T1�����п���λ	//��ʼ����
	AUXR=0x40;		//�����Ĵ���1			//��ʱ��1�ٶȿ���λ_CPUʱ�Ӳ���Ƶ��Ƶ(FOSC/1)
	
	ES=1;			//����1�жϿ���			//��
	EA=1;			//�ж��ܿ���				//��
	
	CK_rjs=0x00;  		
	CK_ryjs=0x00;
	CK_busy=0;
}




//�����ַ���
void CK_SendStr(char *p)
{
	while(*p)
	{	
		CK_SendByte(*p++);
	}
}


//��������
void CK_SendInt(int num)
{
	char str[CK_IntStrLen];
	sprintf(str,"%d",num);
	CK_SendStr(str);	
}



//���͸�����
void CK_SendFloat(float num)
{
	char str[CK_FloatStrLen];
	sprintf(str,"%.3f",num);
	CK_SendStr(str);	
}



//�����ֽ�
void CK_SendByte(char Data)
{
	while (CK_busy);
	CK_busy = 1;
	SBUF = Data;
}

// void CK_Send(unsigned char CKdata[],unsigned int CKdata_len)
// {
// 	int i=0;
// 	for( i; i < CKdata_len; i++)
// 	{
// 		CK_SendByte(&CKdata[i]);
// 	}
// }

//���Է������ݸ���Ƭ���ַ��ظ�����
void CK_Read()
{
	static unsigned char temp = 0;
	if (CK_ryjs!=CK_rjs)
	{
		CK_SendByte(CK_buffer[CK_ryjs++]);
		CK_ryjs &= 0x0f;
	}
	if (temp !=CK_ryjs)
	{
		OLED_ShowString(0,6,&CK_buffer[temp]);
		temp = CK_ryjs;
	}
	

}
//读取数据发送返�?
void CK_SendBack()
{
	static unsigned char temp = 0;
	int X = 0;
	for(;CK_ryjs!=CK_rjs;CK_ryjs++)
	{	
		CK_SendByte(CK_buffer[CK_ryjs]);
		CK_ryjs &= 0x0f;
	}
	for(;temp!=CK_ryjs;temp++)
	{
		
		OLED_ShowChar(X,6,CK_buffer[temp]);
		X=X+8;
	}
	// if (temp !=CK_ryjs)
	// {
	// 	OLED_ShowString(0,6,&CK_buffer[temp]);
	// 	temp = CK_ryjs;
	// }
	

}
//����1�ж��¼�  ����Ĵ�����?1
void UartIsr() interrupt 4 using 1   
{
	if (TI)
	{
		
		CK_busy = 0;
		TI = 0;
	}
	if (RI)
	{
		
		if(CK_rjs >= CK_Bufferlength)
		{
			CK_rjs = 0;
		}
		CK_buffer[CK_rjs++] = SBUF;
		RI = 0;
	}
}
// char CK_rjs;			//���ջ������?
// char CK_ryjs;	//�Ķ��������?