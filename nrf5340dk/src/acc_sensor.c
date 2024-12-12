/*
    An accelerometer reader based on the ADC library available on Zephyr. 
    Connects pins defined by the overlay to adc readers, and provides functions 
    for setting up the pins and taking readings.
*/

#include "acc_sensor.h"

#define ACCELEROMETER DT_ALIAS(accel)
#if DT_NODE_HAS_STATUS(ACCELEROMETER, okay)
const struct device *const acc_device =  DEVICE_DT_GET(ACCELEROMETER);
#else
#error "No accelerometer node"
#endif

// Holds the channels for the three input pins
static const struct adc_dt_spec channels[] = {
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(gy61), 0),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(gy61), 1),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(gy61), 2)
};

// Declarations for debug/test functions
static int calculate_direction(struct AccelerometerMeasurement *measurement, int algorithm);
static int channel_print(void);
static int sequence_print(struct adc_sequence sequence);

// Basic setup
int initialize_accelerometer(void) 
{
    int err = 0;

    // For each channel (= pin)
    for (int i = 0; i < ARRAY_SIZE(channels); i++) {
		// Check for device readiness
        if (!device_is_ready(channels[i].dev)) {
			printk("Channel %d isn't ready?\n", i);
			return -1;
		}

        // Device tree setup(?)
		err = adc_channel_setup_dt(&channels[i]);
		if (err < 0) {
			printk("Channel %d failed with setup, err %d.\n", i, err);
			return -1;
		}
	}

    // Debug
    channel_print();

    return 0;
}

// Read the accelerometer, return struct holding structs of readings and derived angles
struct AccelerometerMeasurement read_data(int algorithm) 
{
    struct AccelerometerMeasurement data = {
        .acceleration = {
            .x = 0,
            .y = 0,
            .z = 0,
            .direction = 0
        },
        .direction = {
            .x_deg = 0,
            .y_deg = 0,
            .z_deg = 0,
            .magnitude = 0
        }
    };
    
    int16_t temp = 0;
    int32_t large_temp = 0;
    // This thing gets data from the devicetree at adc_sequence_init_dt below and holds things
    // like resolution, oversampling value and such
    struct adc_sequence temp_seq = {
        .buffer = &temp,
        .buffer_size = sizeof(temp)
    };
    int err;

    for (int i = 0; i < ARRAY_SIZE(channels); i++) {        
        // Setup of temp_seq
        err = adc_sequence_init_dt(&channels[i], &temp_seq);
        if (err < 0) {
            printk("Failed to set up channel %d, aborting measurement.Err %d.\n", i+1, err);
            return data;
        }

        // Something something read request
        err = adc_read(channels[i].dev, &temp_seq);
        if (err < 0) {
            printk("Failed to read channel %d, aborting measurement.Err %d.\n", i+1, err);
            return data;
        }

        // Converts whatever magic happened inside the computer to a millivolt value
        // (accelerometer should provide ~1600 for still and ~1900 for the side affected by gravity)
        err = adc_raw_to_millivolts_dt(&channels[i], &large_temp);
        if (err < 0) {
            printk("Error %d with parsing channel %d, aborting measurement.\n", err, i+1);
            return data;
        }

        // Assign data to the bundle of measurements
        switch (i) {
            case 0:
                data.acceleration.x = temp;
                break;
            case 1:
                data.acceleration.y = temp;
                break;
            case 2:
                data.acceleration.z = temp;
                break;
        }
    }

    // 
    int res = calculate_direction(&data, algorithm);
    if (res < 0 || data.acceleration.direction == 0) {
        printk("Failed to calculate direction.\n");
    }

    return data;
}

// Calculates the strength of gravity affecting the device and its direction
static int calculate_direction(struct AccelerometerMeasurement *measurement, int algorithm)
{   
    // Base = voltage when an axis is at 0G, high = voltage when axis is at 1G, one_g = 1G
    // (these are just estimates, the exact value could be gotten through an average of results)
    const uint16_t base = 1910, high = 2275, one_g = high - base;
    const double pi = 3.1415926;

    int16_t x = measurement->acceleration.x, y = measurement->acceleration.y, z = measurement->acceleration.z;
    // How much each axel deviates from the baseline
    int16_t real[] = {x - base, y - base, z - base};
    // Length of the vector (aka strength of acceleration)
    uint16_t mag = sqrt(pow(real[0], 2) + pow(real[1], 2) + pow(real[2], 2));
    // Angle of each axis in degrees
    double deg[3];

    uint16_t final[3];

    for (int i = 0; i < 3; i++) {
        deg[i] = (acos((double)real[i] / mag) * 180 / pi);
        final[i] = (uint16_t)deg[i];
    }

    uint16_t g_val = (uint16_t)((1000*mag)/one_g);

    // Prints
    /*printk("x: %dmV, %d°, y: %dmV, %d°, z: %dmV, %d°, magnitude: %d, acceleration (thousandths of 1G): %d.\n", 
        x, final[0], y, final[1], z, final[2], mag, g_val);*/
    
    // Calculating the acceleration.direction value (one of six)
    uint16_t direction;
    switch (algorithm) {
        #ifdef  ACC_NN_CALC_H
        // Calculated with a convolution neural network because the teacher wanted us to include one
        case NEURAL:
            direction = nn_direction(x, y, z);
            break;
        #endif
        #ifdef  KMEANS_H
        case KMEANS:
            direction = kmeans_direction(x, y, z);
            break;
        #endif
        case NONE:
        default:
            if (final[0] < 45) {direction = 1;}
            // X points down
            else if (135 < final[0]) {direction = 2;}
            // Y points up
            else if (final[1] < 45) {direction = 3;}
            // Y points down
            else if (135 < final[1]) {direction = 4;}
            // Z points up
            else if (final[2] < 45) {direction = 5;}
            // Z points down
            else if (135 < final[2]) {direction = 6;}
            break;
    }
    //printk("Returned from nn calc: %d", direction);

    // Assign all values
    measurement->direction.x_deg = final[0];
    measurement->direction.y_deg = final[1];
    measurement->direction.z_deg = final[2];
    measurement->direction.magnitude = g_val;
    measurement->acceleration.direction = direction;    

    return 0;
}

// Debug
static int channel_print(void) 
{
    printk("channel_print():\n");
    for (int i = 0; i < ARRAY_SIZE(channels); i++) {
        printk("    adc_dt_spec structure, channel %d = \n", i);
        printk("    Device pointer = %p\n", channels[i].dev);
        printk("    Channel Id = %d\n", channels[i].channel_id);
    }

    return 0;
}

// Debug
static int sequence_print(struct adc_sequence sequence) 
{
    printk("sequence_print():\n");
    printk("    Channels: %d\n", sequence.channels);
    printk("    Buffer: %p\n", sequence.buffer);
    printk("    Buffer size: %d\n", sequence.buffer_size);
    printk("    Resolution: %d\n", sequence.resolution);
    printk("    Oversampling: %d\n", sequence.oversampling);

    return 0;
}