/*
    Accelerometer service used for advertising the x, y, z and direction values collected from our little device.
    
    Used in conjunction with our sensor, so the sensor is included here and read_and_notify and read_and_indicate
    are built-in.
*/

#include "acc_service.h"

// Whether notification messages should get sent
static bool notify_enabled;
static bool indicate_enabled;
// A collection of parameters concerning indication settings
static struct bt_gatt_indicate_params indication_parameters;

// Holds current reading data
struct AccelerationData reading;

// Declarations
static int accelerometer_notification(void);
static int accelerometer_indication(void);
static void indication_callback(struct bt_conn *connection, struct bt_gatt_indicate_params *parameters, uint8_t error);

// Take accelerometer reading and send a notification
int read_and_notify(void) 
{
    reading = read_data();
    if (reading.direction == 0) {
        //printk("read_and_notify(): error with getting a reading. No notification sent.\n");
        return -1;
    }

    //printk("Values: x = %d, y = %d, z = %d, dir = %d\n", reading.x, reading.y, reading.z, reading.direction);

    int err = accelerometer_notification();
    if (err < 0) {
        //printk("read_and_notify(): error %d sending notification.\n", err);
        return err;
    }
    return 0;
}

// Take accelerometer reading and send an indication
int read_and_indicate(void) 
{
    reading = read_data();
    if (reading.direction == 0) {
        //printk("read_and_indicate(): error with getting a reading. No indication sent.\n");
        return -1;
    }

    int err = accelerometer_indication();
    if (err < 0) {
        //printk("read_and_indicate(): error %d sending indication.\n", err);
        return err;
    }
    return 0;
}

// Internal workings below

// What gets called when the device you're connected to wants to change notification settings(?)
static void notification_configuration_change(const struct bt_gatt_attr *attributes, uint16_t value)
{
    notify_enabled = (value == BT_GATT_CCC_NOTIFY);

    printk("Notification configuration changed, now %d.\n", notify_enabled);
}

// What gets called when the device you're connected to wants to change indication settings(?)
static void indication_configuration_change(const struct bt_gatt_attr *attributes, uint16_t value)
{
    indicate_enabled = (value == BT_GATT_CCC_INDICATE);

    printk("Indication configuration changed, now %d.\n", indicate_enabled);
}

// Setting up the service and attributes holding our data
BT_GATT_SERVICE_DEFINE(
    accelerometer, 
    // Sets up the service for acceleration measurements
    BT_GATT_PRIMARY_SERVICE(ACCELERATION_SERVICE_UUID),
    //
    BT_GATT_CHARACTERISTIC(ACCELERATION_NOTIFICATION_UUID, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, &reading),
    BT_GATT_CCC(notification_configuration_change, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    BT_GATT_CHARACTERISTIC(ACCELERATION_INDICATION_UUID, BT_GATT_CHRC_INDICATE, BT_GATT_PERM_READ, NULL, NULL, &reading),
    BT_GATT_CCC(indication_configuration_change, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

// Sends the reading data to a connected device that's subscribed to notifications
static int accelerometer_notification(void)
{
    if (!notify_enabled) {
        return -EACCES;
    }
    // Looks up the acceleration values characteristic (or attribute?) based on its assigned ID
    return bt_gatt_notify(NULL, bt_gatt_find_by_uuid(NULL, 0, ACCELERATION_NOTIFICATION_UUID), &reading, sizeof(reading));
}

// Sends the reading data to a connected device that's subscribed to indications
static int accelerometer_indication(void)
{
    if(!indicate_enabled) {
        return -EACCES;
    }

    indication_parameters.attr = bt_gatt_find_by_uuid(NULL, 0, ACCELERATION_INDICATION_UUID);
	indication_parameters.func = indication_callback;
	indication_parameters.destroy = NULL;
	indication_parameters.data = &reading;
	indication_parameters.len = sizeof(reading);

    return bt_gatt_indicate(NULL, &indication_parameters);
}

// Gets called when an indication has been sent and is answered to
static void indication_callback(struct bt_conn *connection, struct bt_gatt_indicate_params *parameters, uint8_t error) 
{
    printk("Indication callback\n");
}