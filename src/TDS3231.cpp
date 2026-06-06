#include "TDS3231.h"

const TSoftwareVersion TDS3231_Version(TDS3231_V_MAJOR, TDS3231_V_MINOR, TDS3231_V_PATCH);

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


bool TDS3231::lostPower() { return _readregister(REG_STATUS) >> 7; }


void TDS3231::set(const TTime &tt) {
  uint8_t buffer[8] = {REG_TIME, _bin2bcd(tt.getSec()), _bin2bcd(tt.getMin()), _bin2bcd(tt.getHour()),
                                 _bin2bcd(ttdowToDS3231(tt.getDayOfWeek())),
                                 _bin2bcd(tt.getDay()), _bin2bcd(tt.getMon()), _bin2bcd(tt.getYear() - 2000U)};
  i2c_dev->write(buffer, 8);

  uint8_t statreg = _readregister(REG_STATUS);
  statreg &= ~0x80; // flip OSF bit
  _writeregister(REG_STATUS, statreg);
}


TTime TDS3231::getNow() {
  uint8_t buffer[7];
  buffer[0] = 0;
  i2c_dev->write_then_read(buffer, 1, buffer, 7);

  return TTime(_bcd2bin(buffer[6]) + 2000U, _bcd2bin(buffer[5] & 0x7F), _bcd2bin(buffer[4]), 
               _bcd2bin(buffer[2]),         _bcd2bin(buffer[1]),        _bcd2bin(buffer[0] & 0x7F));
}


double TDS3231::getTemperature() {
  uint8_t buffer[2] = {REG_TEMP, 0};
  i2c_dev->write_then_read(buffer, 1, buffer, 2);
  return (double)buffer[0] + (buffer[1] >> 6) * 0.25f;
}
double TDS3231::getTemperatureF() { return (getTemperature()*1.8)+32; }