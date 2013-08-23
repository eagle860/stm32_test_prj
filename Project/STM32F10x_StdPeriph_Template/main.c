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
//    play_wav();//DACµ¥ÉùµÀ8Î»WAV
//    play_midi(&xyz);
//    fatfs_test();

    app_init();
    for(;;){}
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
