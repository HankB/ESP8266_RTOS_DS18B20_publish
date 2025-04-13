/* Code inspired from 
ESP8266_RTOS_SDK/examples/protocols/sntp
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "driver/gpio.h"

#include "lwip/apps/sntp.h"

static const char *TAG = "sntp";

void init_sntp(void)
{
    ESP_LOGI(TAG, "Init SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();    
}
