#include "Config.h"
#include "CK.h"
#include "Dht.h"
#include "GPIO.h"
#include "Timer.h"

void SYS_INIT(void)
{
    init_IO();
    init_IIC();
    OLED_Init();
    CK_Init();
    Dht_Init();
    Timer0_Init();
}