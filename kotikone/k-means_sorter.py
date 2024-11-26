import numpy as np
import pandas as pd
import matplotlib.pyplot as plot

def read_data():
    pd_frame = pd.read_csv("sensor_values_example.csv", sep=';', usecols=["x", "y", "z"])
    points = pd_frame.to_numpy()
    return points

def randomize_centers(num, limits):
    centers = np.zeros((num, 3))
    rng = np.random.default_rng()
    for i in range(len(centers)):
        for j in range(len(centers[0])):
            centers[i,j] = rng.integers(low=limits[i][0], high=limits[i][1], size=1)
    return centers

def calculate_k_means(points, centers):
    #result = np.array([points[0:99], points[100:199], points[200:299] ,points[300:399], points[400:499], points[500:599]])
    result = np.array((1,6))

    # Määrät, keskietäisyydet
    # Pisteiden lähiomaistiedot sisältävä lista?
    for i in range(len(points)):
        # Tuloksen laskennan valmistelu
        for j in range(len(centers)):
            x = 1
        # Vau, tulos saatu pisteelle
    # Lopputuloksen siirto listoihin lähiomaistiedon mukaan?
    # Vau, tulos saatu tehtävään
    return result

def plot_results(points, centers):
    fig = plot.figure()
    ax = fig.add_subplot(projection="3d")
    print(points[0,:,0])
    ax.scatter(points[0,:,0], points[0,:,1], points[0,:,2], c='b')
    ax.scatter(points[1,:,0], points[1,:,1], points[1,:,2], c='g')
    ax.scatter(points[2,:,0], points[2,:,1], points[2,:,2], c='m')
    ax.scatter(points[3,:,0], points[3,:,1], points[3,:,2], c='y')
    ax.scatter(points[4,:,0], points[4,:,1], points[4,:,2], c='r')
    ax.scatter(points[5,:,0], points[5,:,1], points[5,:,2], c='c')
    ax.scatter(centers[:,0], centers[:,1], centers[:,2], c='k')
    ax.set_xlabel("X axis")
    ax.set_ylabel("Y axis")
    ax.set_zlabel("Z axis")

    plot.show()

data = read_data()
centers = randomize_centers(6, [[1500, 2200], [1500, 2200], [1500, 2200], [1500, 2200], [1500, 2200], [1500, 2200]])
#print(centers)
clusters = calculate_k_means(data, centers)
plot_results(clusters, centers)