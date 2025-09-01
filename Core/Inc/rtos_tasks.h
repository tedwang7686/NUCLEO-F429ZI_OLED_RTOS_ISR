

/**
 * @file    rtos_tasks.h
 * @author  Ted Wang
 * @date    2025-08-01
 * @brief   RTOS task management header for OLED display control on NUCLEO-F429ZI.
 *
 * @details
 * This header defines types, constants, global variables, and function prototypes for
 * managing the OLED display RTOS task using CMSIS-RTOS v2 and the u8g2 graphics library.
 * It supports switching between three display modes (welcome/info, QR code, bongo cat)
 * via a message queue triggered by SW1 (PE3) and SW2 (PE4) button interrupts.
 */

#ifndef RTOS_TASKS_H
#define RTOS_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os2.h"
#include "u8g2.h"

/**
 * @enum DisplayMode_t
 * @brief OLED display mode enumeration.
 *
 * Enumerates the available display modes for the OLED RTOS task.
 */
typedef enum {
    DISPLAY_MODE_BONGO = 0,   /**< Bongo cat animation page (default/fallback) */
    DISPLAY_MODE_QRCODE = 1,  /**< QR code page */
    DISPLAY_MODE_INFO = 2     /**< Welcome/info message page */
} DisplayMode_t;

/* Exported constants --------------------------------------------------------*/
/**
 * @def OLED_ANIMATION_DELAY_MS
 * @brief Animation frame delay for OLED display (milliseconds).
 */
#define OLED_ANIMATION_DELAY_MS      200

/**
 * @def OLED_WELCOME_MESSAGE
 * @brief Welcome message string displayed on the OLED info page.
 */
#define OLED_WELCOME_MESSAGE         "Hi, NUCLEO-F429ZI!"

/**
 * @def OLED_INFO_NAME
 * @brief Name string displayed on the OLED info page
 */
#define OLED_INFO_NAME               "My name is Ted."

/**
 * @def OLED_INFO_GREETING
 * @brief Greeting string displayed on the OLED info page
 */
#define OLED_INFO_GREETING           "How are you doing?"

/**
 * @def OLED_TASK_STACK_SIZE_BYTES
 * @brief Stack size (bytes) for the OLED RTOS task.
 */
#define OLED_TASK_STACK_SIZE_BYTES   (512 * 4)

/**
 * @def OLED_TASK_THREAD_NAME
 * @brief Name of the OLED RTOS task (for debugging/RTOS awareness).
 */
#define OLED_TASK_THREAD_NAME        "OLED_Task"

/**
 * @def OLED_TASK_THREAD_PRIORITY
 * @brief Priority of the OLED RTOS task.
 */
#define OLED_TASK_THREAD_PRIORITY    osPriorityNormal

/**
 * @def OLED_DISPLAY_MODE_QUEUE_SIZE
 * @brief Message queue size for display mode updates.
 */
#define OLED_DISPLAY_MODE_QUEUE_SIZE 3

/* Exported variables --------------------------------------------------------*/
/**
 * @brief Message queue handle for display mode updates (shared by ISR and OLED task).
 */
extern osMessageQueueId_t display_mode_queue;

/**
 * @brief Current display mode (shared by ISR and OLED task).
 */
extern DisplayMode_t current_display_mode;
/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Initialize the OLED display RTOS task and message queue.
 *
 * This function configures and creates the OLED display task and its associated message queue
 * using the CMSIS-RTOS v2 API. The task handles OLED updates based on mode changes triggered by
 * SW1 and SW2 button interrupts. Call this function once during system initialization.
 */
void OLED_Task_Init(void);


#ifdef __cplusplus
}
#endif

#endif // RTOS_TASKS_H