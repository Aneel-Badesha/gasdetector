// Simple hardware test utility for Raspberry Pi 5
// Compile: gcc -o test_hardware test_hardware.c
// Run: sudo ./test_hardware

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define BUTTON_GPIO "/sys/class/gpio/gpio17/value"
#define ADC_CH0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

void test_button() {
    printf("\n=== Testing GPIO Button (GPIO17) ===\n");
    printf("Press the button (Ctrl+C to stop)...\n");
    
    for (int i = 0; i < 50; i++) {
        FILE *f = fopen(BUTTON_GPIO, "r");
        if (!f) {
            printf("ERROR: Cannot open button GPIO file.\n");
            printf("Make sure GPIO is exported: echo 17 | sudo tee /sys/class/gpio/export\n");
            return;
        }
        
        int value;
        fscanf(f, "%d", &value);
        fclose(f);
        
        if (value == 0) {
            printf("Button PRESSED!\n");
        } else {
            printf("Button released (value: %d)\r", value);
            fflush(stdout);
        }
        
        usleep(100000); // 100ms
    }
    printf("\n");
}

void test_adc() {
    printf("\n=== Testing ADC (Channel 0) ===\n");
    printf("Reading 10 samples...\n");
    
    for (int i = 0; i < 10; i++) {
        FILE *f = fopen(ADC_CH0, "r");
        if (!f) {
            printf("ERROR: Cannot open ADC file.\n");
            printf("Possible reasons:\n");
            printf("  1. SPI not enabled (run: sudo raspi-config)\n");
            printf("  2. ADC not connected\n");
            printf("  3. Wrong device path (check: ls /sys/bus/iio/devices/)\n");
            return;
        }
        
        int raw_value;
        fscanf(f, "%d", &raw_value);
        fclose(f);
        
        // Convert to voltage (assuming 3.3V reference, 10-bit ADC)
        double voltage = 3.3 * ((double)raw_value / 1023.0);
        
        printf("Sample %d: Raw=%4d, Voltage=%.3fV\n", i+1, raw_value, voltage);
        sleep(1);
    }
}

void test_i2c() {
    printf("\n=== Testing I2C ===\n");
    printf("Scanning for I2C devices...\n");
    
    int result = system("which i2cdetect > /dev/null 2>&1");
    if (result != 0) {
        printf("i2cdetect not found. Installing i2c-tools...\n");
        system("sudo apt-get install -y i2c-tools");
    }
    
    printf("\nI2C Bus 1 scan:\n");
    system("i2cdetect -y 1");
}

void print_system_info() {
    printf("\n=== System Information ===\n");
    
    printf("\nI2C Status:\n");
    system("ls -l /dev/i2c* 2>/dev/null || echo 'No I2C devices found'");
    
    printf("\nSPI Status:\n");
    system("ls -l /dev/spidev* 2>/dev/null || echo 'No SPI devices found'");
    
    printf("\nIIO Devices (ADC):\n");
    system("ls -l /sys/bus/iio/devices/ 2>/dev/null || echo 'No IIO devices found'");
    
    printf("\nGPIO Exports:\n");
    system("ls /sys/class/gpio/ | grep gpio || echo 'No GPIOs exported'");
}

int main(int argc, char *argv[]) {
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║  Raspberry Pi 5 Fire Detector Hardware Test  ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    if (geteuid() != 0) {
        printf("\nWARNING: Not running as root. Some tests may fail.\n");
        printf("Run with: sudo ./test_hardware\n");
    }
    
    print_system_info();
    
    if (argc > 1) {
        if (strcmp(argv[1], "button") == 0) {
            test_button();
        } else if (strcmp(argv[1], "adc") == 0) {
            test_adc();
        } else if (strcmp(argv[1], "i2c") == 0) {
            test_i2c();
        } else {
            printf("\nUsage: %s [button|adc|i2c]\n", argv[0]);
            printf("Run without arguments to test all\n");
        }
    } else {
        // Test all
        test_i2c();
        test_adc();
        test_button();
    }
    
    printf("\n=== Test Complete ===\n");
    printf("If any tests failed, check:\n");
    printf("  1. Hardware connections (see RPI5_HARDWARE_SETUP.md)\n");
    printf("  2. Interface configuration (run: sudo ./rpi5_setup.sh)\n");
    printf("  3. Power supply (5V/5A recommended for RPi5)\n");
    
    return 0;
}
