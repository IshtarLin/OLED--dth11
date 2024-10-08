#include "Dht.h"
#include "Delay.h"

/// @brief 初始化dht11的引脚
/// @param 空
void Dht_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode = GPIO_OUT_OD;//dht11要求开漏,这里小灯测试时改为了上拉
    GPIO_InitStructure.Pin = GPIO_Pin_1;
    GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);
}
/// @brief dht11起始信号
/// @param 空 
void Dht_start(void)
{
    P11 = 0;
    Delay_ms(25);//拉低延迟
    P11 = 1;
    Delay_us(30);//拉高延迟
}
/// @brief 主机从dht11接收一个字节
/// @param 空
/// @return 接收到的字节
u8 Dht_recivebyte(void)
{
    u8 dat=0,i;
    for(i = 0 ; i < 8 ; i++)//接收八位
    {
        while (P11)//等待低电平,dht11的回应信号
        {
            Delay_us(1);
        }
        while (!P11)//固定的50us低电平
        {
            Delay_us(1);
        }
        
        Delay_us(50);//延迟50us,超出dht11 0数据的26~28us 还为高则数据为1
        dat <<= 1;//移位，直接×2同理
        if(P11 == 1)//不是1不操作。
        dat += 1;

        while (P11)//等待数据线变低，以便接收下一个字节
        {
            Delay_us(1);
        }
    }
    return dat;
}


