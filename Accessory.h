#ifndef ACCESSORY_H_
#define ACCESSORY_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef DCC_MESSAGE_H_
#include "DccMessage.h"
#endif /* DCC_MESSAGE_H_ */

#ifndef ISERIALCOMMANDS_H_
#include "ISerialCommands.h"
#endif /* ISERIALCOMMANDS_H_ */

class Accessory : public ISerialCommands /* abstract */ {
private:
  bool m_isDebug = false;

public:
  // main handling methods of an accessory decoder
  virtual void setup() = 0;

  bool process(DccMessage &aMsg);

  bool isDebug() { return m_isDebug; }


public:
  // wiring up od some serial commands
  virtual void cmdDebug(bool aEnabled);
  virtual void cmdReset();
  virtual void cmdList();
  virtual int16_t cmdAddressGet(uint16_t aCVHi = 9, uint16_t aCVLo = 1);
  virtual void cmdAddressSet(uint16_t aAddress, uint16_t aCVHi = 9,
                             uint16_t aCVLo = 1);
  virtual int16_t cmdCVGet(uint16_t aCV);
  virtual void cmdCVSet(uint16_t aCV, uint8_t aValue);

protected:
  virtual uint16_t getAddress();
  // CV8 = 8 does execute decoder reset
  virtual void resetCV();
  virtual void listCV(uint16_t aCV);
  virtual bool getCV(uint16_t aCV, uint8_t &aReadValue);
  virtual bool setCV(uint16_t aCV, uint8_t aWriteValue);

  // Methods to overwrite to wire up actors
  virtual void handle(DccMessage &aMsg) = 0;
};

#endif /* ACCESSORY_H_ */
