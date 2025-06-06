# ESP8266 RTOS DS18B20 publish

Copied from <https://github.com/HankB/ESP8266_RTOS_Start> using it more or less as a template.

FreeRTOS documentation at <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html>

## Purpose

Configure an ESP8266 to read DS18B20 sensor(s) and publish treadings via MQTT.

## Status

Properly formatted payload published 1/minute (but more cleanup needed.)

*~~Note: This is tested with the sensor on D4 (AKA #2) and that is the same as the built in LED so that is presently commented out.~~

DS18B20 DATA pin has been moved to D2 (#4) to eliminate conflict with the builtin LED.

* 2025-05-02 Working DS18B20
* 2025-04-13 WiFi, MQTT, and NTP working.
* 2025-03-26 Working on getting the starting point working, but first the README.

## TODO

* 2025-05-03 add DS18B20 ID to topic and payload.
* 2025-05-03 better host nbame based on MAC.
* ~~Get existing parts working.~~
* ~~Add DS18B20 - using <https://github.com/UncleRus/esp-idf-lib>~~

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
cat << EOF >> Makefile
EXTRA_COMPONENT_DIRS := /home/user/myprojects/esp/esp-idf-lib/components
EXCLUDE_COMPONENTS := ads130e08 max7219 mcp23x17 led_strip max31865 ls7366r max31855
EOF
```

and

```text
make defconfig # or "make menuconfig" to tweak things.
make # to build
make flash # to build & flash
make monitor # to start monitor. Exit with "<ctrl>]"
```

### Build environment setup

```text
python -m venv /home/hbarta/espressif_venv
cd /home/hbarta/espressif_venv
. bin/activate
python -m pip install -r $IDF_PATH/requirements.txt # Note: --user elided
```

## Errata

* Build error in Debian Testing (AKA Trixie) complaining about missing ncurses library is fixed by the tweak listed in <https://github.com/espressif/esp-idf/issues/14809#issuecomment-2450694593>.
* See also <https://github.com/HankB/ESP8266_Arduino_MQTT_NTP_DS18B20> for a variant of this project using the Arduino libraries and tool chain. The motivation for both variants is that an ESP32/Arduino variant <https://github.com/HankB/ESP32-Arduino-DS18B20_MQTT> runs for a few days and then stops. In addition I have several ESP8266s that I want to deploy using these sensors. I will test these for run time to see if either has the issue.
