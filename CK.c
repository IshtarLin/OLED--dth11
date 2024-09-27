#include <STC8G.H>
#include <CK.H>
#include "OLED.h"
#include <stdio.H>
#include "intrins.h"
//配置工作环境
#define CK_Bti 	115200    	//配置串口波特率
#define	CK_FOSC  11059200	//单片机运行频率
#define CK_IntStrLen 20		//整型上限个数
#define CK_FloatStrLen 20	//浮点型上限个数
#define CK_Bufferlength 16  //缓冲区大小
#define CK_BRT  (65536 - CK_FOSC/ CK_Bti /4)
unsigned char temp;
bit CK_busy;			//繁忙标志
char CK_rjs;			//接收缓冲计数
char CK_ryjs;			//接收阅读计数
char CK_buffer[CK_Bufferlength];		//接收缓冲

//串口初始化
void CK_Init()
{
	SCON=0x50;		//串口1控制寄存器			//串口1工作模式_模式1:可变波特率8位数据方式 and 允许串口接收数据
	TMOD=0x00;		//定时器模式寄存器		//定时器/计数器1工作模式:16位自动重载模式
	TL1=CK_BRT;		//定时器1计数寄存器（低位）
	TH1=CK_BRT>>8;	//定时器1计数寄存器（高位）
	TR1=1;			//定时器T1的运行控制位	//开始计数
	AUXR=0x40;		//辅助寄存器1			//定时器1速度控制位_CPU时钟不分频分频(FOSC/1)
	
	ES=1;			//串口1中断开关			//打开
	EA=1;			//中断总开关				//打开
	
	CK_rjs=0x00;  		
	CK_ryjs=0x00;
	CK_busy=0;
}




//发送字符串
void CK_SendStr(char *p)
{
	while(*p)
	{	
		CK_SendByte(*p++);
	}
}


//发送整型
void CK_SendInt(int num)
{
	char str[CK_IntStrLen];
	sprintf(str,"%d",num);
	CK_SendStr(str);	
}



//发送浮点型
void CK_SendFloat(float num)
{
	char str[CK_FloatStrLen];
	sprintf(str,"%.3f",num);
	CK_SendStr(str);	
}



//发送字节
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

//电脑发送数据给单片机又发回给电脑
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
//串口1中断事件  分配寄存器组1
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
// char CK_rjs;			//接收缓冲计数
// char CK_ryjs;	//阅读缓冲计数