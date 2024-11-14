/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/*
    Modified from the Nordic MyLBS sample
*/

#ifndef ACCS_H_
#define ACCS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>

/** @brief Acceleration service UUID. */
#define BT_UUID_ACCS_VAL BT_UUID_128_ENCODE(0xf50234f0, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)
#define BT_UUID_X_VAL BT_UUID_128_ENCODE(0xf50234f4, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)
#define BT_UUID_Y_VAL BT_UUID_128_ENCODE(0xf50234f5, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)
#define BT_UUID_Z_VAL BT_UUID_128_ENCODE(0xf50234f6, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)
#define BT_UUID_ORIENTATION_VAL BT_UUID_128_ENCODE(0xf50234f7, 0x4833, 0x42b8, 0x8d36, 0x983f06b3ee8e)

#define BT_UUID_ACCS BT_UUID_DECLARE_128(BT_UUID_ACCS_VAL)
#define BT_UUID_X BT_UUID_DECLARE_128(BT_UUID_X_VAL)
#define BT_UUID_Y BT_UUID_DECLARE_128(BT_UUID_Y_VAL)
#define BT_UUID_Z BT_UUID_DECLARE_128(BT_UUID_Z_VAL)
#define BT_UUID_ORIENTATION BT_UUID_DECLARE_128(BT_UUID_ORIENTATION_VAL)

void get_measurements(void);
void set_orientation(void);

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
int accs_send_sensor_notify(uint32_t sensor_value);
int send_sensor_x_notify(uint32_t sensor_value);
int send_sensor_y_notify(uint32_t sensor_value);
int send_sensor_z_notify(uint32_t sensor_value);

#ifdef __cplusplus
}
#endif

#endif /* ACCS_H_ */
