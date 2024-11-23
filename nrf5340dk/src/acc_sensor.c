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
static int calculate_direction(struct AccelerationData data);
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

// Read the accelerometer, return struct holding x, y and z values and a direction
struct AccelerationData read_data(void) 
{
    struct AccelerationData data = {
        .x = 0,
        .y = 0,
        .z = 0,
        .direction = 0
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
                data.x = temp;
                break;
            case 1:
                data.y = temp;
                break;
            case 2:
                data.z = temp;
                break;
        }
    }

    // At this point this doesn't do much
    data.direction = calculate_direction(data);

    return data;
}

// Test/stepping stone
static int calculate_direction(struct AccelerationData data)
{
    int dir = (data.direction > 5 ? 1 : data.direction + 1);
    return dir;
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