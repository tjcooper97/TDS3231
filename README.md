# TDS3231 Library
I put this library together to provide just the core functionality I need out of  a DS3231 while also integrating with my TTime library.

A note about that however is that I use the Adafruit_BusIO library for I2C comms. Of course if you're already using Adafruit's wonderful libraries in your project then this adds no extra weight, but worth considering if you aren't.

## Examples
I've just made one 'demo.cpp' example. This example shows you literally everything there is to know about using this library to interact with your DS3231 and the basics for getting started with TTime.

## Dependencies
- [Adafruit_BusIO](https://github.com/adafruit/Adafruit_BusIO): Adafruit's wonderful I2C & SPI library
- [TSoftwareVersion](https://github.com/tjcooper97/TSoftwareVersion): All my libraries use this so that projects I make can report what library versions they're using
- [TTime](https://github.com/tjcooper97/TTime): My date/time library that I use in all of my libraries & projects