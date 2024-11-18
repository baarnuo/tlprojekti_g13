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

#define STATUS_LED DK_LED1
#define CONNECTION_LED DK_LED2
#define DATA_BUTTON DK_BTN1_MSK

// Data for the thread running the measurements
#define STACKSIZE 1024
#define PRIORITY 7

#define BLINK_INTERVAL 500
#define MEASUREMENT_INTERVAL 5000

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
    if (change & DATA_BUTTON) {
        read_and_indicate();
        
        // Debounce
        k_sleep(K_MSEC(1));
    }
}

// Setup of advertising and hardware, then blinking a led
int main(void)
{
    bool blink_timer = 1;
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
    
    // Blink the status led to let people know the device is on and running as it should
    while (1) {
        dk_set_led(STATUS_LED, (blink_timer = blink_timer ? 0 : 1));
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
        read_and_notify();
        k_sleep(K_MSEC(MEASUREMENT_INTERVAL));
    }
}

K_THREAD_DEFINE(thread_id, STACKSIZE, measurement_thread, NULL, NULL, NULL, PRIORITY, 0, 0);