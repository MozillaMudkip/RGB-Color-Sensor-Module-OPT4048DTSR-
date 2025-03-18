#include "main.h"
#include "i2c.h"
#include "source/app_hw/task_console.h"
#include "source/app_hw/task_color_sensor.h"

int main(void) {
    cy_rslt_t rslt;
    rslt = cybsp_init();
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
    __enable_irq();

    i2c_init();
    /* Initialize FreeRTOS tasks */
    task_console_init();
    /* Initialize color sensor task */
    task_color_sensor_init();  
    /* Start scheduler to operate task functionality */ 
    vTaskStartScheduler();

    for (;;) { }
}
