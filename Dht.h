#ifndef __DHT_H_
#define __DHT_H_
#include "Config.h"
#include "GPIO.h"



void Dht_Init(void);
void Dht_start(void);
u8 Dht_recivebyte(void);
void Dht_recive(void);
void Dht_recivedat(float *humi ,float *tem);
#endif