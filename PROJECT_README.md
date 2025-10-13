# ESP32-S3 ArduCopter Project
**CSCE 611 - Fall 2025**  
**Student:** Pedro Fischetti  
**Date:** October 13, 2025

## Project Overview

This project demonstrates the successful compilation, configuration, and flashing of ArduPilot ArduCopter firmware onto an ESP32-S3-WROOM-1 N16R8 development board. The project involved creating a custom board configuration, resolving compilation issues, and successfully deploying the firmware to hardware.

## Hardware Specifications

- **Board:** ESP32-S3-WROOM-1 N16R8
- **MCU:** ESP32-S3 (QFN56) revision v0.2
- **Flash:** 16MB
- **PSRAM:** 8MB (AP_3v3)
- **Crystal:** 40MHz
- **USB:** USB-Serial/JTAG
- **MAC Address:** d0:cf:13:19:87:fc

## Project Achievements

### ✅ 1. Environment Setup
- Successfully installed ESP-IDF v5.3
- Configured ESP32 toolchain (xtensa-esp32s3-elf-gcc 13.2.0)
- Set up ArduPilot build environment

### ✅ 2. Custom Board Configuration
Created a custom board configuration for ESP32-S3-WROOM-1 N16R8:
- **Location:** `libraries/AP_HAL_ESP32/boards/esp32s3_wroom1_n16r8.h`
- **Hardware Definition:** `libraries/AP_HAL_ESP32/hwdef/esp32s3_wroom1_n16r8/hwdef.dat`

**Key Features Configured:**
- I2C Bus on GPIO 8/9 (400kHz)
- UART1 for GPS (RX=18, TX=17)
- UART2 for MAVLink (RX=16, TX=5)
- RC Outputs on GPIOs 10-15
- WiFi enabled (SSID: "ardupilot-esp32")
- Sensorless configuration for basic bring-up

### ✅ 3. Compilation Issue Resolution
**Problem Encountered:**
```
compilation terminated due to -Wfatal-errors.
error: 'HAL_BOARD_SUBTYPE_ESP32_S3_WROOM1_N16R8' was not declared in this scope
```

**Solution Applied:**
1. Added missing board subtype definition in `libraries/AP_HAL/AP_HAL_Boards.h`:
   ```c
   #define HAL_BOARD_SUBTYPE_ESP32_S3_WROOM1_N16R8 6011
   ```

2. Added configuration definition in board header:
   ```c
   #define CONFIG_HAL_BOARD_SUBTYPE HAL_BOARD_SUBTYPE_ESP32_S3_WROOM1_N16R8
   ```

### ✅ 4. Successful Compilation
- **Build Time:** 2 minutes 8 seconds
- **Target:** ArduCopter for ESP32-S3
- **Output Files Generated:**
  - `build/esp32s3_wroom1_n16r8/esp-idf_build/bootloader/bootloader.bin` (19,712 bytes)
  - `build/esp32s3_wroom1_n16r8/esp-idf_build/partition_table/partition-table.bin` (3,072 bytes)
  - `build/esp32s3_wroom1_n16r8/esp-idf_build/ardupilot.bin` (1,878,512 bytes)

### ✅ 5. Successful Hardware Flashing
**Flashing Command Used:**
```bash
esptool.py --chip esp32s3 --port /dev/ttyACM0 --baud 460800 \
  write_flash --flash_mode dio --flash_freq 80m --flash_size 16MB \
  0x0 build/esp32s3_wroom1_n16r8/esp-idf_build/bootloader/bootloader.bin \
  0x10000 build/esp32s3_wroom1_n16r8/esp-idf_build/partition_table/partition-table.bin \
  0x20000 build/esp32s3_wroom1_n16r8/esp-idf_build/ardupilot.bin
```

**Flashing Results:**
- ✅ Bootloader flashed successfully (409.5 kbit/s)
- ✅ Partition table flashed successfully (409.4 kbit/s)
- ✅ ArduCopter firmware flashed successfully (1163.3 kbit/s)
- ✅ Hash verification passed for all components
- ✅ Device reset and boot successful

## Technical Details

