// Header file for sensor functions

#ifndef SENSOR_H_
#define SENSOR_H_

#include "common.h"

// Raspberry Pi 5 ADC paths (using MCP3008 or similar SPI ADC)
// If using MCP3008 via SPI, paths will be under /sys/bus/iio/devices/iio:device0/
// Adjust device number if different (check with: ls /sys/bus/iio/devices/)
#define A2D_FILE_VOLTAGE0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_FILE_VOLTAGE2 "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define A2D_FILE_VOLTAGE3 "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"
#define A2D_FILE_VOLTAGE4 "/sys/bus/iio/devices/iio:device0/in_voltage4_raw"
#define A2D_FILE_VOLTAGE5 "/sys/bus/iio/devices/iio:device0/in_voltage5_raw"
#define A2D_FILE_VOLTAGE6 "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"

// Raspberry Pi typical reference voltage (may vary based on your ADC)
// For MCP3008 with 3.3V supply: use 3.3
// For MCP3008 with 5V supply: use 5.0
#define A2D_VOLTAGE_REF_V 3.3
// MCP3008 is 10-bit ADC (0-1023), adjust if using different ADC
#define A2D_MAX_READING 1023

void *readTemperature(void *arg);

void *readIR(void *arg);

void *readAirSensors(void *arg);

#endif