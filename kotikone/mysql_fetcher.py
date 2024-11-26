# Extra Linux assignment

import mysql.connector
from mysql_credentials import add, u, pw, db
import csv

mydb = mysql.connector.connect(
    host = add,
    user = u,
    password = pw,
    database = db
)
cursor = mydb.cursor()
query = "SELECT * FROM rawdata"
cursor.execute(query)
    
file = open("sensor_values2.csv", "w+")
# Empties the file
file.truncate(0)
# unix gets a single newline at the end of each row, delimiter ; puts semicolons between fields
writer = csv.writer(file, dialect="unix", delimiter=';')
writer.writerow(["id_measurement", "timestamp", "x", "y", "z", "direction", "x_deg", "y_deg", "z_deg", "gravity", "mac_address"])

# Loop through the rows and write data
for (id, time, x, y, z, dir, x_deg, y_deg, z_deg, grav, mac) in cursor:
    row = [id, time, x, y, z, dir, x_deg, y_deg, z_deg, grav, mac]
    writer.writerow(row)