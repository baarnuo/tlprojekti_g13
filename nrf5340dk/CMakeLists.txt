cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(accel_sensor)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/acc_service.c)
target_sources(app PRIVATE src/acc_sensor.c)
target_sources(app PRIVATE src/acc_nn_calc.c)
target_sources(app PRIVATE src/kmeans.c)
target_sources(app PRIVATE src/conf_matrix.c)