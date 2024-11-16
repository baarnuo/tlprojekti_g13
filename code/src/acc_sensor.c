#include "acc_sensor.h"

#define ACCELEROMETER DT_ALIAS(accel)
#if DT_NODE_HAS_STATUS(ACCELEROMETER, okay)
const struct device *const acc_device =  DEVICE_DT_GET(ACCELEROMETER);
#else
#error "No accelerometer node"
#endif

#define CHANNEL_NO 3

struct Accelerometer initialize_accelerometer(void) 
{
    struct Accelerometer acc = {
        .device = acc_device,
        .pins.pin_x = DT_PROP(ACCELEROMETER, x_chan),
        .pins.pin_y = DT_PROP(ACCELEROMETER, y_chan),
        .pins.pin_z = DT_PROP(ACCELEROMETER, z_chan),
        .data.x = 0,
        .data.y = 0,
        .data.z = 0,
        .direction = 0,
    };

    return acc;
}

int test(struct Accelerometer acc) {
    printk("Device: %p\n", acc.device);
    printk("Pins: %d, %d, %d\n", acc.pins.pin_x, acc.pins.pin_y, acc.pins.pin_z);
    printk("Values: %d, %d, %d\n", acc.data.x, acc.data.y, acc.data.z);
    
    return 0;
}
/*
int read_accelerometer(const struct  *device, struct AccelerationData *data)
{
    int result = 0;

    result = sensor_sample_fetch(device);
    if (result < 0) {
        printk("Couldn't fetch sample, error %d.\n", result);
        return result;
    }
    
    struct sensor_value readings[3];
    for (size_t i = 0; i < CHANNEL_NO; i++) {
        result = sensor_channel_get(device, channels[i], &readings[i]);
        if (result < 0) {
            printk("Error with reading axis %d, error %d.\n", i, result);
            return result;
        }
    }

    double val_x, val_y, val_z;
    val_x = sensor_value_to_double(&readings[0]);
    val_y = sensor_value_to_double(&readings[1]);
    val_z = sensor_value_to_double(&readings[2]);

    printk("Values: x = %.2f, y = %.2f, z = %.2f.\n", val_x, val_y, val_z);

    return 0;
}
*/