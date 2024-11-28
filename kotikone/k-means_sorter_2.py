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
    #Alustetaan tarvittavat listat
    distances = np.zeros(len(centers))
    centerPointCumulativeSum = np.zeros(shape=(6,3))
    counts = np.zeros(len(centers))
    print(centers)

    #Käydään läpi kaikki datapisteet
    for i in range(len(points)):
        #Käydään läpi kaikki keskipisteet
        for j in range(len(centers)):
            #Lasketaan jokaisen datapisteen etäisyys kuhunkin keskipisteeseen
            distances[j] = np.linalg.norm(centers[j] - points[i])

        #Poimitaan pienimmän etäisyyden indeksi talteen
        index = np.argmin(distances)
        #Kasvatetaan counts-listan arvoa pienimmän etäisyyden indeksin kohdalta yhdellä
        counts[index] += 1
        #Lisätään voittajakeskipisteelle datapisteen arvot
        centerPointCumulativeSum[index] += points[i]

    print(counts)
    print(centerPointCumulativeSum)

    #Alustetaan taulukko uusille keskipisteille
    new_centers = np.zeros_like(centers)
    rng = np.random.default_rng()
    for i in range(len(centers)):
        #Jos keskipiste voitti datapisteitä, lasketaan uusi keskipiste
        if counts[i] > 0:
            new_centers[i] = centerPointCumulativeSum[i] / counts[i]
        #Jos keskipiste jäi tyhjäksi, arvotaan uusi keskipiste
        elif counts[i] == 0:
            new_centers[i] = rng.integers(1500, 2200, size=3)

    print("uudet", new_centers)

    return new_centers

def plot_results(points, centers):
    fig = plot.figure()
    ax = fig.add_subplot(projection="3d")
    print("Points:", len(points), len(points[0]), points[:,:])
    ax.scatter(points[0:99,0], points[0:99,1], points[0:99,2], c='b')
    ax.scatter(points[100:199,0], points[100:199,1], points[100:199,2], c='g')
    ax.scatter(points[200:299,0], points[200:299,1], points[200:299,2], c='m')
    ax.scatter(points[300:399,0], points[300:399,1], points[300:399,2], c='y')
    ax.scatter(points[400:499,0], points[400:499,1], points[400:499,2], c='r')
    ax.scatter(points[500:599,0], points[500:599,1], points[500:599,2], c='c')
    ax.scatter(centers[:,0], centers[:,1], centers[:,2], c='k')
    ax.set_xlabel("X axis")
    ax.set_ylabel("Y axis")
    ax.set_zlabel("Z axis")

    plot.show()

data = read_data()
centers = randomize_centers(6, [[1500, 2200], [1500, 2200], [1500, 2200], [1500, 2200], [1500, 2200], [1500, 2200]])
#print(centers)
clusters = calculate_k_means(data, centers)
plot_results(data, clusters)