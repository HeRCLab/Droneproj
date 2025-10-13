/*
 * ESP32-S3-WROOM-1 N16R8 (16MB flash, 8MB PSRAM) board definition
 * Minimal, sensorless bring-up: I2C + GPS + RC outputs mapped, WiFi enabled.
 * GPLv3 (see ArduPilot COPYING.txt)
 */

#pragma once

// -----------------------------------------------------------------------------
// Basic identity
// -----------------------------------------------------------------------------
#define CONFIG_HAL_BOARD_SUBTYPE HAL_BOARD_SUBTYPE_ESP32_S3_WROOM1_N16R8
#define HAL_ESP32_BOARD_NAME "esp32s3-wroom1-n16r8"

// sanity booleans
#define TRUE  1
#define FALSE 0

// -----------------------------------------------------------------------------
// Default Serial Manager protocols
// Serial index mapping in ArduPilot (ESP32):
//   SERIAL0 = console (USB-Serial-JTAG here)
//   SERIAL3 = UART1 (GPS1 in this config)
//   SERIAL2 = UART2 (free)
// Others default to None.
// -----------------------------------------------------------------------------

// Console via USB-Serial-JTAG (handled by IDF; no pins here)
/// #define DEFAULT_SERIAL0_PROTOCOL      SerialProtocol_MAVLink2
/// #define DEFAULT_SERIAL0_BAUD          AP_SERIALMANAGER_CONSOLE_BAUD/1000

// UART2 free (MAVLink2)
#define DEFAULT_SERIAL2_PROTOCOL        SerialProtocol_MAVLink2
#define DEFAULT_SERIAL2_BAUD            (AP_SERIALMANAGER_MAVLINK_BAUD/1000)  // 57600

// UART1 = GPS1
#define DEFAULT_SERIAL3_PROTOCOL        SerialProtocol_GPS
#define DEFAULT_SERIAL3_BAUD            (AP_SERIALMANAGER_GPS_BAUD/1000)      // 38400 (configurable)

// leave the rest None
#define DEFAULT_SERIAL4_PROTOCOL        SerialProtocol_None
#define DEFAULT_SERIAL5_PROTOCOL        SerialProtocol_None
#define DEFAULT_SERIAL6_PROTOCOL        SerialProtocol_None
#define DEFAULT_SERIAL7_PROTOCOL        SerialProtocol_None
#define DEFAULT_SERIAL8_PROTOCOL        SerialProtocol_None
#define DEFAULT_SERIAL9_PROTOCOL        SerialProtocol_None

#define DEFAULT_SERIAL4_BAUD            (115200/1000)
#define DEFAULT_SERIAL5_BAUD            (115200/1000)
#define DEFAULT_SERIAL6_BAUD            (115200/1000)
#define DEFAULT_SERIAL7_BAUD            (115200/1000)
#define DEFAULT_SERIAL8_BAUD            (115200/1000)
#define DEFAULT_SERIAL9_BAUD            (115200/1000)

// -----------------------------------------------------------------------------
// Sensors: start sensorless to ensure clean MAVLink bring-up
// -----------------------------------------------------------------------------
#define HAL_INS_DEFAULT                 HAL_INS_NONE
#define AP_COMPASS_ENABLE_DEFAULT       0
#define ALLOW_ARM_NO_COMPASS

// Allow no baro at boot
#define HAL_BARO_ALLOW_INIT_NO_BARO     1

// Airspeed off
#define AP_AIRSPEED_ENABLED             0
#define AP_AIRSPEED_ANALOG_ENABLED      0
#define AP_AIRSPEED_BACKEND_DEFAULT_ENABLED 0

// -----------------------------------------------------------------------------
// I2C: primary bus on GPIO8/9 (common, safe S3 choice), 400kHz
// -----------------------------------------------------------------------------
#define HAL_ESP32_I2C_BUSES  { \
    .port=I2C_NUM_0, .sda=GPIO_NUM_8, .scl=GPIO_NUM_9, .speed=400*KHZ, .internal=true, .soft=false \
}

