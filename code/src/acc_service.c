#include "acc_service.h"

// Whether notification messages should get sent
static bool notify_enabled;
// A collection of parameters concerning indication settings
static struct bt_gatt_indicate_params indication_parameters;

// X, Y and Z axle values, the rotation of the device
struct AccelerationData accelerometer_values;
enum Direction direction;

static void configuration_change(const struct bt_gatt_attr *attributes, uint16_t value)
{
    notify_enabled = (value == BT_GATT_CCC_NOTIFY);
}

BT_GATT_SERVICE_DEFINE(
    accelerometer, 
    BT_GATT_PRIMARY_SERVICE(ACCELERATION_SERVICE_UUID),
    BT_GATT_CCC(configuration_change, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(ACCELERATION_VALUES_UUID, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, &accelerometer_values),
    BT_GATT_CHARACTERISTIC(ACCELERATION_DIRECTION_UUID, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, &direction),
);

int accelerometer_notification()
{
    if (!notify_enabled) {
        return -EACCES;
    }

    return 0;//bt_gatt_notify(NULL, &accelerometer.attrs[]);
}