#ifndef PUBLIC_H
#define PUBLIC_H
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stm32f10x.h"


extern uint8_t wav8[];

extern uint8_t trg;
extern uint8_t cont;
extern uint16_t adc_val;
extern uint32_t rtc_val;
extern volatile uint32_t cnt_ms;
extern bool flag_sec;               //ÃëÖÐ¶Ï±êÖ¾
extern bool volume_modify;


#define LED_ON()        GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LED_OFF()       GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LED_TRIG()      (GPIOB->ODR ^= GPIO_Pin_5)
#define SOUND_ON()      GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define SOUND_OFF()     GPIO_ResetBits(GPIOB,GPIO_Pin_0)


void periph_init(void);
void delay_us(uint16_t n_us);
void delay_ms(uint16_t n_ms);
void key_scan(void);
void fatfs_test(void);

#endif //~PUBLIC_H
