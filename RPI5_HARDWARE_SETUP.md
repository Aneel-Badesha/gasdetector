# Raspberry Pi 5 Hardware Configuration Guide

## Pin Connections

### I2C (8x8 LED Matrix)
- **GPIO 2** (Pin 3): SDA (I2C Data)
- **GPIO 3** (Pin 5): SCL (I2C Clock)
- **3.3V** (Pin 1 or 17): VCC
- **GND** (Pin 6, 9, 14, 20, 25, 30, 34, 39): Ground

### SPI (MCP3008 ADC)
- **GPIO 8** (Pin 24): CE0 (Chip Enable)
- **GPIO 9** (Pin 21): MISO (Master In Slave Out)
- **GPIO 10** (Pin 19): MOSI (Master Out Slave In)
- **GPIO 11** (Pin 23): SCLK (Clock)
- **3.3V** (Pin 1 or 17): VCC
- **GND** (Pin 6, 9, 14, 20, 25, 30, 34, 39): Ground

### GPIO (User Button)
- **GPIO 17** (Pin 11): Button input (with pull-up resistor)
- **GND** (any ground pin): Button ground

## MCP3008 ADC Wiring

### MCP3008 Pinout (16-pin DIP)
```
    CH0  1  ●  16  VDD  -> 3.3V
    CH1  2     15  VREF -> 3.3V
    CH2  3     14  AGND -> GND
    CH3  4     13  CLK  -> GPIO 11 (SCLK)
    CH4  5     12  DOUT -> GPIO 9 (MISO)
    CH5  6     11  DIN  -> GPIO 10 (MOSI)
    CH6  7     10  CS   -> GPIO 8 (CE0)
    CH7  8      9  DGND -> GND
```

### Sensor Connections to MCP3008
- **CH0 (Pin 1)**: Temperature Sensor (LM35 or similar)
- **CH1 (Pin 2)**: IR Flame Sensor
- **CH2 (Pin 3)**: CO2 Sensor (analog output)
- **CH3 (Pin 4)**: Smoke/Air Quality Sensor
- **CH4 (Pin 5)**: CO Sensor (analog output)
- **CH5-7**: Available for additional sensors

## Sensor Specifications

### Typical Analog Sensors
1. **Temperature Sensor (LM35)**
   - Output: 10mV per °C
   - Range: 0-100°C
   - Supply: 4-30V (use 3.3V or 5V)

2. **IR Flame Sensor**
   - Output: Analog voltage (typically 0-3.3V)
   - Detection range: Typically 60-100cm
   - Frequency: ~760-1100 nm

3. **MQ Series Gas Sensors**
   - MQ-7: CO (Carbon Monoxide)
   - MQ-135: Air Quality (CO2, NH3, NOx, etc.)
   - Output: Analog voltage
   - Requires warm-up time (24-48 hours for accurate readings)

## Voltage Considerations

### Important Notes
- Raspberry Pi GPIO is **3.3V tolerant** - do not exceed 3.3V on any GPIO pin
- MCP3008 reference voltage (VREF) determines measurement range
  - If VREF = 3.3V, ADC measures 0-3.3V range
  - If VREF = 5V (with proper level shifting), ADC measures 0-5V range
- Most sensors can operate at 3.3V or 5V - check datasheets
- If sensors output 5V, use voltage divider or level shifter before MCP3008

### Example Voltage Divider (5V sensor to 3.3V ADC)
```
Sensor Output (5V max) ---[R1: 10kΩ]---+--- To MCP3008 Channel
                                       |
                                   [R2: 20kΩ]
                                       |
                                      GND
```
This gives Vout = Vin × (R2 / (R1 + R2)) = 5V × 0.666 = 3.33V max

## I2C LED Matrix

### Common 8x8 I2C LED Matrix Modules
- HT16K33-based modules (common)
- MAX7219-based modules (SPI, not I2C - different interface)

### I2C Address Detection
After connecting the LED matrix, run:
```bash
i2cdetect -y 1
```
Look for the device address (typically 0x70 for HT16K33)

## Testing Hardware

### Test I2C Connection
```bash
# List I2C buses
i2cdetect -l

# Scan for I2C devices on bus 1
i2cdetect -y 1
```

### Test SPI/ADC
```bash
# Check if SPI devices are present
ls /sys/bus/iio/devices/

# If MCP3008 driver is loaded, you should see iio:device0
# Read raw value from channel 0
cat /sys/bus/iio/devices/iio:device0/in_voltage0_raw
```

### Test GPIO Button
```bash
# Check GPIO value (should be 1 when not pressed, 0 when pressed)
cat /sys/class/gpio/gpio17/value
```

## Troubleshooting

### SPI ADC Not Detected
1. Ensure SPI is enabled: `sudo raspi-config`
2. Check wiring connections
3. May need to load kernel module: `sudo modprobe spi-bcm2835`
4. For MCP3008, may need: `sudo modprobe mcp320x`

### I2C Device Not Found
1. Ensure I2C is enabled: `sudo raspi-config`
2. Check wiring (especially SDA/SCL)
3. Try different I2C bus: `i2cdetect -y 0` (older Pi models)
4. Verify LED matrix has power

### Button Not Working
1. Verify GPIO export: `ls /sys/class/gpio/gpio17`
2. Check direction: `cat /sys/class/gpio/gpio17/direction` (should be "in")
3. Test manually: `cat /sys/class/gpio/gpio17/value`
4. Verify physical connection

## Power Requirements

- Raspberry Pi 5: 5V/5A USB-C power supply (27W recommended)
- Total system draw depends on:
  - Number of sensors
  - LED matrix brightness
  - Additional peripherals
  
Ensure adequate power supply for stable operation.
