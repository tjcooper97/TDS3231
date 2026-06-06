#include "TDS3231.h"

TDS3231 rtc;
TTime_Full now;


void printDateTime() { Serial.print(now.getDate()); Serial.print(" - "); Serial.println(now.getTime()); }


void setup() { 
  Serial.begin(115200);
  Serial.println(F("TDS3231 Library Demo:"));
  Serial.println(F("  -Looking for DS3231"));

  if (!rtc.begin()) { 
    Serial.println(""); Serial.println(F("* DS3231 NOT FOUND! *")); 
    while (true) { delay(1000); }; 
  };
  Serial.println(F("  -Found!"));

  if (rtc.lostPower()) { Serial.println(F("  *DS3231 has lost power since last boot!")); };

  TTime starttime(2026,6,24,15,30,15);
  now = starttime;
  now.showCentury(true); now.showSeconds(true);
  now.setDateFormat(FORMAT_DATE_US); now.setTimeFormat(FORMAT_TIME_24);
  rtc.set(now);
  Serial.print(F("  -Date/Time set to: ")); printDateTime();
  Serial.println(F("Setup complete!"));
}

void loop() { now = rtc.getNow(); printDateTime(); delay(1000); }