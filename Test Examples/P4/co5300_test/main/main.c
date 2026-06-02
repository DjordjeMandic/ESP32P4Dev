#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_check.h"

#include "esp_ldo_regulator.h"

#include "esp_lcd_mipi_dsi.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

#include "esp_lcd_co5300.h"

static const char *TAG = "co5300_rgb888";

#define LCD_H_RES 466
#define LCD_V_RES 466

#define LCD_BIT_PER_PIXEL (24)

#define PIN_RST 13

#define LDO_CHAN 3
#define LDO_MV   2500

// RGB888 framebuffer (3 bytes per pixel)
static uint8_t *framebuffer;

static void fill_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < LCD_H_RES * LCD_V_RES; i++) {
        framebuffer[i * 3 + 0] = r;
        framebuffer[i * 3 + 1] = g;
        framebuffer[i * 3 + 2] = b;
    }
}

void app_main(void)
{
    esp_ldo_channel_handle_t ldo = NULL;
    esp_lcd_dsi_bus_handle_t dsi_bus = NULL;
    esp_lcd_panel_io_handle_t io = NULL;
    esp_lcd_panel_handle_t panel = NULL;

    ESP_LOGI(TAG, "Alloc framebuffer in PSRAM");

    framebuffer = heap_caps_malloc(
        LCD_H_RES * LCD_V_RES * 3,
        MALLOC_CAP_SPIRAM
    );

    if (!framebuffer) {
        ESP_LOGE(TAG, "No PSRAM for framebuffer");
        return;
    }

    ESP_LOGI(TAG, "Enable LDO");
    esp_ldo_channel_config_t ldo_cfg = {
        .chan_id = LDO_CHAN,
        .voltage_mv = LDO_MV,
    };
    ESP_ERROR_CHECK(esp_ldo_acquire_channel(&ldo_cfg, &ldo));

    ESP_LOGI(TAG, "Init DSI bus");
    esp_lcd_dsi_bus_config_t bus_cfg = CO5300_PANEL_BUS_DSI_1CH_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_dsi_bus(&bus_cfg, &dsi_bus));

    ESP_LOGI(TAG, "Init IO");
    esp_lcd_dbi_io_config_t io_cfg = CO5300_PANEL_IO_DBI_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_dbi(dsi_bus, &io_cfg, &io));

    ESP_LOGI(TAG, "Init panel config");

    esp_lcd_dpi_panel_config_t dpi_cfg =
        CO5300_466_466_PANEL_60HZ_DPI_CONFIG_CF(LCD_COLOR_FMT_RGB888);

    co5300_vendor_config_t vendor_cfg = {
        .flags.use_mipi_interface = 1,
        .mipi_config = {
            .dsi_bus = dsi_bus,
            .dpi_config = &dpi_cfg,
        },
    };

    esp_lcd_panel_dev_config_t panel_cfg = {
        .reset_gpio_num = PIN_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = LCD_BIT_PER_PIXEL,
        .vendor_config = &vendor_cfg,
    };

    ESP_ERROR_CHECK(
        esp_lcd_new_panel_co5300(io, &panel_cfg, &panel)
    );

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel));

    ESP_LOGI(TAG, "Start color test");

    while (1) {

        ESP_LOGI(TAG, "RED");
        fill_rgb(255, 0, 0);
        ESP_ERROR_CHECK(
            esp_lcd_panel_draw_bitmap(
                panel,
                0, 0,
                LCD_H_RES, LCD_V_RES,
                framebuffer
            )
        );
        vTaskDelay(pdMS_TO_TICKS(1500));

        ESP_LOGI(TAG, "GREEN");
        fill_rgb(0, 255, 0);
        ESP_ERROR_CHECK(
            esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_H_RES, LCD_V_RES, framebuffer)
        );
        vTaskDelay(pdMS_TO_TICKS(1500));

        ESP_LOGI(TAG, "BLUE");
        fill_rgb(0, 0, 255);
        ESP_ERROR_CHECK(
            esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_H_RES, LCD_V_RES, framebuffer)
        );
        vTaskDelay(pdMS_TO_TICKS(1500));

        ESP_LOGI(TAG, "WHITE");
        fill_rgb(255, 255, 255);
        ESP_ERROR_CHECK(
            esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_H_RES, LCD_V_RES, framebuffer)
        );
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}