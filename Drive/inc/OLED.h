#ifndef __OLED_H__
#define __OLED_H__

#include "Config.h"
#include "stdio.h"

void init_IO();
void init_IIC();
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr);
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr);
void OLED_ShowNum(unsigned char x, unsigned char y, float num);
#endif
