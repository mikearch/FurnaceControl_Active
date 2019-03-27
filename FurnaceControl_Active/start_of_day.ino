unsigned long daystart() {
  unsigned long timeofday = RTC.get();
  unsigned long parttime = timeofday % 86400;
  unsigned long midnight = timeofday - parttime;

  return midnight;
}
