#include "public.h"
#include "driver_conf.h"
#include "app.h"


/*******************************************************************************
SYSCLK          72M
HCLK    AHB     72M
PCLK1   APB1    36M
PCLK2   APB2    72M
ADCCLK          12M
*******************************************************************************/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    periph_init();
//    play_wav();//DAC单声道8位WAV
//    play_midi(&xyz);
//    fatfs_test();

    app_init();
    for(;;){}

    while (1) {
        prepare_data();
/*        if((cnt_ms % 10) == true) {
            key_scan();
            TIM_SetCompare2(TIM3, cnt_ms%1500);
            ADC_SoftwareStartConvCmd(ADC1, ENABLE);
            while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
            adc_val = ADC_GetConversionValue(ADC1);
            wav.volume = 255 * adc_val / 0xFFF / 26 + 1;
        }
        if(volume_modify == true) {
            volume_modify = false;
            for(i = 0; i < 512; i++) {
                //wav.file[wav.pos+512+i] = wav.file[wav.pos+512+i] / wav.volume;
            }
        }
        if(trg == 1) {
            //短按
        }
        if(cont == 1) {
            //长按
        }
        if(flag_sec == true) {
            flag_sec = false;
            usart_send((uint8_t *)&wav.volume, sizeof(wav.volume));
        }*/
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
