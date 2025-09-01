

/**
 * @file    rtos_tasks.c
 * @author  Ted Wang
 * @date    2025-08-01
 * @brief   RTOS task implementation for OLED display control on NUCLEO-F429ZI.
 *
 * @details
 * This file implements the OLED display RTOS task using CMSIS-RTOS v2 and the u8g2 graphics library.
 * The task initializes the OLED (SSD1306, 128x64, I2C1) and updates the display based on three modes:
 *   - Welcome/info message
 *   - QR code
 *   - Bongo cat animation
 * Display mode is controlled via a message queue triggered by SW1 (PE3) and SW2 (PE4) button interrupts.
 * The bongo cat animation toggles between two frames every 200ms. All code is modularized for clarity and maintainability.
 */

/* Includes ------------------------------------------------------------------*/
#include "rtos_tasks.h"
#include "main.h"
#include "oled_driver.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "../Image/img_qrcode.h"
#include "../Image/bongo_cat_1.h"
#include "../Image/bongo_cat_2.h"

/**
 * @defgroup OLED_Private_Defines OLED Private Defines
 * @brief Private macro definitions for OLED display task
 * @{
 */
/** Width of QR code and bongo cat images (pixels) */
#define IMAGE_WIDTH   64
/** Height of QR code and bongo cat images (pixels) */
#define IMAGE_HEIGHT  64
/** Width of bongo cat image (pixels) */
#define BONGO_WIDTH   101
/** Vertical offset for text lines in QR code mode (pixels) */
#define TEXT_OFFSET_Y 15
/** @} */

/**
 * @defgroup OLED_Private_Variables OLED Private Variables
 * @brief Private variables for OLED display task
 * @{
 */
/** OLED task handle */
static osThreadId_t oled_task_handle;
/** Queue for display mode updates */
osMessageQueueId_t display_mode_queue;
/** Current display mode */
DisplayMode_t current_display_mode = DISPLAY_MODE_INFO;
/** UART3 handle for debug/error output */
extern UART_HandleTypeDef huart3;
/** @} */

/**
 * @defgroup OLED_Private_Functions OLED Private Functions
 * @brief Private function prototypes for OLED display task
 * @{
 */
/**
 * @brief OLED display task function (RTOS thread entry)
 * @param argument Unused task parameter (required by CMSIS-RTOS API)
 */
static void OLED_Display_Task(void *argument);
/**
 * @brief Draw bongo cat animation frame
 * @param u8g2 Pointer to the u8g2 display structure
 */
static void DrawBongoCat(u8g2_t *u8g2);
/**
 * @brief Draw QR code screen
 * @param u8g2 Pointer to the u8g2 display structure
 */
static void DrawQRCode(u8g2_t *u8g2);
/**
 * @brief Draw info/welcome screen
 * @param u8g2 Pointer to the u8g2 display structure
 */
static void DrawInfoScreen(u8g2_t *u8g2);
/** @} */


/**
 * @brief  Initialize the OLED display RTOS task and message queue.
 *
 * This function creates the message queue for display mode updates and starts the OLED display task.
 * It must be called once during system initialization (typically in main.c) before the RTOS kernel starts.
 *
 * @note If queue or task creation fails, the function will output an error message via UART3 and call Error_Handler().
 */
void OLED_Task_Init(void)
{
    display_mode_queue = osMessageQueueNew(OLED_DISPLAY_MODE_QUEUE_SIZE, sizeof(DisplayMode_t), NULL);
    if (display_mode_queue == NULL)
    {
        char msg[] = "Failed to create display mode queue\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        Error_Handler();
    }

    const osThreadAttr_t oled_task_attributes = {
        .name = OLED_TASK_THREAD_NAME,
        .priority = OLED_TASK_THREAD_PRIORITY,
        .stack_size = OLED_TASK_STACK_SIZE_BYTES
    };
    oled_task_handle = osThreadNew(OLED_Display_Task, NULL, &oled_task_attributes);
    if (oled_task_handle == NULL)
    {
        char msg[] = "Failed to create OLED display task\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        Error_Handler();
    }
}



