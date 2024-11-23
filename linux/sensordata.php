// Extra Linux assignment

<?php
echo "Running sensor data. ";

include '/var/www/scripts/logindata.php';

echo "... Forming connection. ";

$conn = new mysqli($servername, $username, $password, $database) or die("... Connection failed, error " . $conn->connect_error);

echo "... Fetching data. ";

$sql = "SELECT x_val, y_val, z_val, direction, mac_address FROM rawdata ORDER BY id_measurement DESC LIMIT 20";
$result = $conn->query($sql);

echo "... Data fetched, row count " . $result->num_rows . ".";

if ($result->num_rows > 0) {
	$i = 0;
	echo "... Data recovered:";
	while ($row = $result->fetch_assoc()) {
		$i = $i + 1;
		echo "<br>";
		echo "Item " . $i . ": x = " . $row["x_val"] . ", y = " . $row["y_val"] . ", z = " . $row["z_val"] . ", direction = " . $row["direction"] . ". Device ID " . $row["mac_address"] . ".";
	}
} else {
	echo "... No data found in database.";
}

$conn->close();
?>
