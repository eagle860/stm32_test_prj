#include "systick.h"


void systick_init(uint32_t ticks_per_sec)
{
    RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency / ticks_per_sec);
}
