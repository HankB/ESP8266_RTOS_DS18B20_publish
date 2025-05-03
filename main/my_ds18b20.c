/*
My wrapper around the DS18B20 driver

*/

#include "zh_ds18b20.h"

#include "my_ds18b20.h"

static uint8_t *rom = NULL;
static float temperature = 0.0;

void init_ds18b20(void)
{
    esp_log_level_set("zh_onewire", ESP_LOG_NONE);
    esp_log_level_set("zh_ds18b20", ESP_LOG_NONE);
    zh_onewire_init(GPIO_NUM_2);
    if (zh_onewire_reset() != ESP_OK)
    {
        printf("There are no 1-Wire devices available on the bus.\n");
    }
    else
    {
        zh_onewire_search_rom_init();
        for (;;)
        {
            rom = zh_onewire_search_rom_next();
            if (rom == NULL)
            {
                break;
            }
            printf("Found device ROM: ");
            for (uint8_t i = 0; i < 8; ++i)
            {
                printf("%X ", *(rom++));
            }
            printf("\n");
            rom -= 8;
            zh_ds18b20_read(rom, &temperature);
            printf("Temperature: %d\n", (int)(temperature*1000.0));
        }
    }
}

int read_ds18b20(void) {
    zh_ds18b20_read(rom, &temperature);
    return (int)(temperature*1000.0); // capture the fractional part of the data.
}