### Build Configuration
- **Board:** esp32s3_wroom1_n16r8
- **Toolchain:** xtensa-esp32s3-elf
- **ESP-IDF Version:** v5.3
- **Build System:** WAF
- **Flash Layout:**
  - Bootloader: 0x0000 - 0x4FFF
  - Partition Table: 0x10000 - 0x10FFF  
  - Application: 0x20000 - 0x31FFFF
  - Storage: 0x320000 - 0x35FFFF

### Memory Usage
- **Flash Usage:** 1.83 MB / 3 MB (40% free space remaining)
- **Bootloader:** 19.7 KB / 64 KB (70% free space remaining)

### Pin Configuration
| Function | GPIO | Notes |
|----------|------|-------|
| I2C SDA | 8 | 400kHz bus |
| I2C SCL | 9 | 400kHz bus |
| GPS RX | 18 | UART1 |
| GPS TX | 17 | UART1 |
| MAVLink RX | 16 | UART2 |
| MAVLink TX | 5 | UART2 |
| RC Out 1 | 10 | PWM capable |
| RC Out 2 | 11 | PWM capable |
| RC Out 3 | 12 | PWM capable |
| RC Out 4 | 13 | PWM capable |
| RC Out 5 | 14 | PWM capable |
| RC Out 6 | 15 | PWM capable |

## Software Architecture

The project uses a minimal, sensorless configuration designed for initial bring-up and testing:

- **INS (Inertial Navigation System):** Disabled (HAL_INS_NONE)
- **Compass:** Disabled (allows arming without compass)
- **Barometer:** Optional (HAL_BARO_ALLOW_INIT_NO_BARO)
- **Airspeed:** Disabled
- **Logging:** MAVLink only (saves RAM/flash)
- **Motor Frames:** QUAD only (reduces memory usage)

## Build Instructions

### Prerequisites
```bash
# Install ESP32 prerequisites
Tools/environment_install/install-esp32-prereqs-ubuntu.sh

# Source ESP-IDF environment
source modules/esp_idf/export.sh
```

### Build Commands
```bash
# Configure for custom board
./waf configure --board esp32s3_wroom1_n16r8

# Build ArduCopter
./waf copter
```

### Flashing Commands
```bash
# Flash complete firmware to ESP32-S3
esptool.py --chip esp32s3 --port /dev/ttyACM0 --baud 460800 \
  write_flash --flash_mode dio --flash_freq 80m --flash_size 16MB \
  0x0 build/esp32s3_wroom1_n16r8/esp-idf_build/bootloader/bootloader.bin \
  0x10000 build/esp32s3_wroom1_n16r8/esp-idf_build/partition_table/partition-table.bin \
  0x20000 build/esp32s3_wroom1_n16r8/esp-idf_build/ardupilot.bin
```

## File Structure

Key files modified/created for this project:

```
libraries/AP_HAL/AP_HAL_Boards.h                          # Added board subtype definition
libraries/AP_HAL_ESP32/boards/esp32s3_wroom1_n16r8.h     # Custom board configuration
libraries/AP_HAL_ESP32/hwdef/esp32s3_wroom1_n16r8/       # Hardware definition directory
build/esp32s3_wroom1_n16r8/                              # Build outputs
```

## Verification and Testing

The successful completion of this project is evidenced by:

1. **Clean Compilation:** No errors or warnings during build process
2. **Successful Flashing:** All firmware components flashed without errors
3. **Hardware Verification:** ESP32-S3 chip properly detected and configured
4. **Memory Management:** Efficient use of flash and RAM resources
5. **Hash Verification:** All flashed components passed integrity checks

## Future Enhancements

This foundation enables several potential enhancements:

- Enable IMU sensors for full flight control
- Add GPS integration for position hold
- Implement barometer for altitude control  
- Add compass for heading control
- Enable SD card logging
- Configure RC input protocols (SBUS/CRSF)
- Add telemetry systems

## Conclusion

This project successfully demonstrates the complete workflow of:
1. Configuring a custom ESP32-S3 board for ArduPilot
2. Resolving compilation issues through proper board subtype definition
3. Building and flashing ArduCopter firmware to real hardware
4. Verifying successful deployment through hardware validation

The ESP32-S3 board is now running ArduCopter firmware and ready for further development and testing.

---
**Project Status:** ✅ COMPLETE - Successfully flashed and verified on hardware  
**Last Updated:** October 13, 2025