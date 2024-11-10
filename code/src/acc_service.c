#include "acc_service.h"

// Whether notification messages should get sent
static bool notify_enabled;
static bool indicate_enabled;
// A collection of parameters concerning indication settings
static struct bt_gatt_indicate_params indication_parameters;

// X, Y and Z axle values, the rotation of the device
struct AccelerationData accelerometer_values;
enum Direction direction;

void simulate_measurement(void)
{
    struct AccelerationData new_values;
    new_values.x_raw = 1600 + (rand() % 100);
    new_values.x_acceleration = rand() % 5;
    new_values.y_raw = 1600 + (rand() % 100);
    new_values.y_acceleration = rand() % 5;
    new_values.z_raw = 1600 + (rand() % 100);
    new_values.z_acceleration = rand() % 5;

    accelerometer_values = new_values;

    printk("x: %d, y: %d, z: %d\n", accelerometer_values.x_raw, accelerometer_values.y_raw, accelerometer_values.z_raw);
}

static void notification_configuration_change(const struct bt_gatt_attr *attributes, uint16_t value)
{
    notify_enabled = (value == BT_GATT_CCC_NOTIFY);
}

static void indication_configuration_change(const struct bt_gatt_attr *attributes, uint16_t value)
{
    indicate_enabled = (value == BT_GATT_CCC_INDICATE);
}

BT_GATT_SERVICE_DEFINE(
    accelerometer, 
    BT_GATT_PRIMARY_SERVICE(ACCELERATION_SERVICE_UUID),
    BT_GATT_CHARACTERISTIC(ACCELERATION_VALUES_UUID, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, &accelerometer_values),
    BT_GATT_CCC(notification_configuration_change, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(ACCELERATION_DIRECTION_UUID, BT_GATT_CHRC_INDICATE, BT_GATT_PERM_READ, NULL, NULL, &direction),
    BT_GATT_CCC(indication_configuration_change, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

int accelerometer_notification(void)
{
    if (!notify_enabled) {
        return -EACCES;
    }

    return bt_gatt_notify(NULL, bt_gatt_find_by_uuid(NULL, 0, ACCELERATION_VALUES_UUID), &accelerometer_values, sizeof(accelerometer_values));
}

static void indication_callback(struct bt_conn *connection, struct bt_gatt_indicate_params *parameters, uint8_t error) 
{
    printk("indication callback\n");
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