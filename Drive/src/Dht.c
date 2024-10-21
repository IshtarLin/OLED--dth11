#include "Dht.h"

float Get_humi = 10, Get_tem = 10;
u8 Right_dat[8]; //正确数据存储
/// @brief 初始化dht11的引脚
/// @param 空
void Dht_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode = GPIO_OUT_OD;//dht11要求开漏,这里小灯测试时改为了上拉
    GPIO_InitStructure.Pin = GPIO_Pin_1;
    GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);
    Delay_ms(1000);
    P11 = 1;
}
/// @brief dht11起始信号
/// @param 空 
void Dht_start(void)
{   
    P11 = 1;
    Delay_us(1);
    P11 = 0;
    Delay_ms(20);//拉低延迟
    P11 = 1;
    Delay_us(30);//拉高延迟
}
/// @brief 主机从dht11接收一个字节
/// @param 空
/// @return 接收到的字节
u8 Dht_recivebyte(void)
{
    u8 dat=0,i,j=0;
    for(i = 0 ; i < 8 ; i++)//接收八位
    {
        while (P11)//等待低电平,dht11的回应信号
        {
            Delay_us(1);
            if(++j==255) return 0; 
        }
        while (!P11)//固定的50us低电平
        {
            Delay_us(1);
            if(++j==255) return 0; 
        }
        
        Delay_us(25);//延迟50us,超出dht11 0数据的26~28us 还为高则数据为1
        dat<<=1;//移位，直接×2同理
        if(P11 == 1)//不是1不操作。
        {dat += 1;}

        while (P11)//等待数据线变低，以便接收下一个字节
        {
            Delay_us(1);
            if(++j==255) return 0; 
        }
    }
    return dat;
}
/// @brief 40位数据的接收
/// @param  空
void Dht_recive(void)
{
    u8 Humi_High,Humi_low,Tem_High,Tem_low,verify,HumiH,HumiL,TemH,TemL;
    u8 j=0;
    Dht_start(); //发送起始信号

    if(P11 == 0)
    {
        while (P11 == 0) //80us的应答信号
        {
            Delay_us(1);
            if(++j==255) return; 
        }
        Delay_us(60);//高电平输出80us通知外设准备接收数据
        
        Humi_High = Dht_recivebyte();
        Humi_low = Dht_recivebyte();
        Tem_High = Dht_recivebyte();
        Tem_low = Dht_recivebyte();
        verify = Dht_recivebyte();

        Delay_us(30);  //50us的低电平作为结束信号

        if(Humi_High + Humi_low + Tem_High + Tem_low == verify) //校验位判断
        {
            HumiH = Humi_High;
            HumiL = Humi_low;
            TemH = Tem_High;
            TemL = Tem_low;
        }
        Right_dat[0] = HumiH; 
        Right_dat[1] = HumiL;
        Right_dat[2] = TemH;
        Right_dat[3] = TemL;

        
    }
}

void Dht_recivedat(float *humi ,float *tem)
{
    float temp = 0;
    Dht_recive();
    temp = Right_dat[1];
    temp += Right_dat[0];
    *humi = temp;
    temp = Right_dat[3];
    temp += Right_dat[2];
    *tem = temp;
    
}

void Task_Dht11(void)
{
    Dht_recivedat(&Get_humi,&Get_tem);
}

