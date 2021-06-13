void configRTC(void) {
  RTC.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  RtcDateTime now = RTC.GetDateTime();

  if (!RTC.IsDateTimeValid()) RTC.SetDateTime(compiled);
  if (!RTC.GetIsRunning()) RTC.SetIsRunning(true);
  if (now < compiled) RTC.SetDateTime(compiled);

  RTC.Enable32kHzPin(false);
  RTC.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}
