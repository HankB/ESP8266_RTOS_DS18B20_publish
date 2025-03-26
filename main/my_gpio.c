/* code to init GPIO and flash an on-board LED
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "my_gpio.h"


#define GPIO_LED    2 // shared with serial TX
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_LED)
static void blink_LED_task(void *arg);

void init_gpio(void) {

    // flashing the blue LED messes up serial I/O
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO15/16
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    xTaskCreate(blink_LED_task, "gpio_task", 1024, NULL, 10, NULL);
}


static void blink_LED_task(void *arg)
{
    while(true) {
        vTaskDelay(240 / portTICK_RATE_MS);
        gpio_set_level(GPIO_LED, 0);        // blue LED on
        vTaskDelay(10 / portTICK_RATE_MS);
        gpio_set_level(GPIO_LED, 1);        // blue LED off
    }
}