# Script to read the accelerometer data sent over a BLE connection,
# and to pass it down to our database

import asyncio

import mysql.connector
from mysql_credentials import add, u, pw, db

from struct import unpack

from bleak import BleakScanner, BleakClient
from bleak.backends.characteristic import BleakGATTCharacteristic

# The UUID for the accelerometer data notification characteristic
char_uuid = "f50234f1-4833-42b8-8d36-983f06b3ee8e"
# 0000xxxx-0000-1000-8000-00805f9b34fb is the default structure for BLE UUIDs
# and how a 16-bit uuid (such as our 34f0) is represented if the rest isn't sent
# The UUID for the accelerometer service as a whole
char_uuid2 = ['000034f0-0000-1000-8000-00805f9b34fb']

# MAC address of the device we're connected to
device_id = ""

async def notification_callback(characteristic: BleakGATTCharacteristic, data: bytearray):
	#print("".join([f"\\x{byte:02x}" for byte in data]))
	# Unpacks the data into its four constituent variables
	x, y, z, dir = unpack('<hhhh', data)
	print(x, y, z, dir)
	mydb = mysql.connector.connect(
		host = add,
		user = u,
		password = pw,
		database = db
		)
	cursor = mydb.cursor()
	query = "INSERT INTO rawdata (x_val, y_val, z_val, direction, mac_address) VALUES (%s, %s, %s, %s, %s)"
	variables = (x, y, z, dir, device_id)
	cursor.execute(query, variables)
	mydb.commit()


async def main():
	print("Starting scan...")
	async with BleakScanner() as scanner:
		# Keep looking through different advertisements
		async for device, ad in scanner.advertisement_data():
			# When the advertised service's UUID matches ours, take note of the
			# device address and quit looking
			if (ad.service_uuids == char_uuid2):
				print("Found correct service!")
				global device_id
				device_id = device.address
				print(device_id)
				break
	# Proceed to connect to the device
	async with BleakClient(device_id) as client:
		print("Asking for notifications")
		await client.start_notify(char_uuid, notification_callback)
		# Runs for 240 seconds before stopping
		await asyncio.sleep(240.0)
		await client.stop_notify(char_uuid)


if __name__ == "__main__":
	asyncio.run(main())
