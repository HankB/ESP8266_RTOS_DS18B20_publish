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
* Add DS18B20

## DS18B20 driver

* <https://github.com/aZholtikov/zh_ds18b20>
* <https://github.com/aZholtikov/zh_onewire>

Results are inconsistent. A couple times the driver reported a valid device but never measured a temperature:

```text
I (1519) zh_onewire: Onewire initialization success.
I (1527) zh_onewire: Onewire reset begin.
I (1536) zh_onewire: Onewire reset success.
I (1542) zh_onewire: Onewire search ROM begin.
I (1550) zh_onewire: Onewire reset begin.
I (1558) zh_onewire: Onewire reset success.
I (1591) zh_onewire: Onewire search ROM success.
Found device ROM: 28 DE 2F 57 0 0 0 0 
I (1594) zh_ds18b20: DS18B20 read begin.
I (1596) zh_onewire: Onewire match ROM begin.
I (1604) zh_onewire: Onewire reset begin.
I (1612) zh_onewire: Onewire reset success.
I (1630) zh_onewire: Onewire match ROM success.
I (2375) zh_onewire: Onewire match ROM begin.
I (2376) zh_onewire: Onewire reset begin.
I (2379) zh_onewire: Onewire reset success.
I (2392) zh_onewire: Onewire match ROM success.
E (2406) zh_ds18b20: DS18B20 read fail. Invalid CRC.
Temperature: 
I (2408) zh_onewire: Onewire search ROM begin.
I (2409) zh_onewire: Onewire search ROM terminated.
```1

Usually

```text
I (1519) zh_onewire: Onewire initialization success.
I (1527) zh_onewire: Onewire reset begin.
E (1535) zh_onewire: Onewire reset fail. Timeout exceeded.
There are no 1-Wire devices available on the bus.
```

## Errata

Build error in Debian Testing (AKA Trixie) complaining about missing ncurses library is fixed by the tweak listed in <https://github.com/espressif/esp-idf/issues/14809#issuecomment-2450694593>.
