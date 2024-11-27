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
    return np.exp(data - np.max(data)) / np.exp(data - np.max(data)).sum()

def dense(data, weights):
    res = np.zeros((len(data), len(weights[0][0])))
    # Multiply inputs by each neuron's weight
    res = np.matmul(data, weights[0])
    # Apply bias
    res = np.add(res, weights[1])
    return res

#
def model_redone(tensor, w0):
    # Squish the input into a single array
    flat = flatten(tensor)
    # Put through the first neuron layer
    layer = dense(flat, w0)
    print(layer)
    # Scale down to a total of 1
    res = softmax(layer)
    # Voila
    return res


# The pretrained model
model = models.load_model("acc_cnn.keras")

w0 = model.get_layer("dense").get_weights()

# Test (correct answer is 2)
new = tf.convert_to_tensor([[1514,1896,1909]])
res = model(new)
res2 = model_redone(new, w0)

print(model.summary())
print(res)
print(res2)

plt.subplot(1,2,1)
plt.bar([1, 2, 3, 4, 5, 6], res[0])
plt.subplot(1,2,2)
plt.bar([1, 2, 3, 4, 5, 6], res2[0])
plt.show()


def write_to_file(w, file, label1, label2):

    string = "const double " + label1 + "[3][6] = {"

    for i in range(len(w[0])):
        comma = "" if i == 0 else ", "
        string = string + comma + "{"
        st1 = ", ".join(str(num) for num in w[0][i])
        string = string + st1 + "}"
    string = string + "};\n"

    string = string +  "const double " + label2 + "[6] = {"
    st2 = ", ".join(str(num) for num in w[1])
    string = string + st2 + "};\n\n"

    print(string)

    file.write(string)
    return

def save_weights(w0):
    file_h = open("acc_model_weights.h", "w+")
    file_h.truncate(0)
    h_text = "#ifndef WEIGHTS_H_\n" + \
              "#define WEIGHTS_H_\n\n" + \
              "const double weights[3][6];\n" + \
              "const double biases[6];\n\n" + \
              "#endif"
    file_h.write(h_text)
    file_h.close()

    file_c = open("acc_model_weights.c", "w+")
    file_c.truncate(0)
    preamble = '#include "acc_model_weights.h"\n\n'
    file_c.write(preamble)

    write_to_file(w0, file_c, "weights", "biases")

    file_c.close()
    return

save_weights(w0)