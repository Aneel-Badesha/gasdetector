# Migration from BeagleBone Green to Raspberry Pi 5

## Summary of Changes

This document outlines the modifications made to port the Fire Detector system from BeagleBone Green to Raspberry Pi 5.

### 1. Makefile Changes
**Changed:**
- Replaced `arm-linux-gnueabihf-gcc` (cross-compiler) with `gcc` (native compilation)
- Removed BeagleBone-specific deployment path

**Reason:** Raspberry Pi 5 uses native compilation directly on the device.

### 2. Main Program (main.c)
**Changed:**
- Removed BeagleBone-specific `config-pin` commands
- Added comments about Raspberry Pi I2C configuration

**Reason:** Raspberry Pi uses `raspi-config` for interface configuration, not device tree overlays like BeagleBone.

### 3. GPIO Configuration (user.h)
**Changed:**
- User button GPIO from `gpio72` to `gpio17`

**Reason:** 
- BeagleBone GPIO numbering differs from Raspberry Pi
- GPIO17 is a commonly used button pin on Raspberry Pi
- **User must adjust based on actual hardware connection**

### 4. ADC Configuration (sensor.h)
**Changed:**
- ADC reference voltage: `1.8V` → `3.3V`
- ADC max reading: `4095` (12-bit) → `1023` (10-bit)

**Reason:**
- BeagleBone has built-in 12-bit ADC
- Raspberry Pi requires external ADC (MCP3008 is common 10-bit SPI ADC)
- MCP3008 typically uses 3.3V reference voltage
- **User must adjust based on actual ADC used**

### 5. ADC Interface Paths
**Status:** Unchanged
- Both systems use Linux IIO (Industrial I/O) subsystem
- Paths remain: `/sys/bus/iio/devices/iio:device0/in_voltage*_raw`

**Note:** Device number might differ; check with `ls /sys/bus/iio/devices/`

## Hardware Differences

### BeagleBone Green
- Built-in 7-channel 12-bit ADC (0-1.8V)
- Uses device tree overlays for pin configuration
- `config-pin` utility for runtime configuration
- GPIO accessed via sysfs

### Raspberry Pi 5
- No built-in ADC (requires external SPI/I2C ADC)
- I2C and SPI on fixed hardware pins
- `raspi-config` for enabling interfaces
- GPIO accessed via sysfs (similar to BeagleBone)

## Configuration Required

### Before First Run
1. Enable I2C: `sudo raspi-config` → Interface Options → I2C
2. Enable SPI: `sudo raspi-config` → Interface Options → SPI
3. Export GPIO for button: `echo 17 | sudo tee /sys/class/gpio/export`
4. Set GPIO direction: `echo in | sudo tee /sys/class/gpio/gpio17/direction`

Or simply run: `sudo ./rpi5_setup.sh`

### Verify Hardware Detection
```bash
# I2C devices
i2cdetect -y 1

# SPI/ADC devices
ls /sys/bus/iio/devices/

# GPIO
cat /sys/class/gpio/gpio17/value
```

## Threshold Calibration

The sensor thresholds in `common.h` may need adjustment:
- BeagleBone: 1.8V reference, 12-bit resolution
- Raspberry Pi: 3.3V reference, 10-bit resolution (MCP3008)

The voltage-to-digital conversion ratio changes:
- BeagleBone: 1V = 2275 counts (4095 / 1.8)
- Raspberry Pi (MCP3008): 1V = 310 counts (1023 / 3.3)

**Action Required:** Test and recalibrate threshold values after hardware setup.

## Potential Issues and Solutions

### Issue: ADC readings show 0 or max value
**Solution:** 
- Check SPI wiring
- Verify sensor power supply
- Ensure ADC reference voltage is correct

### Issue: "Permission denied" when accessing GPIO/I2C/SPI
**Solution:** Run program with `sudo`

### Issue: I2C device not found
**Solution:** 
- Enable I2C in raspi-config
- Check physical connections
- Verify LED matrix is powered

### Issue: Program compiles but sensors don't work
**Solution:**
- Check ADC device path: `ls /sys/bus/iio/devices/`
- Update paths in `sensor.h` if needed
- Verify SPI kernel modules loaded: `lsmod | grep spi`

## Testing Steps

1. **Hardware Test:** Run individual component tests (see test_hardware.c)
2. **GPIO Test:** Verify button detection
3. **ADC Test:** Check sensor readings are reasonable
4. **I2C Test:** Confirm LED matrix communication
5. **Full System Test:** Run main program and monitor output

## Files Added for Raspberry Pi 5

- `rpi5_setup.sh` - Automated setup script
- `RPI5_HARDWARE_SETUP.md` - Detailed hardware wiring guide
- `MIGRATION_NOTES.md` - This file
- `test_hardware.c` - Hardware testing utility (optional)

## Compatibility Notes

The software architecture remains the same:
- Multi-threaded design
- Mutex-based synchronization
- Sensor averaging and filtering
- Alarm logic

Only hardware interface details were changed for Raspberry Pi 5 compatibility.
