// Extra Linux assignment

<?php
include '/var/www/scripts/logindata.php';

$conn = new mysqli($servername, $username, $password, $database) or die("... Connection failed, error " . $conn->connect_error);

$sql = "SELECT * FROM rawdata ORDER BY id_measurement DESC LIMIT 20";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
	$i = 0;
	while ($row = $result->fetch_assoc()) {
		$i = $i + 1;
		echo "<br>";
		echo $row["id_measurement"] . ";" . $row["timestamp"] . ";" . $row["x_val"] . ";" . $row["y_val"] . ";" . $row["z_val"] . ";" . $row["direction"] . ";" . $row["x_deg"] . ";" . $row["y_deg"] . ";" . $row["z_deg"] . ";" . $row["gravity"] . ";" . $row["mac_address"];
	}
} else {
	echo "... No data found in database.";
}

$conn->close();
?>
