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
// For direction calculations
#include <math.h>
#include "acc_nn_calc.h"
#include "kmeans.h"

struct AccelerationData {
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint16_t direction;
};

struct DirectionalData {
    uint16_t x_deg;
    uint16_t y_deg;
    uint16_t z_deg;
    uint16_t magnitude;
};

struct AccelerometerMeasurement {
    struct AccelerationData acceleration;
    struct DirectionalData direction;
};

enum Algorithm {
    NONE,
#ifdef  ACC_NN_CALC_H
    NEURAL,
#endif
#ifdef  KMEANS_H
    KMEANS,
#endif
};

// Accelerometer setup and a means of getting a reading
int initialize_accelerometer(void);
struct AccelerometerMeasurement read_data(int algorithm);

#endif