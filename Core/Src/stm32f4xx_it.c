/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rtos_tasks.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBOUNCE_MS 50 // Debounce time in milliseconds
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static uint32_t last_sw1_time = 0;
static uint32_t last_sw2_time = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
  char msg[] = "NMI occurred\r\n";
  HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
  char msg[] = "Hard Fault occurred\r\n";
  HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */
  char msg[] = "Memory Management Fault occurred\r\n";
  HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
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
  char msg[] = "Bus Fault occurred\r\n";
  HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
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
  char msg[] = "Usage Fault occurred\r\n";
  HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
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
  char msg[] = "Debug Monitor Fault occurred\r\n";
  HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SW1_Pin);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SW2_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/* USER CODE BEGIN 1 */


/**
 * @brief EXTI GPIO interrupt callback for user button events (SW1/SW2).
 *
 * This callback is invoked by the HAL when an external interrupt occurs on a GPIO pin.
 * Implements 50ms software debouncing for SW1 (PE3) and SW2 (PE4):
 *   - SW1 triggers the OLED to display the bongo cat screen.
 *   - SW2 triggers the OLED to display the QR code screen.
 * Only SW1/SW2 will send display mode to the OLED RTOS task via message queue.
 * All other GPIO interrupts will only print an error message via UART3.
 *
 * @param gpio_pin The GPIO pin number that triggered the interrupt (e.g., SW1_Pin, SW2_Pin).
 * @return None
 *
 * @note This function is RTOS-safe and non-blocking. Debug messages are sent over UART3.
 *
 * @par Example
 * @code
 * // Press SW1 (PE3): OLED shows bongo cat screen, UART prints "SW1: Show bongo cat screen"
 * // Press SW2 (PE4): OLED shows QR code, UART prints "SW2: Show QR code screen"
 * // Other pins: UART prints error message only
 * @endcode
 */
void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
    uint32_t current_time = HAL_GetTick();
    char msg[64];

    if (gpio_pin == SW1_Pin)
    {
        // SW1: Show bongo cat screen
        if ((HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_SET) &&
            ((current_time - last_sw1_time) > DEBOUNCE_MS))
        {
            uint8_t mode = DISPLAY_MODE_BONGO;
            snprintf(msg, sizeof(msg), "SW1: Show bongo cat screen\r\n");
            HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
            if (osMessageQueuePut(display_mode_queue, &mode, 0, 0) != osOK)
            {
                snprintf(msg, sizeof(msg), "SW1: Failed to send mode to queue\r\n");
                HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
            }
            last_sw1_time = current_time;
        }
    }
    else if (gpio_pin == SW2_Pin)
    {
        // SW2: Show QR code screen
        if ((HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_SET) &&
            ((current_time - last_sw2_time) > DEBOUNCE_MS))
        {
            uint8_t mode = DISPLAY_MODE_QRCODE;
            snprintf(msg, sizeof(msg), "SW2: Show QR code page\r\n");
            HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
            if (osMessageQueuePut(display_mode_queue, &mode, 0, 0) != osOK)
            {
                snprintf(msg, sizeof(msg), "SW2: Failed to send mode to queue\r\n");
                HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
            }
            last_sw2_time = current_time;
        }
    }
    else
    {
        // Other GPIO interrupts: print error message only
        snprintf(msg, sizeof(msg), "Unknown GPIO interrupt (pin=%u), ignored!\r\n", gpio_pin);
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
    }
}

/* USER CODE END 1 */
