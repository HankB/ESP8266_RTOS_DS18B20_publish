# ESP8266 RTOS DS18B20 publish

Copied from <https://github.com/HankB/ESP8266_RTOS_Start> using it more or less as a template.

FreeRTOS documentation at <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html>

## Purpose

Configure an ESP8266 to read DS18B20 sensor(s) and publish treadings via MQTT.

## Build

"Getting Started" guide at <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html>

Following instructions at <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html#get-started-get-esp-idf>. i added ENV VARS and path in `~/.profile`.

* Install the toolchain per <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html> (on Debian Bookworm in my case.)
* Get the ESP8266_RTOS_SDK <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html#get-esp8266-rtos-sdk>. When it came to Python, I installed `python-is-python3` and created a virtual environment using `venv` in which to install the necessary modules.

### esp-idf-lib setup

*Following is a WIP* (Doing it manually to check out the library)

```text
cd .. # (from project folder)
git clone https://github.com/UncleRus/esp-idf-lib.git
cat << EOF >> components.mk
PROJECT_NAME := $(basename $PWD)
EXTRA_COMPONENT_DIRS := /home/user/myprojects/esp/esp-idf-lib/components
EXCLUDE_COMPONENTS := ads130e08 max7219 mcp23x17 led_strip max31865 ls7366r max31855
include $(IDF_PATH)/make/project.mk
```

```text
python -m venv /home/hbarta/espressif_venv
cd /home/hbarta/espressif_venv
. bin/activate
python -m pip install -r $IDF_PATH/requirements.txt # Note: --user elided
```

## Status

* 2025-04-13 WiFi, MQTT, and NTP working.
* 2025-03-26 Working on getting the starting point working, but first the README.

## TODO

* ~~Get existing parts working.~~
* Add DS18B20 - using <https://github.com/UncleRus/esp-idf-lib>

## Errata

Build error in Debian Testing (AKA Trixie) complaining about missing ncurses library is fixed by the tweak listed in <https://github.com/espressif/esp-idf/issues/14809#issuecomment-2450694593>.
