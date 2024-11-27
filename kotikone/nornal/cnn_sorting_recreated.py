import matplotlib.pyplot as plt
import numpy as np
import keras
from keras import layers, models
import tensorflow as tf


def flatten(data):
    res = data.numpy()
    return res

def relu(data):
    # relu (compares the two and chooses the bigger of each element)
    return np.maximum(0, data)

def softmax(data):
    return np.exp(data) / np.exp(data).sum()

def dense(data, weights):
    res = np.zeros((len(data), len(weights[0][0])))
    # Multiply inputs by each neuron's weight
    res = np.matmul(data, weights[0])
    # Apply bias
    res = np.add(res, weights[1])
    return res

# 
def model_redone(tensor, w0, w1):
    # Squish the input into a single array
    flat = flatten(tensor)
    # Put through the first neuron layer
    layer1 = dense(flat, w0)
    # Limit negatives to 0
    rel = relu(layer1)
    # Put though the second neuron layer
    layer2 = dense(rel, w1)
    # Scale down to a total of 1
    res = softmax(layer2)
    # Voila
    return res

def write_to_file(w, file, label1, label2):
    
    string = "#define " + label1 + " = ["

    for i in range(len(w[0])):
        string = string + "["
        st1 = ", ".join(str(num) for num in w[0][i])
        string = string + st1 + "],\n"
    string = string + "]\n"

    string = string + "#define " + label2 + " = ["
    st2 = ", ".join(str(num) for num in w[1])
    string = string + st2 + "]\n\n"

    print(string)

    file.write(string)
    return

def save_weights(w0, w1):
    file = open("acc_model_weights.h", "w+")
    file.truncate(0)

    preamble = "#ifndef WEIGHTS_H_\n#define WEIGHTS_H_\n\n"
    file.write(preamble)
      
    write_to_file(w0, file, "WEIGHTS1", "BIASES1")
    write_to_file(w1, file, "WEIGHTS2", "BIASES2")

    endif = "#endif"
    file.write(endif)
    
    file.close()
    return


# The pretrained model
model = models.load_model("acc_cnn.keras")

w0 = model.get_layer("dense_49").get_weights()
w1 = model.get_layer("dense_50").get_weights()

# Test (correct answer is 2)
new = tf.convert_to_tensor([[1514,1896,1909]])
res = model(new)
res2 = model_redone(new, w0, w1)

# Writes weights to file
save_weights(w0, w1)

print(model.summary())
print(res)
print(res2)

plt.subplot(1,2,1)
plt.bar([1, 2, 3, 4, 5, 6], res[0])
plt.subplot(1,2,2)
plt.bar([1, 2, 3, 4, 5, 6], res2[0])
plt.show()