#ifndef TDS3231_H
#define TDS3231_H
  #include <TSoftwareVersion.h>
    #define TDS3231_V_MAJOR 1
    #define TDS3231_V_MINOR 1
    #define TDS3231_V_PATCH 0
    extern const TSoftwareVersion TDS3231_Version;

  #include <Adafruit_I2CDevice.h>
  #include <Arduino.h>
  #include <TTime.h>


  #define DS3231_ADDRESS        0x68 // I2C address for DS3231
  #define DS3231_TIME           0x00 // Time register
  #define DS3231_ALARM1         0x07 // Alarm 1 register
  #define DS3231_ALARM2         0x0B // Alarm 2 register
  #define DS3231_CONTROL        0x0E // Control register
  #define DS3231_STATUSREG      0x0F // Status register
  #define DS3231_TEMPERATUREREG 0x11 // Temperature register (high byte - low byte is at 0x12), 10-bit temperature value

  inline uint8_t ttdowToDs3231(uint8_t d);



  enum Ds3231SqwPinMode {
    DS3231_OFF = 0x1C,            /**< Off */
    DS3231_SquareWave1Hz = 0x00,  /**<  1Hz square wave */
    DS3231_SquareWave1kHz = 0x08, /**<  1kHz square wave */
    DS3231_SquareWave4kHz = 0x10, /**<  4kHz square wave */
    DS3231_SquareWave8kHz = 0x18  /**<  8kHz square wave */
  };

  class TDS3231 {
    protected:
      static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }
      static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }
      Adafruit_I2CDevice *i2c_dev = NULL; // Pointer to I2C bus interface
      uint8_t _readregister(uint8_t reg);
      void _writeregister(uint8_t reg, uint8_t val);
      
    public:
      bool begin(TwoWire *wireInstance = &Wire);
      
      Ds3231SqwPinMode readSqwPinMode();
      void writeSqwPinMode(Ds3231SqwPinMode mode);

      bool   lostPower(void);

      void   set(const TTime &tt);
      TTime  getNow();

      double getTemperature();
      double getTemperatureF();
  };
#endif