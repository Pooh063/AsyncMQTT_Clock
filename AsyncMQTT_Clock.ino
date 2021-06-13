#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <NTPClient.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <GyverTM1637.h>
#include "myconfig.h"

char msgDATE[] = BUILD_DATE;
String pathSketch = __FILE__;
String mqttPath = MQTT_DEVICE_NAME "s/" MQTT_DEVICE_NAME "_" + String(ESP.getChipId(), HEX);
String mqttDeviceID = MQTT_DEVICE_NAME "_" + String(ESP.getChipId(), HEX);
bool showDOTs = false;
bool synchronized = false;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

Ticker wifiReconnectTimer;
Ticker mqttReconnectTimer;

AsyncMqttClient mqttClient;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET * 60 * 60);

RtcDS3231<TwoWire> RTC(Wire);

GyverTM1637 disp(CLK_PIN, DIO_PIN);

void setup() {
  initDisplay();
  configRTC();
  getDATE();
  initWIFI();
  initOTA();
  initMQTT();
  initNTP();
}

void loop() {
  ArduinoOTA.handle();
  updateNTP();
  showTime(500); // Every half a second
  sendUptime(300000); // Every 5 minutes
}
