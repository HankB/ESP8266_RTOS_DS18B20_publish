#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
PROJECT_NAME := ESP8266_RTOS_DS18B20_publish
EXTRA_COMPONENT_DIRS := /home/hbarta/Programming/esp-idf-lib/components
EXCLUDE_COMPONENTS := ads130e08 max7219 mcp23x17 led_strip max31865 ls7366r max31855
include $(IDF_PATH)/make/project.mk
