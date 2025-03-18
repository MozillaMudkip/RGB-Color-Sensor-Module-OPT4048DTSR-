#include "task_color_sensor.h"

/* Define I2C SDA and SCL pins */
#define COLOR_SENSOR_SDA    P9_0  
#define COLOR_SENSOR_SCL    P9_1 

#define OPT4048_I2C_ADDR  0x44  // I2C address with ADDR tied to GND (considered the default for this sensor)
#define OPT4048_REG_DATA  0x00  // Register to read color data
#define OPT4048_REG_CONFIG 0x0A // Sensor configuration register
/* FreeRTOS queue to send color data */
QueueHandle_t q_color_data;

static bool color_sensor_write_config(void) {
    uint8_t config_data[3] = {0x0A, 0x32, 0x38};  //Power on sensor and put into continuous operation
    uint8_t read_data[2];
    cy_rslt_t result;

    xSemaphoreTake(Semaphore_I2C, portMAX_DELAY); // Take I2C semaphore
    // Write to the configuration register
    result = cyhal_i2c_master_write(&i2c_master_obj, OPT4048_I2C_ADDR, config_data, 3, 0, true);
    //Delay to give sensor time to get ready for a read
    vTaskDelay(pdMS_TO_TICKS(500));
    //Read data from configuration register (for checking on oscilloscope)
    result = cyhal_i2c_master_read(&i2c_master_obj, OPT4048_I2C_ADDR, read_data, 2, 0, true);
    xSemaphoreGive(Semaphore_I2C); // Release I2C semaphore
    //Print out failure if we're given a failing read or write response
    if (result != CY_RSLT_SUCCESS) {
        task_print("Failed to write configuration register: 0x%04X\n", result); 
        return false;
    }
    return true;
}
/* Function to read from OPT4048 */
static bool color_sensor_read(uint8_t reg, uint8_t *data, uint8_t len) {     
    cy_rslt_t result;
    xSemaphoreTake(Semaphore_I2C, portMAX_DELAY); //Gain control of I2C bus

    //Write register address of the sensor
    result = cyhal_i2c_master_write(&i2c_master_obj, OPT4048_I2C_ADDR, &reg, 1, 0, true);   
    if (result != CY_RSLT_SUCCESS){
        xSemaphoreGive(Semaphore_I2C); //Return I2C bus on failure
        return false;
    }
    vTaskDelay(pdMS_TO_TICKS(500));
    //Read the data from the sensor
    result = cyhal_i2c_master_read(&i2c_master_obj, OPT4048_I2C_ADDR, data, len, 0, true);
    xSemaphoreGive(Semaphore_I2C); //Return I2C bus
    return (result == CY_RSLT_SUCCESS);
}

/* Color Sensor Task */
static void task_color_sensor(void *param) {
    /* Initialize the sensor by writing to the configuration register */
    if (!color_sensor_write_config()) {
        task_print("Failed to initialize OPT4048 sensor. Check I2C communication.\n");
        return; // Exit if initialization fails
    }
    
    while (1) {
        /* Wait before next read, avoids reading before sensor is ready */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* CLI Handler for 'colorsensor' command */
static BaseType_t cli_handler_colorsensor(
    char *pcWriteBuffer,
    size_t xWriteBufferLen,
    const char *pcCommandString)
{
    const char *pcParameter;
    BaseType_t xParameterStringLength, xReturn;
    color_data_t color_values;
    uint8_t raw_data[8];

    /* Get parameter string */
    pcParameter = FreeRTOS_CLIGetParameter(
        pcCommandString, 1, &xParameterStringLength);
    configASSERT(pcParameter);

    /* If we recieve the print command from the serial monitor... */
    if (strncmp(pcParameter, "print", xParameterStringLength) == 0) {
        //Read color sensor data 
        if (color_sensor_read(OPT4048_REG_DATA, raw_data, 8)) {    
            uint16_t x = (raw_data[0] << 8) | raw_data[1];
            uint16_t y = (raw_data[2] << 8) | raw_data[3];
            uint16_t z = (raw_data[4] << 8) | raw_data[5];
            uint16_t w = (raw_data[6] << 8) | raw_data[7];

            //Update color values
            color_values.red = x;
            color_values.green = y;
            color_values.blue = z;
            color_values.clear = w;

            /* Send data to queue */
            xQueueSend(q_color_data, &color_values, portMAX_DELAY);

            vTaskDelay(pdMS_TO_TICKS(500));
            /* Print out color data to serial monitor */
            task_print("X: %u, Y: %u, Z: %u, W: %u\n",
                       color_values.red, color_values.green,
                       color_values.blue, color_values.clear);
        }
    } 
    //In the event of any other input, give correct usage
    else {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid command. Use 'colorsensor print'.\n");
        xReturn = pdFALSE;
        return xReturn;
    }
    xReturn = pdFALSE;
    return xReturn;
}

/* The CLI command definition for the color sensor 
 User types in "colorsensor print" into serial monitor 
 to recieve printout of most recent color reading */
static const CLI_Command_Definition_t xColorSensor =
    {
        "colorsensor",
        "\r\ncolorsensor <print>\r\n Controls color sensor output.\r\n",
        cli_handler_colorsensor,
        1
    };

/* Initialization function for color sensor */
void task_color_sensor_init(void) {
    cy_rslt_t rslt;

    /* Create queue for color data */
    q_color_data = xQueueCreate(5, sizeof(color_data_t));
    configASSERT(q_color_data != NULL);

    /* Register CLI command */
    FreeRTOS_CLIRegisterCommand(&xColorSensor);
    
    /* Create FreeRTOS task */
    xTaskCreate(task_color_sensor, "Task_Color_Sensor", configMINIMAL_STACK_SIZE + 100, NULL, 2, NULL);
}
