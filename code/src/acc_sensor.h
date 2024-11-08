#ifndef ACC_SENSOR_H
#define ACC_SENSOR_H

#include <zephyr/drivers/sensor.h>

struct AccelerationData {
    uint16_t x_raw;
    uint16_t x_acceleration;
    uint16_t y_raw;
    uint16_t y_acceleration;
    uint16_t z_raw;
    uint16_t z_acceleration;
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    AHEAD,
    BACK
};

#endif