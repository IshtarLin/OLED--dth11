#ifndef __DHT_H_
#define __DHT_H_
#include "Config.h"
#include "GPIO.h"
void Dht_Init(void);
void Dht_start(void);
u8 Dht_recivebyte(void);

#endif