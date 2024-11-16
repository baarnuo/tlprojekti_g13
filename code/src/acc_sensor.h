#ifndef ACC_SENSOR_H
#define ACC_SENSOR_H

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>

struct Pins {
    uint16_t pin_x;
    uint16_t pin_y;
    uint16_t pin_z;
};

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
    const struct device *device;
    struct Pins pins;
    struct AccelerationData data;
    uint16_t direction;
};

int test(struct Accelerometer acc);
struct Accelerometer initialize_accelerometer(void);

#endif