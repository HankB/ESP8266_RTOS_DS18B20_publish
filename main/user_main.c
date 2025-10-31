/* copided from Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <time.h>

#include "my_info.h"
#include "my_gpio.h"
#include "my_wifi.h"
#include "my_mqtt.h"
#include "my_sntp.h"
#include "my_ds18b20.h"
#include "my_main.h"

static const char *TAG = "user_main";
time_t uptime;
time_t now;

#define publish_buf_len 100
char publish_buf[];

void app_main()
{
    time_t  boot_timestamp = 0;
    int  loop_count = 0;
    static int loop_delay_sec = 10;

    printf("init_wifi()\n");
    init_wifi();
    printf("mqtt_start()\n");
    mqtt_start();
    printf("init_sntp()\n");
    init_sntp();
    printf("print_my_info()\n");
    print_my_info();
    printf("init_gpio()\n");
    init_gpio();
    printf("init_DS18B20()\n");
    init_DS18B20();
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
    printf("Hello there!\n");
    
    while( true) {
        now = time(0);
        if(boot_timestamp == 0)
        {
            if( now > 1650502924)
            {
                boot_timestamp = now + loop_count*loop_delay_sec;
                uptime = 0;
            }
            else
            {
                loop_count++;
                uptime = now;
            }
        }
        else
        {
            uptime = now - boot_timestamp;
        }
        snprintf(publish_buf, publish_buf_len, "hello world heap:%d, t:%ld, uptime:%ld",
                        esp_get_free_heap_size(), now, uptime);
        mqtt_publish(NULL, publish_buf);
        vTaskDelay(1000*loop_delay_sec / portTICK_PERIOD_MS); // publish every loop_delay_sec s.
    }

    esp_restart();
}
