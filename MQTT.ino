void initMQTT(void) {
  mqttClient.setCredentials(MQTT_USER, MQTT_PASS);
  mqttClient.setMaxTopicLength(512);
  mqttClient.setClientId(mqttDeviceID.c_str());
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String strTopic = String(topic);
  char message[len + 1];
  strlcpy(message, (char *) payload, len + 1);
  String strPayload = String((char*)message);
  int intPayLoad = strPayload.toInt();

  if (strTopic == (mqttPath + "/BRIGHTNESS").c_str()) {
    if ((intPayLoad >= 0) && (intPayLoad < 8)) {
      disp.brightness(intPayLoad);  // Brightness, 0 - 7 (minimum - maximum)
    }
  }
}

void connectToMqtt(void) {
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  String coreVersion = ESP.getCoreVersion();
  coreVersion.replace('_', '.');

  //mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/Core").c_str(), 0, true, ESP.getCoreVersion().c_str()); // Output format: 2_5_2
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/Core").c_str(), 0, true, coreVersion.c_str()); // Output format: 2.5.2
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/Build_date").c_str(), 0, true, BUILD_DATE);
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/Flash_date").c_str(), 0, true, msgDATE);
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/Flash_size").c_str(), 0, true, (String(ESP.getFlashChipRealSize()/1024, DEC) + " KiB").c_str());
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/Reset_Reason").c_str(), 0, true, ESP.getResetReason().c_str());
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/SDK").c_str(), 0, true, ESP.getSdkVersion());
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/FreeHeap").c_str(), 0, true, String(ESP.getFreeHeap(), DEC).c_str());
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/myName").c_str(), 0, true, pathSketch.substring(pathSketch.lastIndexOf('\\') + 1).c_str());
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/myIP").c_str(), 0, true, WiFi.localIP().toString().c_str());
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/myMAC").c_str(), 0, true, WiFi.macAddress().c_str());
  mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/uptime").c_str(), 0, true, "< 5 min ago");

  mqttClient.subscribe((mqttPath + "/BRIGHTNESS").c_str(), 1);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}
