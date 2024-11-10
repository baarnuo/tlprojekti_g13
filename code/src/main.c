#include <stdio.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/sys/util_macro.h>
#include <zephyr/kernel.h>

#include <dk_buttons_and_leds.h>

#include "acc_sensor.h"
#include "acc_service.h"

#define ADVERTISING_INTERVAL 800

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LENGTH (sizeof(DEVICE_NAME) - 1)

#define STATUS_LED DK_LED1
#define CONNECTION_LED DK_LED2
#define DATA_BUTTON DK_BTN1_MSK

#define STACKSIZE 1024
#define PRIORITY 7

#define BLINK_INTERVAL 500

static struct bt_le_adv_param *advertisement_parameters = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY),
    ADVERTISING_INTERVAL, ADVERTISING_INTERVAL + 1, NULL);

int main(void)
{
    bool blink_timer = 1;
    
    while (1) {
        dk_set_led(STATUS_LED, (blink_timer = blink_timer ? 0 : 1));
        k_sleep(K_MSEC(BLINK_INTERVAL));
    }
}

void thread(void)
{
    while (1) {
        simulate_measurement();
        k_sleep(K_MSEC(5000));
    }
}

K_THREAD_DEFINE(thread_id, STACKSIZE, thread, NULL, NULL, NULL, PRIORITY, 0, 0);