# Quick Start Guide - Raspberry Pi 5

## Initial Setup (One-Time)

### 1. Prepare Raspberry Pi 5
```bash
# Update system
sudo apt-get update
sudo apt-get upgrade -y

# Install required packages (if not already installed)
sudo apt-get install -y build-essential i2c-tools git
```

### 2. Run Setup Script
```bash
# Make script executable
chmod +x rpi5_setup.sh

# Run setup (enables I2C, SPI, configures GPIO)
sudo ./rpi5_setup.sh

# Reboot to ensure all changes take effect
sudo reboot
```

### 3. Connect Hardware
Follow the wiring diagram in `RPI5_HARDWARE_SETUP.md`:
- Connect MCP3008 ADC to SPI pins
- Connect 5 sensors to ADC channels 0-4
- Connect 8x8 LED matrix to I2C pins (GPIO2/3)
- Connect button to GPIO17

## Testing Hardware

### Quick Test
```bash
# Compile test utility
make test

# Run hardware tests
sudo ./test_hardware

# Or test individual components
sudo ./test_hardware button
sudo ./test_hardware adc
sudo ./test_hardware i2c
```

### Manual Tests
```bash
# Check I2C devices (should show LED matrix address)
i2cdetect -y 1

# Check SPI/ADC devices
ls /sys/bus/iio/devices/

# Test button
cat /sys/class/gpio/gpio17/value  # Should show 1 (not pressed) or 0 (pressed)

# Test ADC channel 0
cat /sys/bus/iio/devices/iio:device0/in_voltage0_raw
```

## Compile and Run

### Build Main Program
```bash
# Clean previous builds
make clean

# Compile
make

# The executable 'main' is created
```

### Run the Fire Detector
```bash
# Must run with sudo for hardware access
sudo ./main
```

### Stop the Program
- Press the physical button (GPIO17)
- Or use `Ctrl+C` in terminal

## Expected Output

When running correctly, you should see:
```
Temperature: 23.45, IR Voltage: 1.23
CO Voltage: 0.56, CO2 Voltage: 0.78, Smoke Voltage: 0.34
```

The program will continuously monitor sensors and display readings every 2.5 seconds.

## Troubleshooting

### "Permission denied" errors
**Solution:** Run with `sudo`:
```bash
sudo ./main
```

### "ERROR: unable to open voltage input file"
**Cause:** ADC not detected or SPI not enabled

**Solutions:**
1. Check SPI is enabled: `ls /dev/spidev*`
2. If empty, enable SPI: `sudo raspi-config` → Interface Options → SPI
3. Check ADC wiring (especially power and SPI connections)
4. Verify correct device path: `ls /sys/bus/iio/devices/`
5. If device number differs, update paths in `sensor.h`

### I2C LED matrix not working
**Solutions:**
1. Check I2C is enabled: `ls /dev/i2c*`
2. Scan for devices: `i2cdetect -y 1`
3. Verify LED matrix has power (3.3V or 5V depending on module)
4. Check SDA/SCL connections

### Button not responding
**Solutions:**
1. Verify GPIO exported: `ls /sys/class/gpio/ | grep gpio17`
2. If not exported: `echo 17 | sudo tee /sys/class/gpio/export`
3. Check button wiring (button should connect GPIO17 to GND when pressed)
4. Test manually: `watch -n 0.1 cat /sys/class/gpio/gpio17/value` (press button to see change)

### Sensor readings seem wrong
**Possible causes:**
1. ADC reference voltage mismatch (check `sensor.h`)
2. Sensor not calibrated
3. Threshold values need adjustment for new hardware

**Solutions:**
1. Verify ADC settings in `sensor.h`:
   - `A2D_VOLTAGE_REF_V` should match your ADC setup (typically 3.3V)
   - `A2D_MAX_READING` should be 1023 for MCP3008 (10-bit)
2. Calibrate thresholds in `common.h` based on actual sensor readings
3. Check sensor power supply voltage

## Configuration Files

### Key files to adjust for your hardware:
- **`sensor.h`**: ADC voltage reference and resolution
- **`user.h`**: Button GPIO pin number
- **`common.h`**: Sensor threshold values

## Additional Resources

- `README.md` - Full project documentation
- `RPI5_HARDWARE_SETUP.md` - Detailed wiring guide
- `MIGRATION_NOTES.md` - Changes from BeagleBone version

## Support

If problems persist:
1. Verify power supply (RPi5 needs 5V/5A, 27W recommended)
2. Check all physical connections
3. Review system logs: `dmesg | tail -50`
4. Check interface status: `sudo systemctl status`
