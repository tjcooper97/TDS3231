#ifndef TDS3231_H
#define TDS3231_H
  #include <TSoftwareVersion.h>
    #define TDS3231_V_MAJOR 1
    #define TDS3231_V_MINOR 1
    #define TDS3231_V_PATCH 1
    extern const TSoftwareVersion TDS3231_Version;

  #include <Adafruit_I2CDevice.h>
  #include <Arduino.h>
  #include <TTime.h>


  #define DS3231_ADDRESS 0x68
  #define REG_TIME       0x00
  #define REG_CONTROL    0x0E
  #define REG_STATUS     0x0F
  #define REG_TEMP       0x11 // 0x12 = lower byte, 10-bit value

  inline uint8_t ttdowToDs3231(uint8_t d);


  class TDS3231 {
    protected:
      static uint8_t _bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }
      static uint8_t _bin2bcd(uint8_t val) { return val + 6 * (val / 10); }

      Adafruit_I2CDevice *i2c_dev = NULL;
      uint8_t _readregister(uint8_t reg);
      void    _writeregister(uint8_t reg, uint8_t val);
      
    public:
      bool begin(TwoWire *wireInstance = &Wire);

      bool lostPower();

      void   set(const TTime &tt);
      TTime  getNow();

      double getTemperature();
      double getTemperatureF();
  };
#endif