import numpy as np
import pandas as pd
import keras
from keras import layers
import tensorflow as tf
import matplotlib.pyplot as plt


pd_data = pd.read_csv("sensor_values_example.csv", sep=';', usecols=["x", "y", "z", "direction"])
# Shuffle the data to mitigate the issues brought by split_dataset splitting the data in half
# (because our data is in order of direction 5, 6, 3, 4, 1, 2 or so)
pd_data = pd_data.sample(frac=1).reset_index(drop=True)

dir_values = pd_data.pop("direction")
dir_values_oh = pd.get_dummies(dir_values, columns=["direction"])
#print(pd_data)
#print(dir_values_oh)

# Separates the data into training and validation data
xyz_train = pd_data[0:500].to_numpy()
xyz_test = pd_data[500:600].to_numpy()
#print(xyz_train)
dir_train = dir_values_oh[0:500].to_numpy()
dir_test = dir_values_oh[500:600].to_numpy()

training_dataset = tf.data.Dataset.from_tensor_slices((xyz_train, dir_train))
testing_dataset = tf.data.Dataset.from_tensor_slices((xyz_test, dir_test))
# Turns the dataset into batches to be passed into the model
batched_training = training_dataset.batch(40)
batched_testing = testing_dataset.batch(40)


input_shape = (3,)
# Wow, the model
accel_model = tf.keras.Sequential([
    layers.Input(shape=input_shape),
    layers.Flatten(),
    #layers.Dense(64, activation="relu"),
    layers.Dense(6, activation="softmax")
])

accel_model.compile(
    loss = tf.keras.losses.CategoricalCrossentropy(),
    optimizer = tf.keras.optimizers.Adam(learning_rate=
                  keras.optimizers.schedules.ExponentialDecay(
                    0.25,
                    decay_steps=120,
                    decay_rate=0.1,
                    staircase=False)
                  ),
    metrics=['accuracy'],
    )

accel_model.summary()

# Test
res = accel_model.evaluate(batched_testing, return_dict=True)

# Training
training = accel_model.fit(batched_training, epochs=10)

new = tf.convert_to_tensor([[1514,1896,1909]])
res = accel_model(new)

print(res)

plt.bar([1, 2, 3, 4, 5, 6], res[0])
plt.show()