#ifndef _MY_CONFIG_H
#define _MY_CONFIG_H

#define UTC_OFFSET 4
#define CLK_PIN 16
#define DIO_PIN 14
#define BUILD_DATE "11.06.2021"
#define WIFI_SSID "******"
#define WIFI_PASSWORD "******"
#define OTA_PASSWORD "*****"

#define MQTT_HOST IPAddress(192, 168, 1, 100)
#define MQTT_PORT 1883
#define MQTT_USER "*****"
#define MQTT_PASS "*****"

#define MQTT_DEVICE_NAME "Clock"
#define MQTT_SERVICE_INFO_PATH "/info"

#endif // _MY_CONFIG_H
