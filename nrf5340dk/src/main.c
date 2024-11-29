#include <stdio.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/util_macro.h>
#include <zephyr/kernel.h>

#include <dk_buttons_and_leds.h>

#include "acc_service.h"

// How often advertisements are sent out (in units of 0.625 milliseconds, equates to 500 milliseconds)
#define ADVERTISING_INTERVAL 800

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LENGTH (sizeof(DEVICE_NAME) - 1)

// Led that blinks when the device is running, and
// the led that is on when the device is connected via BLE
#define STATUS_LED DK_LED1
#define CONNECTION_LED DK_LED2
// Led for indicating the direction for confusion matrix measurements, and
// a led that turns on when the confusion matrix measurement is taking place
#define CM_DIRECTION_LED DK_LED3
#define CM_MEASUREMENT_LED DK_LED4
// Button for sending an indication
#define DATA_BUTTON DK_BTN1_MSK
// The button switching between k-means and neural network for directional calculations,
// the button for choosing the true direction for the confusion matrix, and
// the button for starting the confusion matrix measurement
#define ALGORITHM_BUTTON DK_BTN2_MSK
#define CM_DIRECTION_BUTTON DK_BTN3_MSK
#define CM_START_BUTTON DK_BTN4_MSK

static enum Algorithm chosen_algorithm;
static int true_direction;

// Data for the thread running the measurements
#define STACKSIZE 1024
#define PRIORITY 7

#define BLINK_INTERVAL 500
#define MEASUREMENT_INTERVAL 10000

// 
static const struct bt_data advertisement_data[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LENGTH),
    // Our accelerometer service's UUID (but in a weird format)
    BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xf0, 0x34),
};

// 
static const struct bt_data scan_data[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, sizeof(ACCELERATION_NOTIFICATION_UUID)),
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, sizeof(ACCELERATION_INDICATION_UUID))
};

// Advertising: can be connected to, advertises twice a second, not directed
static struct bt_le_adv_param *advertisement_parameters = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY),
    ADVERTISING_INTERVAL, ADVERTISING_INTERVAL + 1, NULL);

// Bluetooth setup:

// When a bluetooth connection is formed (or tried to)
static void on_connect(struct bt_conn *conn, uint8_t error)
{
    if (error) {
        printk("Something went wrong with connection, error %d.\n", error);
        return;
    }

    printk("Conncted.");
    dk_set_led_on(CONNECTION_LED);
}

// When a bluetooth connection is formed (or tried to)
static void on_disconnect(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected, reason %d.\n", reason);
    dk_set_led_off(CONNECTION_LED);
}

// The functions that are called when a connection starts and ends
struct bt_conn_cb connection_callbacks = {
    .connected = on_connect,
    .disconnected = on_disconnect,
};

// ISR for when a button is pressed(?)
static void button_press(uint32_t state, uint32_t change)
{
    // Send an indication with accelerometer data
    if ((state & change) & DATA_BUTTON) {
        read_and_indicate(chosen_algorithm);
    }

    // Change the algorithm for the directional measurements
    if ((state & change) & ALGORITHM_BUTTON) {
        int maxNo = 0;
        #ifdef ACC_CNN_CALC_H
        maxNo++;
        #endif
        #ifdef KMEANS_H
        maxNo++;
        #endif
        chosen_algorithm = (chosen_algorithm == maxNo ? 0 : chosen_algorithm + 1);
    }

    // Switch the true direction for the confusion matrix measurement
    if ((state & change) & CM_DIRECTION_BUTTON) {
        // Moves the directional counter ahead one step (0b0000 0001 to 0b0000 0011, for example),
        // Resets to 1 on 
        true_direction = (true_direction == 6 ? 1 : true_direction + 1);
    }

    // Start a confusion matrix measurement
    if ((state & change) & CM_START_BUTTON) {
        confusion_matrix_measurement(chosen_algorithm, true_direction);
    }
}

// Setup of advertising and hardware, then blinking a led
int main(void)
{
    chosen_algorithm = NONE;
    true_direction = 1;

    int16_t blink_counter = 1;
    int error;

    printk("Setting up the device.\n");

    // Led setup
    error = dk_leds_init();
    if (error) {
        printk("Led setup failed, error code %d.\n", error);
        return -1;
    }
    printk("Leds initialized.\n");

    // Button setup
    error = dk_buttons_init(button_press);
    if (error) {
        printk("Button setup failed, error code %d.\n", error);
        return -1;
    }
    printk("Buttons initialized.\n");

    // Bluetooth initialization
    error = bt_enable(NULL);
    if (error) {
        printk("Bluetooth setup failed, error code %d.\n", error);
        return -1;
    }
    printk("Bluetooth initialized.\n");

    // Bluetooth startup
    error = bt_le_adv_start(advertisement_parameters, advertisement_data, ARRAY_SIZE(advertisement_data),
        scan_data, ARRAY_SIZE(scan_data));
    if (error) {
        printk("Bluetooth startup failed, error code %d.\n", error);
        return -1;
    }
    printk("Bluetooth started.\n");

    printk("Device set up succesfully.\n");
    
    while (1) {
        // blink_counter manipulation: the counter should 
        // a. blink the status led on-to-off-to-on every cycle
        // b. be able to blink x times per cycle of 8 to indicate the True Direction(tm)
        // So it's a bit shift thing 
        blink_counter = (blink_counter & 0x8000 ? 1 : blink_counter << 1);
        // Blink the status led to let people know the device is on and running as it should
        dk_set_led(STATUS_LED, ((blink_counter & 0x5555) > 0));
        // The led indicating which direction is set as true for confusion matrix measurements
        // (number of consecutive blinks), as well as the chosen algorithm 
        // (nothing for none, blinks for k-means, "inverted blinks" for neural)
        uint16_t comp = 0x0000;
        switch (chosen_algorithm) {
            case NONE:
                comp = 0xFFFF;
                break;
            #ifdef ACC_CNN_CALC_H
            case NEURAL:
                comp = ~(0x5555 >> (2 * (8 - true_direction)));
                break;
            #endif 
            #ifdef KMEANS_H
            case KMEANS:
                comp = 0x5555 << (2 * true_direction);
                break;
            #endif
        } 

        dk_set_led(CM_DIRECTION_LED, ((blink_counter & comp) > 0));
        // Take a nap waiting for the next blink interval
        k_sleep(K_MSEC(BLINK_INTERVAL));
    }
}

// A separate thread to take measurements independetly of the main loop
void measurement_thread(void)
{
    int error = initialize_accelerometer();
    if (error) {
        printk("Failed to initialize accelerometer, error code %d.\n", error);
        return;
    }
    
    while (1) {
        read_and_notify(chosen_algorithm);
        k_sleep(K_MSEC(MEASUREMENT_INTERVAL));
    }
}

K_THREAD_DEFINE(thread_id, STACKSIZE, measurement_thread, NULL, NULL, NULL, PRIORITY, 0, 0);