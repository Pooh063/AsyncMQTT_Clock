void sendUptime(unsigned long interval) {
  yield();
  static unsigned long prevTime = 0;
  if (millis() - prevTime > interval) {
    prevTime = millis();
    if (mqttClient.connected()) {
      mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/uptime").c_str(), 0, true, getUptime().c_str());
      mqttClient.publish((mqttPath + MQTT_SERVICE_INFO_PATH + "/FreeHeap").c_str(), 0, true, String(ESP.getFreeHeap(), DEC).c_str());
    }
  }
}

String getUptime(void) {
  static unsigned int  _rolloverCount   = 0;    // Number of 0xFFFFFFFF rollover we had in millis()
  static unsigned long _lastMillis      = 0;    // Value of the last millis()
  unsigned long currentMilliSeconds = millis();

  if (currentMilliSeconds < _lastMillis) {
    _rolloverCount++;
  }

  _lastMillis = currentMilliSeconds;

  unsigned long currentSeconds = (0xFFFFFFFF / 1000 ) * _rolloverCount + (_lastMillis / 1000);
  uint8_t days = currentSeconds / (60 * 60 * 24);
  uint8_t hours = (currentSeconds / (60 * 60)) % 24;
  uint8_t minutes = (currentSeconds / 60) % 60;
  uint8_t seconds = currentSeconds % 60;

  String result = "";
  if (days)
    result += String(days) + " day";
  if (days > 1)  result += "s";
  if (hours)
    result += " " + String(hours) + " hour";
  if (hours > 1)  result += "s";
  if (minutes)
    result += " " + String(minutes) + " minute";
  if (minutes > 1)  result += "s";
  if (seconds)
    result += " " + String(seconds) + " second";
  if (seconds > 1)  result += "s";
  result.trim();
  return result + " ago";
}

void getDATE(void) {
  unsigned int month, day, year;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  sscanf(__DATE__, "%s %d %d", msgDATE, &day, &year);
  month = (strstr(month_names, msgDATE) - month_names) / 3 + 1;
  sprintf(msgDATE, "%02d.%02d.%04d", day, month, year); // Format: 08.03.2018
}
