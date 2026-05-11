#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


uint8_t gpio_pins[] = { 19, 18, 14, 15, 16, 17 };

void app_main(void)
{
    while (1) {
        for (uint8_t i = 0; i < sizeof(gpio_pins); ++i) {
            printf("GPIO%d toggle\n", gpio_pins[i]);

            // Configure the GPIO pin
            gpio_reset_pin(gpio_pins[i]);
            gpio_set_direction(gpio_pins[i], GPIO_MODE_OUTPUT);

            printf("ON\n");
            gpio_set_level(gpio_pins[i], 1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            printf("OFF\n");
            gpio_set_level(gpio_pins[i], 0);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}