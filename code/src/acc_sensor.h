#ifndef ACC_SENSOR_H
#define ACC_SENSOR_H

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>

struct AccelerationData {
    int16_t x;
    int16_t y;
    int16_t z;
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    AHEAD,
    BACK
};

struct Accelerometer {
    struct device *device;
    struct AccelerationData data;
    enum Direction direction;
};

int test(void);

#endif