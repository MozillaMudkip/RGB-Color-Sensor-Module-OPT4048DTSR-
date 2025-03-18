#include "main.h"
#include "i2c.h"
#include "source/app_hw/task_console.h"
//#include "source/app_hw/task_blink.h"
//#include "source/app_hw/task_dc_control.h"
#include "source/app_hw/task_color_sensor.h"

int main(void) {
    cy_rslt_t rslt;
    rslt = cybsp_init();
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
    __enable_irq();

    i2c_init();
    /* Initialize FreeRTOS tasks */
    task_console_init();
    //task_blink_init();
    //task_dc_control_init();
    task_color_sensor_init();  

    vTaskStartScheduler();

    for (;;) { }
}
