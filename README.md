
# NUCLEO-F429ZI OLED RTOS Project

This project demonstrates how to use the NUCLEO-F429ZI development board with an SH1106/SSD1306 OLED (I2C, 128x64), leveraging the u8g2 graphics library and CMSIS-RTOS v2 (FreeRTOS) to achieve professional multi-mode display. The codebase is modular, supporting real-time multitasking, animation, QR code, and info screen switching.

## Features
- **Hardware**: STM32 NUCLEO-F429ZI + OLED (I2C, PB8/PB9)
- **Software**: STM32 HAL, FreeRTOS (CMSIS-RTOS v2), u8g2 graphics library
- **Functionality**:
  - SW1/SW2 button interrupts (PE3, PE4) to switch display modes (welcome/info, QR code, bongo cat animation)
  - OLED display mode persists; bongo cat animation plays automatically when no queue command is present
  - Professional Doxygen documentation and maintainable structure

## Directory Structure
```
NUCLEO-F429ZI_OLED_RTOS/
├── Core/
│   ├── Inc/         # Header files (main.h, rtos_tasks.h, ...)
│   └── Src/         # Source files (main.c, rtos_tasks.c, ...)
├── Hardware/
│   ├── oled/        # OLED driver
│   └── u8g2/        # u8g2 graphics library source
├── Image/           # Bitmap data (bongo_cat, img_qrcode)
├── Drivers/         # HAL, CMSIS, etc.
├── MDK-ARM/         # Keil project files
├── Middlewares/     # Third-party middleware (e.g., FreeRTOS)
├── README.md        # This documentation
└── LICENSE          # License file
```

## Quick Start
1. **Hardware Connection**:
   - OLED SCL → PB8, SDA → PB9, VCC → 5V, GND → GND
   - 4.7kΩ pull-up resistors required on SCL/SDA
   - SW1 (PE3), SW2 (PE4) user buttons for display mode switching (external interrupt)
2. **Development Environment**:
   - STM32CubeIDE or Keil uVision
   - Use STM32CubeMX to verify I2C1/FreeRTOS configuration
3. **Build & Flash**:
   - Import the project into STM32CubeIDE/Keil, build, and flash to NUCLEO-F429ZI
4. **Display Verification**:
   - Default shows welcome message; press SW1/SW2 to switch to QR code/animation

## Main Code Structure
- `rtos_tasks.c/h`: OLED display task, message queue, state machine
- `stm32f4xx_it.c`: External interrupt (SW1/SW2) handling and debounce
- `oled_driver.c/h`: OLED initialization and u8g2 interface
- `Image/`: bongo cat/QR code bitmaps

## Advanced Features
- **Doxygen Documentation**: All core code is documented with professional English Doxygen comments
- **Extensible**: Easily add new display modes, animations, sensors, etc.
- **Error Handling**: UART3 outputs error messages; queue/task creation failure triggers Error_Handler

## License
This project is licensed under the MIT License. See LICENSE for details.
The u8g2 graphics library is licensed under Apache 2.0; please retain its LICENSE file.

## Acknowledgments
- [u8g2 Library](https://github.com/olikraus/u8g2)
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [FreeRTOS](https://www.freertos.org)