#include "Config.h"
#include "OLED_CONT.h"
#include "stdio.h"

#define OLED_CMD 0 //写命令
#define OLED_DATA 1 //写数据
#define Max_Column  128
#define SIZE 16
void init_IO()

{

   RSTCFG=0x50;  //开启RST键进入ISP模式

   P0M1 = 0x00;   P0M0 = 0x00;   //设置P0口为准双向口

   P2M1 = 0x00;   P2M0 = 0x00;   //设置P1口为准双向口

   P3M1 = 0x00;   P3M0 = 0x00;   //设置P3口为准双向口

}
void init_IIC()
{

   P_SW2|=0x10;//p24、25

   I2CCFG=0xe0;

}

void IIC_wait()
{

   while(!(I2CMSST&0x40));//等待

   I2CMSST&=~0x40;//软清零

}

void IIC_START()
{

   I2CMSCR=0x01;

   IIC_wait();

}

void IIC_SendData(u8 dat)
{

   I2CTXD=dat;

   I2CMSCR=0x02;
   
   IIC_wait();

}

void IIC_RevAck()
{

   I2CMSCR=0x03;

   IIC_wait();

}

void IIC_STOP()

{

   I2CMSCR=0x06;

   IIC_wait();

}

//cmd:数据/命令标志 0：表示命令;1：表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)

{  

   IIC_START();

   IIC_SendData(0x78);//OLED屏地址码0x78

   IIC_RevAck();

   if(cmd==0)
   {

      IIC_SendData(0x00);

      IIC_RevAck();

   }

   else
   {

      IIC_SendData(0x40);

      IIC_RevAck();

   }

   IIC_SendData(dat);

   IIC_RevAck();

   IIC_STOP();

}


/*下面的抄的网络资源*/
//设置显示坐标位置
void OLED_Set_Pos(u8 x, u8 y)
{
   OLED_WR_Byte(0xb0+y,OLED_CMD);

   OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);

   OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}       

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!    
void OLED_Clear(void) 
{ 

   u8 i,n;       

   for(i=0;i<8;i++) 

   { 

      OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）

      OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址

      OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址  

      for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);

   } //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127

//y:0~63

//mode:0,反白显示;1,正常显示        

//size:选择字体 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{       

   unsigned char c=0,i=0;

   c=chr-' ';//得到偏移后的值       

   if(x>Max_Column-1){x=0;y=y+2;}

   if(SIZE==16)

   {

      OLED_Set_Pos(x,y); 

      for(i=0;i<8;i++)

      OLED_WR_Byte(OLED_F8x16[c*16+i],OLED_DATA);

      OLED_Set_Pos(x,y+1);

      for(i=0;i<8;i++)

      OLED_WR_Byte(OLED_F8x16[c*16+i+8],OLED_DATA);

   }

    /*这里有SIZE==8的，我直接删了*/
}

//显示一个字符号串

void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
   u8 j=0;
   while (chr[j]!='\0'){   
      OLED_ShowChar(x,y,chr[j]);
      x+=8;
      if(x>120){x=0;y+=2;}
      j++;
   }
}

//初始化SSD1306                


void OLED_ShowNum(u8 x, u8 y, float num)
{
   char str[100] = "";


   sprintf(str, "%.3f", num);

   


   OLED_ShowString(x, y, str);


}






void OLED_Init(void)

{

   OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel

   OLED_WR_Byte(0x00,OLED_CMD);//---set low column address

   OLED_WR_Byte(0x10,OLED_CMD);//---set high column address

   OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)

   OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register

   OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness

   OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常

   OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常

   OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display

   OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)

   OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty

   OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)

   OLED_WR_Byte(0x00,OLED_CMD);//-not offset

   OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency

   OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec

   OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period

   OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

   OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration

   OLED_WR_Byte(0x12,OLED_CMD);

   OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh

   OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level

   OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)

   OLED_WR_Byte(0x02,OLED_CMD);//

   OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable

   OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable

   OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)

   OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)

   OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

   OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/

   OLED_Clear();

   OLED_Set_Pos(0,0);

} 

