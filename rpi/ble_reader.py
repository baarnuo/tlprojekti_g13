import asyncio
from struct import unpack

from bleak import BleakClient
from bleak.backends.characteristic import BleakGATTCharacteristic

address = ("cd:72:02:f4:fb:67")
char_uuid = "f50234f1-4833-42b8-8d36-983f06b3ee8e"
# 0000xxxx-0000-1000-8000-00805f9b34fb is the default structure for BLE UUIDs
# and how a 16-bit uuid (such as our 34f0) is represented if the rest isn't sent
# Doesn't seem to matter here, but for example our accelerometer service is shown in
# that format (000034f0-0000-1000-8000-00805f9b34fb) while scanning
char_uuid2 = "000034f1-0000-1000-8000-00805f9b34fb"


async def notification_callback(characteristic: BleakGATTCharacteristic, data: bytearray):
	print("".join([f"\\x{byte:02x}" for byte in data]))
	# Unpacks the data into its four constituent variables
	x, y, z, dir = unpack('<hhhh', data)
	print(x, y, z, dir)


async def main():
	async with BleakClient(address) as client:
		print("Connected")
		await client.start_notify(char_uuid, notification_callback)
		# Runs for 60 seconds before stopping
		await asyncio.sleep(60.0)
		await client.stop_notify(char_uuid)

if __name__ == "__main__":
	asyncio.run(main())
