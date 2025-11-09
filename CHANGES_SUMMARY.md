# Raspberry Pi 5 Port - Complete Summary

## ✅ Project Successfully Updated for Raspberry Pi 5!

This fire and gas detector system has been ported from BeagleBone Green to Raspberry Pi 5.

---

## 📋 Files Modified

### Core Files
1. **`Makefile`**
   - Changed compiler from `arm-linux-gnueabihf-gcc` to `gcc` (native compilation)
   - Fixed bug in `common.o` rule
   - Added `test` target for hardware testing utility
   - Updated `clean` target

2. **`main.c`**
   - Removed BeagleBone-specific `config-pin` commands
   - Added comments about Raspberry Pi I2C configuration

3. **`sensor.h`**
   - Updated ADC reference voltage: 1.8V → 3.3V
   - Updated ADC resolution: 4095 (12-bit) → 1023 (10-bit)
   - Added detailed comments about MCP3008 ADC

4. **`user.h`**
   - Changed button GPIO: gpio72 → gpio17
   - Added setup instructions in comments

5. **`README.md`**
   - Complete rewrite for Raspberry Pi 5
   - Added setup instructions
   - Added troubleshooting section
   - Updated hardware specifications

---

## 📄 New Files Created

1. **`rpi5_setup.sh`** - Automated setup script
   - Enables I2C and SPI interfaces
   - Exports and configures GPIO17 for button
   - Installs required packages

2. **`RPI5_HARDWARE_SETUP.md`** - Detailed hardware guide
   - Complete wiring diagrams
   - Pin mapping for all components
   - Voltage considerations
   - Testing procedures

3. **`MIGRATION_NOTES.md`** - Technical migration details
   - Detailed explanation of all changes
   - Hardware differences between platforms
   - Calibration notes
   - Troubleshooting guide

4. **`QUICKSTART.md`** - Quick reference guide
   - Step-by-step setup instructions
   - Common commands
   - Quick troubleshooting

5. **`test_hardware.c`** - Hardware testing utility
   - Tests I2C, SPI/ADC, and GPIO
   - Provides diagnostic information
   - Helpful for initial hardware verification

6. **`CHANGES_SUMMARY.md`** - This file
   - Overview of all modifications

7. **`.gitignore`** - Git ignore file
   - Excludes compiled binaries and temporary files

---

## 🔧 Hardware Requirements

### Raspberry Pi 5 Setup
- **Main Board:** Raspberry Pi 5
- **Power:** 5V/5A USB-C power supply (27W recommended)
- **OS:** Raspberry Pi OS (64-bit recommended)

### Required Components
1. **MCP3008 ADC** (10-bit, 8-channel SPI ADC)
   - Converts analog sensor signals to digital
   - Connected via SPI (GPIO 8, 9, 10, 11)

2. **5 Analog Sensors:**
   - Temperature sensor (CH0) - e.g., LM35
   - IR flame sensor (CH1)
   - CO2 sensor (CH2)
   - Smoke/Air quality sensor (CH3)
   - CO sensor (CH4)

3. **8x8 LED Matrix** (I2C)
   - For visual alarms
   - Connected to GPIO2 (SDA) and GPIO3 (SCL)

4. **Push Button**
   - For clean program exit
   - Connected to GPIO17

---

## 🚀 Quick Start

```bash
# 1. Setup (one-time)
chmod +x rpi5_setup.sh
sudo ./rpi5_setup.sh
sudo reboot

# 2. Test hardware
make test
sudo ./test_hardware

# 3. Build and run
make clean
make
sudo ./main
```

---

## 📊 Key Differences: BeagleBone vs Raspberry Pi 5

| Feature | BeagleBone Green | Raspberry Pi 5 |
|---------|------------------|----------------|
| **ADC** | Built-in 12-bit (1.8V) | External required (MCP3008: 10-bit, 3.3V) |
| **Compilation** | Cross-compile on PC | Native on device |
| **Pin Config** | `config-pin` utility | `raspi-config` |
| **I2C Bus** | Multiple buses | Usually bus 1 |
| **GPIO Access** | sysfs | sysfs (same) |
| **Processor** | ARM Cortex-A8 (1 GHz) | ARM Cortex-A76 (2.4 GHz) |

