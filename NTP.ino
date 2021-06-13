void initNTP(void) {
  timeClient.begin();
}

void updateNTP(void) {
  yield();
  if (!synchronized) {
    if (timeClient.update()) {
      synchronized = true;
      disp.point(true);
      disp.displayClock(timeClient.getHours(), timeClient.getMinutes());

      RtcDateTime timeToSet;
      timeToSet.InitWithEpoch32Time(timeClient.getEpochTime());
      RTC.SetDateTime(timeToSet);
      delay (10000);
    }
  }
}
