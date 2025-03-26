#ifndef MY_MQTT_H
#define MY_MQTT_H

void mqtt_start(void);
void mqtt_publish(const char* topic, const char* payload);

#endif // MY_MQTT_H