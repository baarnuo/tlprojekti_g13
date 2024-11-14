#ifndef ACC_SENSOR_H
#define ACC_SENSOR_H

#define CHANNEL_NO 3

#define ACCELEROMETER DT_ALIAS(accel)
#if DT_NODE_HAS_STATUS(ACCELEROMETER, okay)
const struct device *const accel = DEVICE_DT_GET(ACCELEROMETER);
#else
#error "No accelerometer node"
#endif

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

static int test(void) {
    printk("%d", accel->x_chan);
    
    return 0;
}

/*
static int read_accelerometer(const struct device *device, struct AccelerationData *data)
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

#endif