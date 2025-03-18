#ifndef __TASK_COLOR_SENSOR_H__
#define __TASK_COLOR_SENSOR_H__

#include "main.h"

/* Struct to store color data */
typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t clear;
} color_data_t;

/* FreeRTOS Queue Handle */
extern QueueHandle_t q_color_data;

/* Function Declarations */
void task_color_sensor_init(void);

#endif /* __TASK_COLOR_SENSOR_H__ */
