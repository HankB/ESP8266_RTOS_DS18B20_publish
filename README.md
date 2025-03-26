# ESP8266_RTOS_Start

Copied from `hello_world` example at `ESP8266_RTOS_SDK/examples/get-started/hello_world` and built/flashed/monitored on an ESP01 plugged into a USB adapter configured with pushbutton to short the required pins to flash the device.

From the original README

>Starts a FreeRTOS task to print "Hello World"

FreeRTOS documentation at <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html>

## Purpose

Build out to provide a starting point for projects that will use WiFi, MQTT, NTP, and GPIO.

## Build

"Getting Started" guide at <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html>

### Old

Assumes that the SDK is intalled in `~/esp` but can be set elsewhere using environment variables.

```text
export ESP_BASE_DIR=~/esp
export IDF_PATH=${ESP_BASE_DIR}/ESP8266_RTOS_SDK
. ${IDF_PATH}/export.sh
make
make flash # ESP-01 - need to plug in ESP01 while holding button
make monitor # need to unplug/replug ESP01 to get it to run.
```

### New

Following instructions at <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html#get-started-get-esp-idf>. i added ENV VARS and path in `~/.profile`.

* Install the toolchain per <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html> (on Debian Bookworm in my case.)
* Get the ESP8266_RTOS_SDK <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html#get-esp8266-rtos-sdk>. When it came to Python, I installed `python-is-python3` and created a virtual environment using `venv` in which to install the necessary modules.

```text
python -m venv /home/hbarta/espressif_venv
cd /home/hbarta/espressif_venv
. bin/activate
python -m pip install -r $IDF_PATH/requirements.txt # Note: --user elided
```

Building examples and this project then worked and installed successfully on an ESP-01 and `KeeYees ESP8266 ESP-12F`.

## Status

* 2025-03-26 This project <s>no longer</s> builds. 

```text
hbarta@olive:~/Programming/ESP8266/ESP8266_RTOS_Start$ export ESP_BASE_DIR=~/esp
hbarta@olive:~/Programming/ESP8266/ESP8266_RTOS_Start$ export IDF_PATH=${ESP_BASE_DIR}/ESP8266_RTOS_SDK
hbarta@olive:~/Programming/ESP8266/ESP8266_RTOS_Start$ . ${IDF_PATH}/export.sh
Adding ESP-IDF tools to PATH...
/usr/bin/env: ‘python’: No such file or directory
hbarta@olive:~/Programming/ESP8266/ESP8266_RTOS_Start$ make
make: xtensa-lx106-elf-gcc: No such file or directory
expr: syntax error: unexpected argument ‘5.2.0’
/bin/sh: 1: xtensa-lx106-elf-gcc: not found
/bin/sh: 1: xtensa-lx106-elf-gcc: not found
Toolchain path: 
WARNING: Failed to find Xtensa toolchain, may need to alter PATH or set one in the configuration menu
make: python: No such file or directory
make: *** No rule to make target 'check_python_dependencies', needed by '/home/hbarta/Programming/ESP8266/ESP8266_RTOS_Start/build/include/config/auto.conf'.  Stop.
hbarta@olive:~/Programming/ESP8266/ESP8266_RTOS_Start$
```

Apparently PIO support is lacking <https://github.com/platformio/platform-espressif8266/issues/219> Will have a quick go with the instructions listed above but I have a nagging recollection that RTOS may no longer be supported for the ESP8266. Back to the instructions at <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html#get-started-get-esp-idf> and it does seem to be working OK. I first built the "hello world" example and loaded it in an ESP-01 and then the 'KeeYees ESP8266 ESP-12F' recently purchased. It was unable to connect to the MQTT broker because I did not have one running.

* Cloned code from `hello_world` example builds and runs.
* GPIO output
* Associates with AP. (info output and GPIO commented out.) Works on NodeMCU but not ESP-01.
* SNTP working.

## TODO

* Add WiFi - partial, reconnect not tested
* Add MQTT - working, reconnect tested
* Add SNTP - working, usage in main a bit buggy

## Convenient host commands

```text
mosquitto_sub -v -t esp8266.1/pub/\#
systemctl status mqtt
sudo systemctl stop mosquitto
sudo tail -f /var/log/mosquitto/mosquitto.log
make menuconfig # to change build/config settings
```

## Errata

* `make monitor` will restart the device.
* `minicom` will not work at the default baud rate of 74880. But it will capture some boot messages put out at 115200 using the -C option. Used `make menuconfig` to set baudrate to 115200. (Still some output at ???)
* Setting GPIO_LED to 1 (to work on the ESP01) doesn't work on the NodeMCU board and causes serial monitor to no longer work.
