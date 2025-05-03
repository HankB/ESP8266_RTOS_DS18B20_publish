/*
* Start by copying the example code at 
* https://github.com/UncleRus/esp-idf-lib/blob/master/examples/ds18x20/multi/main/main.c
*/

#include <inttypes.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds18x20.h>
#include <esp_log.h>
#include <esp_err.h>
#include "esp_system.h"
#include <string.h>

#include "my_ds18b20.h"
#include "my_main.h"
#include "my_mqtt.h"

#define CONFIG_EXAMPLE_DS18X20_MAX_SENSORS 8

static const gpio_num_t SENSOR_GPIO = GPIO_NUM_4; //CONFIG_EXAMPLE_ONEWIRE_GPIO;
static const int MAX_SENSORS = CONFIG_EXAMPLE_DS18X20_MAX_SENSORS;
static const int RESCAN_INTERVAL = 8;
static const uint32_t LOOP_DELAY_MS = (1000*60);

static const char *TAG = "ds18x20_test";


#define topic_len 100
static char topic_buf[topic_len];

/*
Can't see how to get floating point in printf() family so doin' it the hard way.
Output will be formatted as "nnn.nn", "nn.nn", "-nnn.nn", "-nn.nn". It is 
required of the caller to insure adequate buffer space.
*/
const char * format_temp(float t, char * buf)
{
    // negative values are nasty - save the sign and make positive.
    char sign[2]="";
    if(t < 0.0) {
        sign[0]='-';
        sign[1] = 0;
        t -= 2*t;
    }

    int     whole_part = (int)t;
    int     frac_part = (int)((t-whole_part+.005)*100);

    sprintf(buf, "%s%d.", sign, whole_part);
    sprintf(buf+strlen(buf), "%2.2d", frac_part);

    return buf;
}

static const char *sensor_type(uint8_t family_id)
{
    switch (family_id)
    {
        case DS18X20_FAMILY_DS18S20:
            return "DS18S20";
        case DS18X20_FAMILY_DS1822:
            return "DS1822";
        case DS18X20_FAMILY_DS18B20:
            return "DS18B20";
        case DS18X20_FAMILY_MAX31850:
            return "MAX31850";
    }
    return "Unknown";
}

void ds18x20_publisher(void *pvParameter)
{
    onewire_addr_t addrs[MAX_SENSORS];
    float temps[MAX_SENSORS];
    size_t sensor_count = 0;

    // There is no special initialization required before using the ds18x20
    // routines.  However, we make sure that the internal pull-up resistor is
    // enabled on the GPIO pin so that one can connect up a sensor without
    // needing an external pull-up (Note: The internal (~47k) pull-ups of the
    // ESP do appear to work, at least for simple setups (one or two sensors
    // connected with short leads), but do not technically meet the pull-up
    // requirements from the ds18x20 datasheet and may not always be reliable.
    // For a real application, a proper 4.7k external pull-up resistor is
    // recommended instead!)
    gpio_set_pull_mode(SENSOR_GPIO, GPIO_PULLUP_ONLY);

    esp_err_t res;
    while (1)
    {
        // Every RESCAN_INTERVAL samples, check to see if the sensors connected
        // to our bus have changed.
        res = ds18x20_scan_devices(SENSOR_GPIO, addrs, MAX_SENSORS, &sensor_count);
        if (res != ESP_OK)
        {
            ESP_LOGE(TAG, "Sensors scan error %d (%s)", res, esp_err_to_name(res));
            continue;
        }

        if (!sensor_count)
        {
            ESP_LOGW(TAG, "No sensors detected!");
            continue;
        }

        ESP_LOGI(TAG, "%d sensors detected", sensor_count);

        // If there were more sensors found than we have space to handle,
        // just report the first MAX_SENSORS..
        if (sensor_count > MAX_SENSORS)
            sensor_count = MAX_SENSORS;

        // Do a number of temperature samples, and print the results.
        for (int i = 0; i < RESCAN_INTERVAL; i++)
        {
            ESP_LOGI(TAG, "Measuring...");

            res = ds18x20_measure_and_read_multi(SENSOR_GPIO, addrs, sensor_count, temps);
            if (res != ESP_OK)
            {
                ESP_LOGE(TAG, "Sensors read error %d (%s)", res, esp_err_to_name(res));
                continue;
            }

            for (int j = 0; j < sensor_count; j++)
            {
                float temp_c = temps[j];
                float temp_f = (temp_c * 1.8) + 32;
                char    temp_buffer[10];
                // Float is used in printf(). You need non-default configuration in
                // sdkconfig for ESP8266, which is enabled by default for this
                // example. See sdkconfig.defaults.esp8266
                // can't get that to work. :(
                ESP_LOGI(TAG, "Sensor %08" PRIx32 "%08" PRIx32 " (%s) reports %.3f°C (%.3f°F)",
                        (uint32_t)(addrs[j] >> 32), (uint32_t)addrs[j],
                        sensor_type(addrs[j]),
                        temp_c, temp_f);

                printf("%d C %d F\n", (int)(temp_c*100),  (int)(temp_f*100));
                printf("%.2f°F\n", temp_f);
                // MQTT payload should look something like
                // {"t":1746220529, "temp":77.00, "device":"DS18B20", "DS18B20_ID":"2838ef07b6013cd5"}
                // and we can add the heap and uptime as well.
                snprintf(topic_buf, topic_len, "{\"heap\":%d, \"t\":%ld, \"uptime\":%ld, \"temp\":%s, \"device\":\"%s\"}",
                    esp_get_free_heap_size(), now, uptime, format_temp(temp_f, temp_buffer), sensor_type(addrs[j]));
                // TODO: build topic with host name and sensor ID
                mqtt_publish("HA/esp8266.1/test/DS18B20_temp", topic_buf);
            }


            // Wait for a little bit between each sample (note that the
            // ds18x20_measure_and_read_multi operation already takes at
            // least 750ms to run, so this is on top of that delay).
            vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_MS));
        }
    }
}

void init_DS18B20(void)
{
    xTaskCreate(ds18x20_publisher, "publisher", configMINIMAL_STACK_SIZE * 4, NULL, 5, NULL);
}

/*
void app_main()
{
    xTaskCreate(ds18x20_test, "ds18x20_test", configMINIMAL_STACK_SIZE * 4, NULL, 5, NULL);
}
    */
