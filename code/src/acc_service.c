#include "acc_service.h"

// Whether notification messages should get sent
static bool notify_enabled;
static bool indicate_enabled;
// A collection of parameters concerning indication settings
static struct bt_gatt_indicate_params indication_parameters;

// X, Y and Z axle values, the rotation of the device
struct AccelerationData accelerometer_values;
struct AccelerationData accelerometer_results;
enum Direction direction;

void simulate_measurement(void)
{
    accelerometer_values.x = 1590 + (rand() % 20);
    accelerometer_values.y = 1590 + (rand() % 20);
    accelerometer_values.z = 1890 + (rand() % 20);

    accelerometer_results.x = (accelerometer_values.x - 1600) * 32;   
    accelerometer_results.y = (accelerometer_values.y - 1600) * 32;   
    accelerometer_results.z = (accelerometer_values.z - 1600) * 32;

    accelerometer_raw_notification();
    accelerometer_calculated_notification();

    // Testing purposes
    simulate_direction();

    printk("x: %d, y: %d, z: %d, x_acc: %d, y_acc: %d, z_acc: %d\n", 
        accelerometer_values.x, accelerometer_values.y, accelerometer_values.z, 
        accelerometer_results.x, accelerometer_results.y, accelerometer_results.z);
}

void simulate_direction(void) 
{
    direction = rand() % 6;
}

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
    // Notifications for the raw values
    BT_GATT_CHARACTERISTIC(ACCELERATION_VALUES_UUID, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, &accelerometer_values),
    BT_GATT_CCC(notification_configuration_change, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    // Notifications for the acceleration values
    BT_GATT_CHARACTERISTIC(ACCELERATION_RESULTS_UUID, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, &accelerometer_results),
    BT_GATT_CCC(notification_configuration_change, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    // Indication for the directional value
    BT_GATT_CHARACTERISTIC(ACCELERATION_DIRECTION_UUID, BT_GATT_CHRC_INDICATE, BT_GATT_PERM_READ, NULL, NULL, &direction),
    BT_GATT_CCC(indication_configuration_change, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

int accelerometer_raw_notification(void)
{
    if (!notify_enabled) {
        return -EACCES;
    }
    // Looks up the acceleration values characteristic (or attribute?) based on its assigned ID
    return bt_gatt_notify(NULL, bt_gatt_find_by_uuid(NULL, 0, ACCELERATION_VALUES_UUID), &accelerometer_values, sizeof(accelerometer_values));
}

int accelerometer_calculated_notification(void)
{
    if (!notify_enabled) {
        return -EACCES;
    }
    // Looks up the acceleration values characteristic (or attribute?) based on its assigned ID
    return bt_gatt_notify(NULL, bt_gatt_find_by_uuid(NULL, 0, ACCELERATION_RESULTS_UUID), &accelerometer_results, sizeof(accelerometer_results));
}

// Gets called when an indication has been sent and is answered to
static void indication_callback(struct bt_conn *connection, struct bt_gatt_indicate_params *parameters, uint8_t error) 
{
    printk("Indication callback\n");

    // LOG_DBG() refuses to build for some reason?
    //LOG_DBG("Indication callback, indication %s.\n", error == 0U ? "succesful" : "failed");
}

int accelerometer_indication(void)
{
    if(!indicate_enabled) {
        return -EACCES;
    }

    indication_parameters.attr = bt_gatt_find_by_uuid(NULL, 0, ACCELERATION_DIRECTION_UUID);
	indication_parameters.func = indication_callback;
	indication_parameters.destroy = NULL;
	indication_parameters.data = &direction;
	indication_parameters.len = sizeof(direction);

    return bt_gatt_indicate(NULL, &indication_parameters);
}