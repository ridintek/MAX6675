#ifndef _MAX6675_H_
#define _MAX6675_H_

#include <Arduino.h>
#include <SPI.h>

#define MAX6675_NO_TEMPERATURE -999

//  STATE constants returned by read()
//  TODO check
#define STATUS_OK 0x00
#define STATUS_ERROR 0x04
#define STATUS_NOREAD 0x80
#define STATUS_NO_COMMUNICATION 0x81

//  Thermocouples working is based upon Seebeck effect.
//  Different TC have a different Seebeck Coefficient  (µV/°C)
//  See
//  http://www.analog.com/library/analogDialogue/archives/44-10/thermocouple.html
//
//  K_TC == 41.276 µV/°C

class MAX6675 {
 public:
  // HW SPI
  MAX6675(uint8_t cs, SPIClass* spi = &SPI);
  // SW SPI
  MAX6675(uint8_t cs, uint8_t miso, uint8_t sck);

  void begin();

  //       returns state - bit field: 0 = STATUS_OK
  uint8_t read();
  float getCelsius(void) { return _temperature + _offset; };
  float getFahrenheit(void) { return getCelsius() * 1.8 + 32; };

  uint8_t getStatus(void) const { return _status; };

  //       use offset to calibrate the TC.
  //       offset is in degrees Celsius.
  void setOffset(const float t) { _offset = t; };
  float getOffset() const { return _offset; };

  uint32_t lastRead() { return _lastTimeRead; };
  uint16_t getRawData() { return _rawData; };

  //       speed in Hz
  void setSPIspeed(uint32_t speed);
  uint32_t getSPIspeed() { return _SPIspeed; };
  void setSWSPIdelay(uint16_t del = 0) { _swSPIdelay = del; };
  uint16_t getSWSPIdelay() { return _swSPIdelay; };

  //  obsolete in future.
  float getTemperature(void) { return _temperature + _offset; };

 private:
  uint32_t _read();

  uint8_t _status;
  float _temperature;
  float _offset;
  uint32_t _lastTimeRead;
  uint16_t _rawData;
  bool _hwSPI;

  uint8_t _clock;
  uint8_t _miso;
  uint8_t _select;

  uint16_t _swSPIdelay = 0;
  uint32_t _SPIspeed;
  SPIClass* _mySPI;
  SPISettings _spi_settings;
};

#endif  // _MAX6675_H_