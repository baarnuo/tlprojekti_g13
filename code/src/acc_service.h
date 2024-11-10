#ifndef ACC_SERVICE_H
#define ACC_SERVICE_H

#include <stdbool.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>
#include "acc_sensor.h"

// UUIDs for our data (the whole service, x y and z directional values and the direction of the device)
#define ACCELERATION_SERVICE_UUID_VALUE BT_UUID_128_ENCODE(0xf50234f0, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)
#define ACCELERATION_VALUES_UUID_VALUE BT_UUID_128_ENCODE(0xf50234f1, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)
#define ACCELERATION_DIRECTION_UUID_VALUE BT_UUID_128_ENCODE(0xf50234f2, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)

#define ACCELERATION_SERVICE_UUID BT_UUID_DECLARE_128(ACCELERATION_SERVICE_UUID_VALUE)
#define ACCELERATION_VALUES_UUID BT_UUID_DECLARE_128(ACCELERATION_VALUES_UUID_VALUE)
#define ACCELERATION_DIRECTION_UUID BT_UUID_DECLARE_128(ACCELERATION_DIRECTION_UUID_VALUE)

void simulate_measurement(void);
int accelerometer_notification(void);
int accelerometer_indication(void);

#endif