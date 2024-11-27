#include "acc_cnn_calc.h"

int16_t cnn_direction(int16_t x, int16_t y, int16_t z)
{
    double data[] = {(double)x, (double)y, (double)z};
    double outputs[] = {0, 0, 0, 0, 0, 0};
    
    // Dense layer
    for (int i = 0; i < 6; i++) {
        // Perform multiplication
        for (int j = 0; j < 3; j++) {
            outputs[i] += data[j] * weights[j][i];
        }
        // Add biases
        outputs[i] += biases[i];
    }

    // Perform softmax

    int16_t res = 0;
    double largest = 0;
    // Pick out the highest confidence value
    for (int k = 1; k < 7; k++) {
        if (outputs[k-1] > largest) {
            largest = outputs[k-1];
            res = k;
        }
    }

    //printk("Returning from cnn calc: %d", res);

    return res;
}