// -----------------------------------------------------------------------------
// SPI: (none for now; add when wiring IMU/flash/etc.)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// RMT/RCIN: leave RCIN off for now (use SBUS/CRSF later via a UART if desired)
// -----------------------------------------------------------------------------
#define HAL_ESP32_RMT_RX_PIN_NUMBER     4
/* RCIN disabled for now (legacy RMT path)
// #define HAL_ESP32_RCIN                GPIO_NUM_36
*/

// -----------------------------------------------------------------------------
// RCOUT: pick PWM-capable pins that are safe on S3 and not used by USB/UART/I2C
// Chosen: 10, 11, 12, 13, 14, 15  (all valid GPIOs on S3)
// -----------------------------------------------------------------------------
#define HAL_ESP32_RCOUT { GPIO_NUM_10, GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_15 }

// -----------------------------------------------------------------------------
// WiFi: enable simple WiFi transport (you can disable if you want pure UART)
// -----------------------------------------------------------------------------
#define HAL_ESP32_WIFI                  1     // 1 = TCP, 2 = UDP
#define WIFI_SSID                       "ardupilot-esp32"
#define WIFI_PWD                        "ardupilot-esp32"

// -----------------------------------------------------------------------------
// UART pin mapping:
//   UART0: console via USB-Serial-JTAG (no pins needed here)
//   UART1: GPS (RX=18, TX=17)
//   UART2: free (RX=16, TX=5)  -> adjust as you like
// -----------------------------------------------------------------------------
#define HAL_ESP32_UART_DEVICES \
    { .port=UART_NUM_0, .rx=(gpio_num_t)-1, .tx=(gpio_num_t)-1 }, /* Console over USB-JTAG, pins not used */ \
    { .port=UART_NUM_1, .rx=GPIO_NUM_18, .tx=GPIO_NUM_17 },       /* GPS1 */ \
    { .port=UART_NUM_2, .rx=GPIO_NUM_16, .tx=GPIO_NUM_5  }        /* Free */

// -----------------------------------------------------------------------------
// ADC / LEDs
// -----------------------------------------------------------------------------
#define DEFAULT_NTF_LED_TYPES           Notify_LED_None

// -----------------------------------------------------------------------------
// SD card: none by default (enable later via SDMMC or SDSPI if you add a socket)
// -----------------------------------------------------------------------------
/*
#define HAL_ESP32_SDCARD
#define HAL_ESP32_SDMMC
// or, SPI mode example (adjust pins to your wiring):
#define HAL_ESP32_SDSPI { .host=SPI3_HOST, .dma_ch=2, .mosi=GPIO_NUM_2, .miso=GPIO_NUM_1, .sclk=GPIO_NUM_7, .cs=GPIO_NUM_6 }
*/

// -----------------------------------------------------------------------------
// Logging: keep disabled at first to save RAM/flash while bringing up
// -----------------------------------------------------------------------------
#define HAL_LOGGING_FILESYSTEM_ENABLED  0
#define HAL_LOGGING_DATAFLASH_ENABLED   0
#define HAL_LOGGING_MAVLINK_ENABLED     1   // keep MAVLink logging path enabled (GCS)

// ArduPilot logging backends default mask
#define HAL_LOGGING_BACKENDS_DEFAULT    1

// RC protocols (SBUS/CRSF) parsing in HAL — keep off initially; enable via params later
#define AP_RCPROTOCOL_ENABLED           0

// -----------------------------------------------------------------------------
// Copter frames: keep just QUAD to reduce DRAM .bss on early bring-up
// -----------------------------------------------------------------------------
#define AP_MOTORS_FRAME_DEFAULT_ENABLED 0
#define AP_MOTORS_FRAME_QUAD_ENABLED    1


