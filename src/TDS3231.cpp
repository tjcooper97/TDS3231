#include "TDS3231.h"

const TSoftwareVersion TDS3231_Version(TDS3231_V_MAJOR, TDS3231_V_MINOR, TDS3231_V_PATCH);

#ifdef __AVR__
  #include <avr/pgmspace.h>
#elif defined(ARDUINO_SAM_DUE)
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif




inline uint8_t ttdowToDS3231(uint8_t d) { return d == 1 ? 7 : d-1; }





void TDS3231::_writeregister(uint8_t reg, uint8_t val) {
  uint8_t buffer[2] = {reg, val};
  i2c_dev->write(buffer, 2);
}

uint8_t TDS3231::_readregister(uint8_t reg) {
  uint8_t buffer[1];
  i2c_dev->write(&reg, 1);
  i2c_dev->read(buffer, 1);
  return buffer[0];
}




bool TDS3231::begin(TwoWire *wireInstance) {
  if (i2c_dev) { delete i2c_dev; };
  i2c_dev = new Adafruit_I2CDevice(DS3231_ADDRESS, wireInstance);
  return (i2c_dev->begin());
}


Ds3231SqwPinMode TDS3231::readSqwPinMode() {
  int mode = _readregister(DS3231_CONTROL) & 0x1C;
  if (mode & 0x04) { mode = DS3231_OFF; };
  return static_cast<Ds3231SqwPinMode>(mode);
}

void TDS3231::writeSqwPinMode(Ds3231SqwPinMode mode) {
  uint8_t ctrl = _readregister(DS3231_CONTROL);
  ctrl &= ~0x04; // turn off INTCON
  ctrl &= ~0x18; // set freq bits to 0
  _writeregister(DS3231_CONTROL, ctrl | mode);
}


bool TDS3231::lostPower(void) { return _readregister(DS3231_STATUSREG) >> 7; }


void TDS3231::set(const TTime &tt) {
  uint8_t buffer[8] = {DS3231_TIME, bin2bcd(tt.getSec()), bin2bcd(tt.getMin()), bin2bcd(tt.getHour()),
                                    bin2bcd(ttdowToDS3231(tt.getDayOfWeek())),
                                    bin2bcd(tt.getDay()), bin2bcd(tt.getMon()), bin2bcd(tt.getYear() - 2000U)};
  i2c_dev->write(buffer, 8);

  uint8_t statreg = _readregister(DS3231_STATUSREG);
  statreg &= ~0x80; // flip OSF bit
  _writeregister(DS3231_STATUSREG, statreg);
}

TTime TDS3231::getNow() {
  uint8_t buffer[7];
  buffer[0] = 0;
  i2c_dev->write_then_read(buffer, 1, buffer, 7);

  return TTime(bcd2bin(buffer[6]) + 2000U, bcd2bin(buffer[5] & 0x7F), bcd2bin(buffer[4]), 
               bcd2bin(buffer[2]),         bcd2bin(buffer[1]),        bcd2bin(buffer[0] & 0x7F));
}

double TDS3231::getTemperature() {
  uint8_t buffer[2] = {DS3231_TEMPERATUREREG, 0};
  i2c_dev->write_then_read(buffer, 1, buffer, 2);
  return (double)buffer[0] + (buffer[1] >> 6) * 0.25f;
}
double TDS3231::getTemperatureF() { return (getTemperature()*1.8)+32; }