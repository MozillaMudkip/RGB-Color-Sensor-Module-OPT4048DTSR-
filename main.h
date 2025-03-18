/**
 * @file main.h
 * @author Ben D
 * @brief 
 * @version 0.1
 * @date 2025-03-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

 #ifndef __MAIN_H__
 #define __MAIN_H__
 
 /* Include Infineon BSP Libraries */
 #include "cy_pdl.h"
 #include "cyhal.h"
 #include "cybsp.h"
 
 /* Include Standard C Libraries */
 #include <ctype.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <stdbool.h>
 #include <stdarg.h>
 #include <stdint.h>
 
 /* FreeRTOS Includes */
 #include <FreeRTOS.h>
 #include <event_groups.h>
 #include <queue.h>
 #include <semphr.h>
 #include <task.h>
 #include "source/FreeRTOSConfig.h"
 #include "source/FreeRTOS_CLI.h"
 
 /* Include Project Specific Files */
 #include "source/app_hw/i2c.h"
 #include "source/app_hw/spi.h"
 #include "source/app_hw/task_console.h"
 #include "source/app_hw/task_io_expander.h"
 //include color sensor task
 #include "source/app_hw/task_color_sensor.h"
 
 #endif
 
