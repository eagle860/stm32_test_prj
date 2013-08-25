#include "public.h"
#include "driver_conf.h"


uint8_t trg = 0;            //短按
uint8_t cont = 0;           //长按
uint16_t adc_val = 0;
uint32_t rtc_val = 0;
volatile uint32_t cnt_ms = 0;
bool flag_sec = false;
bool volume_modify = true;

//------------------------------------------------------------------------------
void periph_init(void)
{
     gpio_init();
     tim_init();
     usart_init();
     //rtc_init();
     adc_init();
     dac_init();
}
//------------------------------------------------------------------------------
void key_scan(void)
{
    uint8_t key_val = !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    trg = key_val & (key_val ^ cont);
    cont = key_val;
}

//------------------------------------------------------------------------------
//使用systick进行延时，最大支持65535us延时
void delay_us(uint16_t n_us)
{
#ifndef UCOS
    
#else
    SysTick->LOAD = n_us * (SystemCoreClock/8000000);
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    do {} while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
#endif
}
//------------------------------------------------------------------------------
//使用systick进行延时，最大支持延时1864ms
void delay_ms(uint16_t n_ms)
{
#ifndef UCOS
#else
    SysTick->LOAD = n_ms * (SystemCoreClock/8000);
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    do {} while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
#endif
}
//------------------------------------------------------------------------------
#include "ff.h"
#include "diskio.h"
void fatfs_test(void)
{
	FATFS Fatfs;
	FIL fp;
	const uint8_t w_b[10] = {0,1,2,3,4,5,6,7,8,9};
	const char *str = "dir test";
	uint8_t r_b[10];
	uint32_t w_n, r_n;

	f_mount(1, &Fatfs); //MMC == 1
//	f_mkfs(1, 0, _MAX_SS);

	f_open(&fp, "1:/test.txt", FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	f_write(&fp, w_b, 10, &w_n);
	f_lseek(&fp, 0);
	f_read(&fp, r_b, 10, &r_n);
	f_lseek(&fp, 4);
	f_read(&fp, r_b, 10, &r_n);
	f_close(&fp);

	f_mkdir("1:/dir");
	f_open(&fp, "1:/dir/test.txt", FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	f_write(&fp, str, strlen(str)+1, &w_n);
	f_lseek(&fp, 0);
	f_read(&fp, r_b, 10, &r_n);
	f_lseek(&fp, 4);
	f_read(&fp, r_b, 10, &r_n);
	f_close(&fp);

	return;
}
