#ifndef ISERIALCOMMANDS_H_
#define ISERIALCOMMANDS_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class ISerialCommands {
public:
  virtual void cmdDebug(bool aEnabled) = 0;
  virtual void cmdReset() = 0;
  virtual void cmdList() = 0;
  virtual int16_t cmdAddressGet(uint16_t aCVHi = 9, uint16_t aCVLow = 1) = 0;
  virtual void cmdAddressSet(uint16_t aAddress ,uint16_t aCVHi = 9, uint16_t aCVLow = 1) = 0;
  virtual int16_t cmdCVGet(uint16_t aCV) = 0;
  virtual void cmdCVSet(uint16_t aCV, uint8_t aValue) = 0;
};

#endif /* ISERIALCOMMANDS_H_ */
