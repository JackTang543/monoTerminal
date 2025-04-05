#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"



extern TIM_HandleTypeDef htim14;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern DMA_HandleTypeDef hdma_spi1_tx;

void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}



/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}



/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}


void TIM8_TRG_COM_TIM14_IRQHandler(){
  HAL_IncTick();
  HAL_TIM_IRQHandler(&htim14);
}

#include "sAPP_Debug.h"

void EXTI9_5_IRQHandler(){
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  }
}

void OTG_FS_IRQHandler(void){
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}

void DMA2_Stream2_IRQHandler(void){
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
}



