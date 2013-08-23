#ifndef TIM_H
#define TIM_H
#include "public.h"


void tim1_init(uint16_t arr, uint16_t psc);
void tim3_pwm_init(uint16_t arr, uint16_t psc);
void tim6_init(uint16_t arr, uint16_t psc);
void tim_init(void);


#endif //~TIM_H
