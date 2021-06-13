void initWIFI(void) {
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  mqttReconnectTimer.detach(); // Ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, initWIFI);
}

void initOTA(void) {
  ArduinoOTA.setHostname(mqttDeviceID.c_str());
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.begin();
}
