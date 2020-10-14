#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#include "HUGS.h"
#include "hugsIO.h"

void app_main(void)
{
	vTaskDelay(100 / portTICK_PERIOD_MS);

	start_HUGS_tasks();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    start_HUGSIO_tasks();
}

