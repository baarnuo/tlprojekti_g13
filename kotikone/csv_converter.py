import requests

# Fetches data from the server
rawdata = requests.get("http://172.20.241.34/sensordata_csv.php")
# Replaces echo line breaks with python/windows/whatever compatible ones so that 
# the data gets arranged in actual rows
data = rawdata.text.replace("<br>", "\n")

file = open("sensor_values.csv", "w+")
# Clears any previous contents
file.truncate(0)

file.write(data)