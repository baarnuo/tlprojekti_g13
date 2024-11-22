/*
    An accelerometer reader based on the ADC library available on Zephyr. 
    Connects pins defined by the overlay to adc readers, and provides functions 
    for setting up the pins and taking readings.
*/

#ifndef ACC_SENSOR_H
#define ACC_SENSOR_H

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>

struct AccelerationData {
    int16_t x;
    int16_t y;
    int16_t z;
    int16_t direction;
};

// Accelerometer setup and a means of getting a reading
int initialize_accelerometer(void);
struct AccelerationData read_data(void);

#endif