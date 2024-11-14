/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief LED Button Service (LBS) sample
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "my_lbs.h"
#include "adc.h"

//LOG_MODULE_DECLARE(Lesson4_Exercise2);

static bool notify_mysensor_enabled;
struct Measurement measurements;
uint8_t sensor_orientation = 0;

static void mylbsbc_ccc_mysensor_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	notify_mysensor_enabled = (value == BT_GATT_CCC_NOTIFY);
}

void get_measurements(void)
{
	measurements = readADCValue();
	printk("x = %d, y = %d, z = %d\n", measurements.x, measurements.y, measurements.z);
	printk("Sensorin asento %d\n", sensor_orientation);

	send_sensor_x_notify(measurements.x);
	send_sensor_y_notify(measurements.y);
	send_sensor_z_notify(measurements.z);
	my_lbs_send_sensor_notify(sensor_orientation);
}

void set_orientation(void)
{
	printk("Nappia painettu -> asento vaihtuu\n");
	printk("Asennot:\n");
	printk("0 = X-akseli alas - suuri arvo\n");
	printk("1 = X-akseli ylös - pieni arvo\n");
	printk("2 = Y-akseli alas - suuri arvo\n");
	printk("3 = Y-akseli ylös - pieni arvo\n");
	printk("4 = Z-akseli alas - suuri arvo\n");
	printk("5 = Z-akseli ylös - pieni arvo\n");

	sensor_orientation++;

	if (sensor_orientation > 5) {
		sensor_orientation = 0;
	}
}

BT_GATT_SERVICE_DEFINE(
	my_lbs_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_LBS),

	BT_GATT_CHARACTERISTIC(BT_UUID_X, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, &measurements.x),
	BT_GATT_CCC(mylbsbc_ccc_mysensor_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_Y, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, &measurements.y),
	BT_GATT_CCC(mylbsbc_ccc_mysensor_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_Z, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, &measurements.z),
	BT_GATT_CCC(mylbsbc_ccc_mysensor_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_LBS_MYSENSOR, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, &sensor_orientation),
	BT_GATT_CCC(mylbsbc_ccc_mysensor_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

int my_lbs_send_sensor_notify(uint32_t sensor_value)
{
	if (!notify_mysensor_enabled) {
		return -EACCES;
	}

	return bt_gatt_notify(NULL, bt_gatt_find_by_uuid(NULL, 0, BT_UUID_LBS_MYSENSOR), &sensor_value, sizeof(sensor_value));
}

int send_sensor_x_notify(uint32_t sensor_value)
{
	if (!notify_mysensor_enabled) {
		return -EACCES;
	}

	return bt_gatt_notify(NULL, bt_gatt_find_by_uuid(NULL, 0, BT_UUID_X), &sensor_value, sizeof(sensor_value));
}

int send_sensor_y_notify(uint32_t sensor_value)
{
	if (!notify_mysensor_enabled) {
		return -EACCES;
	}

	return bt_gatt_notify(NULL, bt_gatt_find_by_uuid(NULL, 0, BT_UUID_Y), &sensor_value, sizeof(sensor_value));
}

int send_sensor_z_notify(uint32_t sensor_value)
{
	if (!notify_mysensor_enabled) {
		return -EACCES;
	}

	return bt_gatt_notify(NULL, bt_gatt_find_by_uuid(NULL, 0, BT_UUID_Z), &sensor_value, sizeof(sensor_value));
}