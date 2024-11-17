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

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    AHEAD,
    BACK
};

int initialize_accelerometer(void);
int read_data(struct AccelerationData *data);

#endif