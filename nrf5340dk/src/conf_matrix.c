#include "conf_matrix.h"

static int none_matrix[6][6] = {{0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}};
#ifdef ACC_NN_CALC_H
static int nn_matrix[6][6] = {{0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}};
#endif
#ifdef KMEANS_H
static int kmeans_matrix[6][6] = {{0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}};
#endif

void confusion_matrix_measurement(int algorithm, int true_direction) 
{
    int (*matrix)[6][6];
    char printout[30];
    switch (algorithm) {
        #ifdef ACC_NN_CALC_H
        case NEURAL:
            matrix = &nn_matrix;
            strcpy(printout, "neural algorithm chosen");
            break;
        #endif
        #ifdef KMEANS_H
        case KMEANS:
            matrix = &kmeans_matrix;
            strcpy(printout, "k-means algorithm chosen");
            break;
        #endif
        case NONE:
        default:
            matrix = &none_matrix;
            strcpy(printout, "no algorithm chosen");
            break;
    }

    for (int i = 0; i < 100; i++) {
        struct AccelerometerMeasurement measurement = read_data(algorithm);
        // In case of error
        if (measurement.acceleration.direction == 0) {
            continue;
        }
        int result = measurement.acceleration.direction - 1;
        (*matrix)[true_direction - 1][result] += 1;

        k_sleep(K_MSEC(100));
    }

    printk("Confusion matrix results: %s.\n", printout);
    printk("     P R E D I C T E D\n");
    char text[7] = " REAL ";
    for (int j = 0; j < 6; j++) {
        printk("%c [%d] [%d] [%d] [%d] [%d] [%d]\n", text[j], (*matrix)[j][0], (*matrix)[j][1], (*matrix)[j][2], (*matrix)[j][3], (*matrix)[j][4], (*matrix)[j][5]);
    }

    return;
}