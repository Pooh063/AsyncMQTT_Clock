void initDisplay(void) {
  disp.clear();
  disp.brightness(2);  // Brightness, 0 - 7 (minimum - maximum)
}

void showTime(unsigned long interval) {
  yield();
  static unsigned long prevTime = 0;
  if (millis() - prevTime > interval) {
    prevTime = millis();
    showDOTs = !showDOTs;
    disp.point(showDOTs);
    if (showDOTs) {
      RtcDateTime currentTime = RTC.GetDateTime();
      disp.displayClock(currentTime.Hour(), currentTime.Minute());
    }
  }
}
