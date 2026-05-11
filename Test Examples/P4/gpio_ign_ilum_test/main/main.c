#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "sdkconfig.h"


void app_main(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << 26),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    io_conf.pin_bit_mask = (1ULL << 27);
    gpio_config(&io_conf);

    while (1) {
        int level = gpio_get_level(26);
        int level2 = gpio_get_level(27);

        printf("Illumination: %d\n", level);
        printf("Ignition: %d\n\n", level2);

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}