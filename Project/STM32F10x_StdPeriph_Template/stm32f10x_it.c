/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "public.h"
#include "driver_conf.h"
#include "ucos_ii.h"
#include "app.h"


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	OS_CPU_SR  cpu_sr;


	OS_ENTER_CRITICAL();						   /* Tell uC/OS-II that we are starting an ISR 		 */
	OSIntNesting++;
	OS_EXIT_CRITICAL();

	OSTimeTick(); 							   /* Call uC/OS-II's OSTimeTick()						 */

	OSIntExit();								   /* Tell uC/OS-II that we are leaving the ISR 		 */
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE)) {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        usart_rx_buf[usart_rx_len++%RX_BUF_LEN] = USART_ReceiveData(USART1);
    }
    if(USART_GetITStatus(USART1, USART_IT_TXE)) {
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
        if(usart_tx_len-- != 0) {
            USART_SendData(USART1, *usart_tx_buf++);
        }
        else {
            usart_tx_buf = NULL;
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
            USART_ITConfig(USART1, USART_IT_TC, ENABLE);
        }
    }
    if(USART_GetITStatus(USART1, USART_IT_TC)) {
        USART_ClearITPendingBit(USART1, USART_IT_TC);
        flag_tx_complete = true;
        USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    }
}

void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        cnt_ms++;
    }
}

void DMA2_Channel3_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr;
    static uint8_t buf_index = 0;

    OS_ENTER_CRITICAL();						   /* Tell uC/OS-II that we are starting an ISR 		 */
    OSIntNesting++;
    OS_EXIT_CRITICAL();
    if(DMA_GetITStatus(DMA2_IT_TC3)) {
        DMA_Cmd(DMA2_Channel3, DISABLE);
        if(0 == buf_index) {
            DMA2_Channel3->CMAR = (uint32_t)wav_buf1;
            buf_index = 1;
        }
        else {
            DMA2_Channel3->CMAR = (uint32_t)wav_buf2;
            buf_index = 0;
        }
        DMA_Cmd(DMA2_Channel3, ENABLE);
        OSMboxPost(msg_wav, (void *)&buf_index);
        DMA_ClearITPendingBit(DMA2_IT_TC3);
    }
    OSIntExit();
}

/*void TIM7_IRQHandler(void)
{
    uint16_t temp = 0;
    if(TIM_GetITStatus(TIM7, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        LED_ON();
        if(wav.pos >= wav.wavlen) {
            TIM_Cmd(TIM7, DISABLE);
            LED_OFF();
            return;
        }
        if(wav.CHnum == 1) {//单声道
            if(wav.SampleBits == 8) {//8位精度
                temp = wav.file[wav.pos];
                wav.pos++;
                DAC_SetChannel2Data(DAC_Align_8b_R, temp/wav.volume);
                DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
            }
            else if(wav.SampleBits == 16) {//16位精度
                temp = ((u8)(wav.file[wav.pos+1]-0x80)<<4) | (wav.file[wav.pos]>>4);
                wav.pos += 2;
                DAC_SetChannel2Data(DAC_Align_12b_R, temp/wav.volume);
                DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
            }
        }
        else if(wav.CHnum == 2) {//立体声
            if(wav.SampleBits == 8) {//8位精度
                temp = wav.file[wav.pos];
                wav.pos += 2;
                DAC_SetChannel2Data(DAC_Align_8b_R, temp/wav.volume);
                DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
            }
            else if(wav.SampleBits == 16) {//16位精度
                temp = ((u8)(wav.file[wav.pos+1]-0x80)<<4) | (wav.file[wav.pos]>>4);
                wav.pos += 4;
                DAC_SetChannel2Data(DAC_Align_12b_R, temp/wav.volume);
                DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
            }
        }
    }
}*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
