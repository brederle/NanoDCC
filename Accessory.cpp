#ifndef ACCESSORY_H_
#include "Accessory.h"
#endif /* ACCESSORY_H_ */

#ifndef EEPROM_H
#include <EEPROM.h>
#define EEPROM_H
#endif

// we store the CV at the same positions in
// EEPROM (waste, but valid simplification)

#define ADDRLO 1 // Basic address part 1
#define ADDRHI 9 // Basic address part 2
// CV2 to 4 are used by actors differently
// CV7 and CV8 have special handling by default
#define CV28 28 // TODO: BiDi communication
#define CV29 29 // TODO: Accessory configuration

/*******************************************************************
 * Main handling routine to filter and process relevant DCC
 * messages for this decoder. The method is usually used in main
 * loop.
 *******************************************************************/
bool Accessory::process(DccMessage &aMsg) {
  String cmd;
  bool result = false;

  if (aMsg.isGA()) { 
    if (getAddress() == aMsg.getGAddress()) {
      handle(aMsg);
      result = true;
    }

    if (isDebug()) {
      aMsg.serialPrint();
      Serial.println();
    }
  }
  //else if (isSM()) {
  //
  //}

  return false;
}

uint16_t Accessory::getAddress() {
  return (((uint16_t)(EEPROM[ADDRHI] & 0b00000111) << 6) | (EEPROM[ADDRLO] & 0b00111111));
}

void Accessory::resetCV() {
  // Default address 1
  EEPROM[ADDRLO] = 0x01;
  EEPROM[ADDRHI] = 0x00;

  // no BiDi (yet)
  EEPROM[CV28] = 0x02;
  EEPROM[CV29] = 0x08;

  if (isDebug()) {
    Serial.println("Accessory CV reset.");
  }
}

void Accessory::listCV(uint16_t aCV) {
  switch (aCV) {
  case 1:
    Serial.print("CV001,CV009: ");
    Serial.print(String(getAddress(), DEC));
    Serial.println(" //address");
    break;
  case 28:
    Serial.print("CV028......: 0b");
    Serial.print(String(EEPROM[CV28], BIN));
    Serial.println(" //Bidi");
    break;
  case 29:
    Serial.print("CV029......: 0b");
    Serial.print(String(EEPROM[CV29], BIN));
    Serial.println(" //Acc.config");
    break;
  }
}

bool Accessory::getCV(uint16_t aCV, uint8_t &aReadValue) {
  bool isGet = true; 

  switch (aCV) {
  case 1:
    aReadValue = EEPROM[ADDRLO];
    break;
  case 8:
    aReadValue = 0x0D; // Public domain decoder vendor ID
  case 9:
    aReadValue = EEPROM[ADDRHI];
    break;
  case 28:
    aReadValue = EEPROM[CV28];
    break;
  case 29:
    aReadValue = EEPROM[CV29];
    break;
  default:
    isGet = false;
    break;
  }

  if (isDebug() && isGet) {
    Serial.print("Accessory CV");
    Serial.print(String(aCV, HEX));
    Serial.print('=');
    Serial.println(String(aCV, HEX));
  }

  return isGet;
}

bool Accessory::setCV(uint16_t aCV, uint8_t aWriteValue) {
  bool isSet = false; 

  switch (aCV) {
  case 1:
    EEPROM[ADDRLO] = aWriteValue;
    break;
  case 9:
    EEPROM[ADDRHI] = aWriteValue;
    break;
  case 8:
    resetCV();
    break;
  case 28:
    EEPROM[CV28] = aWriteValue;
    break;
  case 29:
    EEPROM[CV29] = aWriteValue;
    break;
  default:
    isSet = true;
    break;
  }

  if (isDebug() && isSet) {
    Serial.print("Accessory CV");
    Serial.print(String(aCV, HEX));
    Serial.print('=');
    Serial.println(String(aCV, HEX));
    Serial.print(" written.");
  }

  // TODO: aknowledge
  return isSet;
}

// wire up serial commands
void Accessory::cmdDebug(bool aEnabled) {
  m_isDebug = aEnabled;
}

void Accessory::cmdReset() { resetCV(); }

void Accessory::cmdList() {
  for (uint16_t cv = 1; cv <= 1024; cv++) {
    listCV(cv);
  }
}

int16_t Accessory::cmdAddressGet(uint16_t aCVHi = 9, uint16_t aCVLo = 1) {
  uint8_t hi = 0, lo = 0;

  getCV(aCVHi, hi);
  getCV(aCVLo, lo);
  return ((((uint16_t)hi & 0b00000111) << 6) | (lo & 0b00111111));
}

void Accessory::cmdAddressSet(uint16_t aAddress, uint16_t aCVHi = 9,
                              uint16_t aCVLo = 1) {
  setCV(aCVHi, (uint8_t)((aAddress & 0b111000000) >> 6));
  setCV(aCVLo, (uint8_t)(aAddress & 0b00111111));
}

int16_t Accessory::cmdCVGet(uint16_t aCV) {
  uint8_t value = -1;
  getCV(aCV, value);
  return value;
}

void Accessory::cmdCVSet(uint16_t aCV, uint8_t aValue) { setCV(aCV, aValue); }