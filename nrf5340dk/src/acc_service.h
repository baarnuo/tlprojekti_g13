/*
    Accelerometer service used for advertising the x, y, z and direction values collected from our little device.
    
    Used in conjunction with our sensor, so the sensor is included here and read_and_notify and read_and_indicate
    are built-in.
*/

#ifndef ACC_SERVICE_H
#define ACC_SERVICE_H

#include <stdbool.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>
#include "acc_sensor.h"

// UUID creation
#define ACCELERATION_SERVICE_UUID_VALUE BT_UUID_128_ENCODE(0xf50234f0, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)
#define ACCELERATION_NOTIFICATION_UUID_VALUE BT_UUID_128_ENCODE(0xf50234f1, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)
#define ACCELERATION_INDICATION_UUID_VALUE BT_UUID_128_ENCODE(0xf50234f2, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)

#define ACCELERATION_SERVICE_UUID BT_UUID_DECLARE_128(ACCELERATION_SERVICE_UUID_VALUE)
#define ACCELERATION_NOTIFICATION_UUID BT_UUID_DECLARE_128(ACCELERATION_NOTIFICATION_UUID_VALUE)
#define ACCELERATION_INDICATION_UUID BT_UUID_DECLARE_128(ACCELERATION_INDICATION_UUID_VALUE)


// The two functions you can use to collect and send data
int read_and_notify(void);
int read_and_indicate(void);

#endif