#include "TDS3231.h"

TDS3231 rtc;

TTime starttime(2024,12,31,4,20,0);
TTime_Full starttimef(starttime);

void setup() { if (!rtc.begin()) { while (true) { delay(1000); }; }; rtc.set(starttimef); }
void loop() { starttimef = rtc.getNow(); delay(1000); }