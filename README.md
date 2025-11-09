# Advanced Fire and Gas Detector

An embedded fire and gas detection system built with C programming language for Raspberry Pi 5 hardware. This system provides real-time monitoring and alerting capabilities for fire and gas hazards.

## Overview

This project implements a comprehensive fire and gas detection system using multiple sensors for redundant safety monitoring. The system features visual alarm capabilities and is designed for reliable operation in safety-critical environments.

## Hardware Components

### Target Platform
- **Raspberry Pi 5**: Main processing unit and GPIO controller

### Sensors (5 total)
- Fire detection sensors
- Gas detection sensors
- Connected via ADC (e.g., MCP3008 via SPI) for analog input

### Display & Alerting
- **8x8 LED Matrix**: Visual alarm display connected via I2C interface
- Provides clear visual indication of alarm states

## Software Architecture

### Development Environment
- **Target OS**: Raspberry Pi OS (64-bit recommended)
- **Programming Language**: C
- **Compilation**: Native compilation on Raspberry Pi 5

### Key Features
- Real-time sensor monitoring
- GPIO interface management
- I2C communication for LED matrix control
- Alarm state management
- Multi-threaded architecture

## System Design

### Sensor Integration
- Multiple sensor inputs for enhanced reliability
- SPI-based ADC interfacing (e.g., MCP3008)
- Configurable threshold detection

### Alert System
- 8x8 LED matrix for visual warnings
- I2C protocol for display communication
- Customizable alarm patterns

### Safety Features
- Multi-sensor redundancy
- Real-time monitoring
- Fail-safe alarm mechanisms

## Getting Started

### Prerequisites
- Raspberry Pi 5 with Raspberry Pi OS
- GCC compiler (usually pre-installed)
- pthread library (usually pre-installed)
- SPI-based ADC (e.g., MCP3008) for analog sensors
- 8x8 LED matrix display (I2C)
- Root/sudo access for GPIO and hardware interfaces

### Hardware Setup
1. **Enable I2C Interface:**
   ```bash
   sudo raspi-config
   # Navigate to: Interface Options -> I2C -> Enable
   ```

2. **Enable SPI Interface (for ADC):**
   ```bash
   sudo raspi-config
   # Navigate to: Interface Options -> SPI -> Enable
   ```

3. **Connect Hardware:**
   - Connect MCP3008 ADC to SPI pins (GPIO 8, 9, 10, 11)
   - Connect 5 sensors to ADC channels 0-4
   - Wire 8x8 LED matrix to I2C pins (GPIO 2 SDA, GPIO 3 SCL)
   - Connect button to GPIO17 (or adjust in `user.h`)

4. **Export GPIO for button (if needed):**
   ```bash
   echo 17 | sudo tee /sys/class/gpio/export
   echo in | sudo tee /sys/class/gpio/gpio17/direction
   ```

### Software Compilation and Execution

1. **Clone/Download the repository**

2. **Compile the program:**
   ```bash
   make clean
   make
   ```

3. **Run the program (requires sudo for hardware access):**
   ```bash
   sudo ./main
   ```

4. **Stop the program:**
   - Press the connected button (GPIO17 by default)
   - Or use Ctrl+C

### Configuration Notes

- **ADC Settings:** Default is configured for MCP3008 (10-bit, 3.3V reference)
  - If using different ADC, update `A2D_MAX_READING` and `A2D_VOLTAGE_REF_V` in `sensor.h`
  
- **GPIO Pin Mapping:** 
  - Button: GPIO17 (change in `user.h` if needed)
  - I2C: GPIO2 (SDA), GPIO3 (SCL) - hardware fixed
  - SPI: GPIO8-11 - hardware fixed

- **Sensor Thresholds:** Adjust detection thresholds in `common.h`:
  - `TEMPPOINT`: Temperature alarm threshold
  - `IRPOINT`: IR sensor threshold
  - `COPOINT`: CO sensor threshold
  - `CO2POINT`: CO2 sensor threshold
  - `SMOKEPOINT`: Smoke sensor threshold

## Technical Specifications

- **Target Architecture**: ARM64 (Raspberry Pi 5)
- **Communication Protocols**: GPIO, I2C, SPI
- **Sensor Count**: 5 detection units
- **Display**: 8x8 LED matrix
- **Real-time Operation**: Continuous monitoring with multi-threaded design

## Troubleshooting

- **Permission denied errors:** Run with `sudo`
- **I2C/SPI not working:** Ensure interfaces are enabled in `raspi-config`
- **ADC not reading:** Check SPI wiring and that the correct device appears in `/sys/bus/iio/devices/`
- **GPIO button not working:** Verify GPIO is exported and pin number matches your connection

This embedded system provides a robust foundation for fire and gas detection applications with reliable hardware interfacing optimized for Raspberry Pi 5.
