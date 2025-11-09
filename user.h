// Header file for user button

#ifndef USER_H_
#define USER_H_

#include "common.h"
// Raspberry Pi 5: GPIO17 is a common button pin
// Change this based on your actual GPIO pin connection
// For GPIO17, the path would be /sys/class/gpio/gpio17/value
// You need to export the GPIO first: echo 17 > /sys/class/gpio/export
#define USERBUTTON "/sys/class/gpio/gpio17/value"

// Function to get value of USER button
int UserButtonValue();

// Detects when USER button is pressed and ends program
void *exitProgram(void *arg);

#endif
