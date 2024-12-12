#include "kmeans.h"

int16_t kmeans_direction(int16_t x, int16_t y, int16_t z)
{
    int16_t direction = 0;
    //double data[] = {(double)x, (double)y, (double)z};
    double distances[3];
    double centers[6][3] = CENTERS;
    double smallest_distance = 1000000000;

    for (int i = 0; i < 6; i++) {
        distances[0] = centers[i][0] - x;
        distances[1] = centers[i][1] - y;
        distances[2] = centers[i][2] - z;
        double distance = sqrt(pow(distances[0], 2) + pow(distances[1], 2) + pow(distances[2], 2));
        if (distance < smallest_distance) {
            direction = i + 1;
            smallest_distance = distance;
        }
    }
    
    return direction;
}