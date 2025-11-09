#!/bin/bash
# Setup script for Raspberry Pi 5 Fire Detector
# Run with: sudo ./rpi5_setup.sh

echo "=== Raspberry Pi 5 Fire Detector Setup ==="
echo ""

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo "ERROR: Please run as root (use sudo)"
    exit 1
fi

# Enable I2C interface
echo "Enabling I2C interface..."
if ! raspi-config nonint do_i2c 0; then
    echo "Warning: Could not enable I2C automatically"
    echo "Please enable manually: sudo raspi-config -> Interface Options -> I2C"
fi

# Enable SPI interface for ADC
echo "Enabling SPI interface..."
if ! raspi-config nonint do_spi 0; then
    echo "Warning: Could not enable SPI automatically"
    echo "Please enable manually: sudo raspi-config -> Interface Options -> SPI"
fi

# Export GPIO for button (GPIO17)
echo "Setting up GPIO17 for button input..."
GPIO_NUM=17
GPIO_PATH="/sys/class/gpio/gpio${GPIO_NUM}"

if [ ! -d "$GPIO_PATH" ]; then
    echo $GPIO_NUM > /sys/class/gpio/export
    sleep 0.5
fi

if [ -d "$GPIO_PATH" ]; then
    echo "in" > ${GPIO_PATH}/direction
    echo "Setting pull-up resistor..."
    # Configure pull-up using raspi-gpio if available
    if command -v raspi-gpio &> /dev/null; then
        raspi-gpio set $GPIO_NUM ip pu
    fi
    echo "GPIO17 configured as input with pull-up"
else
    echo "Warning: Could not configure GPIO17"
fi

# Install required packages if not present
echo ""
echo "Checking for required packages..."
if ! dpkg -l | grep -q i2c-tools; then
    echo "Installing i2c-tools..."
    apt-get update
    apt-get install -y i2c-tools
fi

if ! dpkg -l | grep -q build-essential; then
    echo "Installing build-essential..."
    apt-get install -y build-essential
fi

echo ""
echo "=== Setup Complete ==="
echo ""
echo "Next steps:"
echo "1. Connect your hardware (sensors, ADC, LED matrix, button)"
echo "2. Verify I2C devices: i2cdetect -y 1"
echo "3. Check SPI devices: ls /sys/bus/iio/devices/"
echo "4. Compile: make clean && make"
echo "5. Run: sudo ./main"
echo ""
echo "Note: You may need to reboot for all changes to take effect"
