#ifndef ACC_SENSOR_H
#define ACC_SENSOR_H

#include <zephyr/drivers/sensor.h>

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

#endif