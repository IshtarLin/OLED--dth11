#include "CK.h"

#define CK_Bti          115200  
#define CK_FOSC         11059200 
#define CK_IntStrLen    20       
#define CK_FloatStrLen  20       
#define CK_Bufferlength 16       
#define CK_BRT          (65536 - CK_FOSC / CK_Bti / 4)
unsigned char temp;
bit CK_busy;                           
char CK_rjs;                          
char CK_ryjs;                          
char CK_buffer[CK_Bufferlength] = {9};


void CK_Init()
{
    SCON = 0x50;        
    TMOD = 0x00;        
    TL1  = CK_BRT;      
    TH1  = CK_BRT >> 8; 
    TR1  = 1;           
    AUXR = 0x40;        

    ES = 1; 
    EA = 1; 

    CK_rjs  = 0x00;
    CK_ryjs = 0x00;
    CK_busy = 0;
}

void CK_SendStr(char *p)
{
    while (*p) {
        CK_SendByte(*p++);
    }
}

void CK_SendInt(int num)
{
    char str[CK_IntStrLen];
    sprintf(str, "%d", num);
    CK_SendStr(str);
}

void CK_SendFloat(float num)
{
    char str[CK_FloatStrLen];
    sprintf(str, "%.3f", num);
    CK_SendStr(str);
}

void CK_SendByte(char Data)
{
    while (CK_busy);
    CK_busy = 1;
    SBUF    = Data;
}

// void CK_Send(unsigned char CKdata[],unsigned int CKdata_len)
// {
// 	int i=0;
// 	for( i; i < CKdata_len; i++)
// 	{
// 		CK_SendByte(&CKdata[i]);
// 	}
// }

void CK_Read()
{
    static unsigned char temp = 0;
    if (CK_ryjs != CK_rjs) {
        CK_SendByte(CK_buffer[CK_ryjs++]);
        CK_ryjs &= 0x0f;
    }
    if (temp != CK_ryjs) {
        OLED_ShowString(0, 6, &CK_buffer[temp]);
        temp = CK_ryjs;
    }
}

void CK_SendBack()
{
    static unsigned char temp = 0;
    int X                     = 0;
    for (; CK_ryjs != CK_rjs; CK_ryjs++) {
        CK_SendByte(CK_buffer[CK_ryjs]);
        CK_ryjs &= 0x0f;
    }
    for (; temp != CK_ryjs; temp++) {

        OLED_ShowChar(X, 6, CK_buffer[temp]);
        X = X + 8;
    }
    // if (temp !=CK_ryjs)
    // {
    // 	OLED_ShowString(0,6,&CK_buffer[temp]);
    // 	temp = CK_ryjs;
    // }
}

void UartIsr() interrupt 4 using 1
{
    if (TI) {

        CK_busy = 0;
        TI      = 0;
    }
    if (RI) {

        if (CK_rjs >= CK_Bufferlength) {
            CK_rjs = 0;
        }
        CK_buffer[CK_rjs++] = SBUF;
        RI                  = 0;
    }
}
void Task_OLED(void)
{
    CK_SendBack();
}
// char CK_rjs;			
// char CK_ryjs;	