/**
 * @brief  RTOS OLED display task (main display loop).
 *
 * This RTOS task initializes the OLED hardware and continuously updates the display
 * according to the current display mode received from the message queue. Supported modes:
 *   - DISPLAY_MODE_INFO: Shows the welcome/info message
 *   - DISPLAY_MODE_QRCODE: Shows the QR code page
 *   - DISPLAY_MODE_BONGO: Shows the bongo cat animation (default/fallback)
 *
 * If an invalid mode is received, the display will default to the info screen.
 * The bongo cat animation toggles frames every 200ms.
 *
 * @param argument [in] Unused task parameter (required by CMSIS-RTOS API)
 * @return None
 * @note This function runs as an RTOS thread and must not return.
 */
static void OLED_Display_Task(void *argument)
{
    OLED_Init();
    u8g2_t *u8g2 = OLED_GetDisplay();
    if (u8g2 == NULL)
    {
        char msg[] = "Failed to initialize OLED display\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        Error_Handler();
    }

    u8g2_ClearBuffer(u8g2);
    u8g2_ClearDisplay(u8g2);
    u8g2_SendBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_ncenB08_tr);

    static uint32_t last_update = 0;
    while (1)
    {
        DisplayMode_t new_mode;
        osStatus_t status = osMessageQueueGet(display_mode_queue, &new_mode, NULL, OLED_ANIMATION_DELAY_MS);

        if (status == osOK)
        {
            current_display_mode = new_mode;
            u8g2_ClearBuffer(u8g2);
        }

        uint32_t current_time = osKernelGetTickCount();
        if (current_time - last_update >= OLED_ANIMATION_DELAY_MS)
        {
            u8g2_ClearBuffer(u8g2);  
            switch (current_display_mode)
            {
                case DISPLAY_MODE_BONGO:
                    DrawBongoCat(u8g2);
                    break;
                case DISPLAY_MODE_QRCODE:
                    DrawQRCode(u8g2);
                    break;
                case DISPLAY_MODE_INFO:
                default:
                    DrawInfoScreen(u8g2);
                    break;
            }
            u8g2_SendBuffer(u8g2);
            last_update = current_time;
        }
    }
}

/**
 * @brief Draw the bongo cat animation frame on the OLED.
 *
 * This function toggles between two bongo cat frames to create a simple animation effect.
 * It should be called every 200ms from the display task loop.
 *
 * @param u8g2 Pointer to the u8g2 display structure.
 * @return None
 */
static void DrawBongoCat(u8g2_t *u8g2)
{
    static bool frame = false;
    u8g2_DrawXBMP(u8g2, 13, 0, BONGO_WIDTH, IMAGE_HEIGHT, frame ? gImage_bongo_cat_1 : gImage_bongo_cat_2);
    frame = !frame;
}

/**
 * @brief Draw the QR code screen on the OLED.
 *
 * This function draws a QR code image and several descriptive text lines at calculated positions.
 *
 * @param u8g2 Pointer to the u8g2 display structure.
 * @return None
 */
static void DrawQRCode(u8g2_t *u8g2)
{
    u8g2_DrawXBMP(u8g2, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, gImage_img_qrcode);
    u8g2_DrawStr(u8g2, 70, TEXT_OFFSET_Y, "QRcode");
    u8g2_DrawStr(u8g2, 70, TEXT_OFFSET_Y + TEXT_OFFSET_Y, "scan can");
    u8g2_DrawStr(u8g2, 70, TEXT_OFFSET_Y + 2 * TEXT_OFFSET_Y, "link to");
    u8g2_DrawStr(u8g2, 70, TEXT_OFFSET_Y + 3 * TEXT_OFFSET_Y, "Youtube");
}

/**
 * @brief Draw the info/welcome screen on the OLED.
 *
 * This function draws a welcome message and additional info text on the OLED display.
 *
 * @param u8g2 Pointer to the u8g2 display structure.
 * @return None
 */
static void DrawInfoScreen(u8g2_t *u8g2)
{
    u8g2_DrawStr(u8g2, 0, TEXT_OFFSET_Y, OLED_WELCOME_MESSAGE);
    u8g2_DrawStr(u8g2, 0, TEXT_OFFSET_Y + TEXT_OFFSET_Y, "My name is Ted.");
    u8g2_DrawStr(u8g2, 0, TEXT_OFFSET_Y + 2 * TEXT_OFFSET_Y, "How are you doing?");
}
