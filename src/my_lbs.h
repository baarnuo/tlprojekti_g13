/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef BT_LBS_H_
#define BT_LBS_H_

/**@file
 * @defgroup bt_lbs LED Button Service API
 * @{
 * @brief API for the LED Button Service (LBS).
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>

/** @brief LBS Service UUID. */
#define BT_UUID_LBS_VAL BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_LBS_MYSENSOR_VAL BT_UUID_128_ENCODE(0x00001526, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_X_VAL BT_UUID_128_ENCODE(0x00001527, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_Y_VAL BT_UUID_128_ENCODE(0x00001528, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_Z_VAL BT_UUID_128_ENCODE(0x00001529, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_LBS_ORIENTATION_VAL BT_UUID_128_ENCODE(0x00001530, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

#define BT_UUID_LBS BT_UUID_DECLARE_128(BT_UUID_LBS_VAL)
#define BT_UUID_LBS_MYSENSOR BT_UUID_DECLARE_128(BT_UUID_LBS_MYSENSOR_VAL)
#define BT_UUID_X BT_UUID_DECLARE_128(BT_UUID_X_VAL)
#define BT_UUID_Y BT_UUID_DECLARE_128(BT_UUID_Y_VAL)
#define BT_UUID_Z BT_UUID_DECLARE_128(BT_UUID_Z_VAL)


/** @brief Send the sensor value as notification.
 *
 * This function sends an uint32_t  value, typically the value
 * of a simulated sensor to all connected peers.
 *
 * @param[in] sensor_value The value of the simulated sensor.
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int my_lbs_send_sensor_notify(uint32_t sensor_value);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* BT_LBS_H_ */
