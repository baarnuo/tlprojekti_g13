import requests
# Server IP, probably doesn't need to be on the open web?
from mysql_credentials import csv_add

# Fetches data from the server
rawdata = requests.get(csv_add)
# Replaces echo line breaks with python/windows/whatever compatible ones so that 
# the data gets arranged in actual rows
data = rawdata.text.replace("<br>", "\n")

file = open("sensor_values.csv", "w+")
# Clears any previous contents
file.truncate(0)

file.write(data)