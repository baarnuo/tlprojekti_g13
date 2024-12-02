# Information technology project, autumn 2024: a system processing accelerometer data

[README in Finnish / suomeksi](README-LUEMINUT.md)

---
## Our goals

A program on the nRF 5340 Development Kit could read an accelerometer, use a machine learning algorithm to deduce the direction of the device from the acceleration caused by gravity, and pass the direction to a Raspberry Pi over a Bluetooth Low Energy connection. The data was then passed to a database, from which it could be queried through a HTTP interface. (Image 1.)

![System architecture diagram.](readme_images/Arkkitehtuurikuva_2.png?raw=true)
#### Image 1: System architecture

In addition to the technical goals, we studied and utilised Scrum and Kanban.

The project [is taking place in] year 2 of a Degree Programme in Information Technology (specialized in embedded development) in Oulu University of Applied Sciences.

## Group members

[@Nornalite](https://github.com/Nornalite/)

[@baarnuo](https://github.com/baarnuo/)

## Phases and components

![Raspberry Pi, accelerometer, nRF 5340DK and some collected data on a screen.](readme_images/setup.jpg?raw=true)
#### Image 2: Our hardware

### nRF 5340 Development Kit
Our Development Kit program comprised of multiple parts. Our accelerometer module utilised [Zephyr's adc driver](https://docs.zephyrproject.org/latest/doxygen/html/group__adc__interface.html) to read data from our accelerometer. The readings were attached to an accelerometer service and sent out over Bluetooth Low Energy. Firstly, our service was adevrtised to nearby devices. A device could then form a connection with our Development Kit and subscribe to notifications, which would get the Kit to send out its data after each measurement.

We used an example adc reader program provided by our teacher and Nordic Semiconductor's [Bluetooth Low Energy Fundamentals](https://academy.nordicsemi.com/courses/bluetooth-low-energy-fundamentals/) course as starting points.

### Machine learning
We taught and implemented two machine learning algorithms to calculate directional outputs. (Yes, we know it wasn't particularly efficient, but the school wanted to include machine learning.)

Our primary algorithm was k-means clustering, where randomised centre points move towards nearby clusters of data points over multiple rounds of recalculations (image 3). We first wrote a k-means clustering program, and then fed our data in to get a set of 6 centre points from the algorithm. Afterwards we imported said centre points to the nRF 5340DK, where we added a small program to compare and match accelerometer readings to the nearest centre point.

![Six tight clusters of data points, and individual centre points, some of which are in data clusters and others out on the open.](readme_images/readings_with_centers.png?raw=true)
#### Image 3: K-means clustering algorithm after a round of calculations

As an extra assignment I used [Tensorflow](https://www.tensorflow.org/) to create a simple neural network to sort readings to one of the six general directions. A single-layer neural network with a Categorical Crossentropy loss function and Exponential Decay optimizer in a Sequential model provided perfect accuracy in a span of 10 epochs with a set of 600 samples (image 4.)

With the model taught I exported the weights and biases to a .h file and imported it to teh Development Kit. There I input the taught parameters to a function which recreated the neural net and thus could sort data into directional categories.

![Neural network model with three input nodes, one flatten layer and a dense layer with six output nodes.](readme_images/cnn_model.png?raw=true)
#### Image 4: Neural network model

### Raspberry Pi 3 B
Our Raspberry Pi hosted a program which utilised the [bleak](https://github.com/hbldh/bleak) library to scan for BLE advertisements containing our service's Universally Unique Identifier (UUID). Received data was then unpacked and sent to our database with the help of [mysql-connector](https://pypi.org/project/mysql-connector-python/).

### Linux server and MySQL database
The school granted us personal Ubuntu servers for the duration of the course. We used these servers to practice working with Linux through three sets of extra assignments (from which some outputs can be seen in the linux folder, but most of which were reported in a separate, non-available learning diary.) We also created personal MySQL databases on the server to hold our accelerometer data.

### Other methodologies
We studied utilised both Scrum and Kanban methodologies. The project was divided into small, weekly segments, and we had both daily inter-team meetings and weekly gatherings of multiple teams. We used a kanban board from GitHub's projects section to track progress and further divide the project to smalled tasks.

## Progression and conclusion
Personal issues caused some intereference during the project, but the technical side went well and we completed both the base project and included extra assignments.

The end.