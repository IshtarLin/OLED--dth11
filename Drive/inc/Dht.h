#ifndef __DHT_H_
#define __DHT_H_

#include "Config.h"
#include "Delay.h"
#include "GPIO.h"

extern u8 Right_dat[];

void Dht_Init(void);
void Dht_start(void);
u8 Dht_recivebyte(void);
void Dht_recive(void);
void Dht_recivedat(float *humi ,float *tem);
void Task_Dht11(void);
#endif