---

## ⚙️ Configuration Checklist

- [ ] I2C enabled (`sudo raspi-config`)
- [ ] SPI enabled (`sudo raspi-config`)
- [ ] GPIO17 exported and configured as input
- [ ] MCP3008 ADC connected to SPI pins
- [ ] Sensors connected to ADC channels
- [ ] LED matrix connected to I2C pins
- [ ] Button connected to GPIO17
- [ ] Code compiled successfully
- [ ] Hardware test passes
- [ ] Main program runs

---

## 🔍 Testing Checklist

- [ ] `i2cdetect -y 1` shows LED matrix device
- [ ] `ls /sys/bus/iio/devices/` shows ADC (iio:device0)
- [ ] `cat /sys/class/gpio/gpio17/value` responds to button press
- [ ] `./test_hardware` completes successfully
- [ ] `sudo ./main` displays sensor readings
- [ ] Button press exits program cleanly

---

## ⚠️ Important Notes

### ADC Configuration
The ADC settings in `sensor.h` assume a **MCP3008** with **3.3V reference**.

**If using different ADC:**
- Update `A2D_VOLTAGE_REF_V` to match your reference voltage
- Update `A2D_MAX_READING` to match ADC resolution (e.g., 4095 for 12-bit)

### GPIO Pin Selection
GPIO17 is used for the button by default. **You can use any available GPIO:**
- Update `USERBUTTON` definition in `user.h`
- Export the new GPIO number
- Configure as input

### Sensor Thresholds
Original thresholds in `common.h` were calibrated for BeagleBone's 1.8V/12-bit ADC.

**After hardware setup:**
1. Run the program and observe sensor readings
2. Adjust threshold values in `common.h`:
   - `TEMPPOINT` - Temperature alarm threshold
   - `IRPOINT` - IR flame detection threshold
   - `COPOINT` - CO gas threshold
   - `CO2POINT` - CO2 threshold
   - `SMOKEPOINT` - Smoke detection threshold

---

## 📚 Documentation Structure

```
README.md              - Main documentation (updated for RPi5)
QUICKSTART.md          - Fast setup guide
RPI5_HARDWARE_SETUP.md - Detailed wiring and hardware info
MIGRATION_NOTES.md     - Technical migration details
CHANGES_SUMMARY.md     - This file
```

---

## 🐛 Common Issues

### "Permission denied"
→ Run with `sudo`

### "Unable to open voltage input file"
→ Check SPI enabled and ADC wired correctly

### "Cannot open button GPIO file"
→ Export GPIO: `echo 17 | sudo tee /sys/class/gpio/export`

### LED matrix not responding
→ Check I2C enabled and LED matrix powered

### Sensor readings seem off
→ Calibrate thresholds in `common.h`

**For detailed troubleshooting, see `QUICKSTART.md`**

---

## ✨ What Stayed the Same

The core software architecture is **unchanged**:
- ✅ Multi-threaded design
- ✅ Mutex synchronization
- ✅ Sensor averaging (10-sample buffer)
- ✅ Alarm logic
- ✅ Thread management
- ✅ Program structure

Only hardware interface details were adapted for Raspberry Pi 5.

---

## 🎯 Next Steps

1. **Connect Hardware** following `RPI5_HARDWARE_SETUP.md`
2. **Run Setup** using `rpi5_setup.sh`
3. **Test Components** with `test_hardware`
4. **Compile & Run** the main program
5. **Calibrate Thresholds** based on real sensor data
6. **Deploy** in your target environment

---

## 📞 Support

If you encounter issues:
1. Check the troubleshooting section in `QUICKSTART.md`
2. Review hardware connections in `RPI5_HARDWARE_SETUP.md`
3. Verify system logs: `dmesg | grep -E "i2c|spi|gpio"`
4. Ensure adequate power supply (5V/5A for RPi5)

---

**Your fire detector is now ready to run on Raspberry Pi 5! 🎉**
