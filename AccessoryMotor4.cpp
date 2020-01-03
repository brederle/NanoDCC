#ifndef ACCESSORYMOTOR4_H_
#include "AccessoryMotor4.h"
#endif /* ACCESSORYMOTOR4_H_ */

#ifndef MOTOR4ACTOR_H_
#include "Motor4Actor.h"
#endif /* MOTOR4ACTOR_H_ */

#ifndef EEPROM_H
#include <EEPROM.h>
#define EEPROM_H
#endif

#define VERSION 0x09

#define TIMEOUT0 3 // Turndown timeout 0
#define TIMEOUT1 4 // Turndown timeout 1
#define TIMEOUT2 5 // Turndown timeout 2
#define TIMEOUT3 6 // Turndown timeout 3

#define PWM0 35 // PWM value motor1
#define PWM1 36 // PWM value motor1
#define PWM2 37 // PWM value motor1
#define PWM3 38 // PWM value motor1

void AccessoryMotor4::setup() {
  m_motors.setup();
}

void AccessoryMotor4::process(DccMessage &aMsg) {
  Accessory::process(aMsg);
  //m_motors.processTimeouts();
}

void AccessoryMotor4::handle(DccMessage &aMsg) {
  if (aMsg.getGAValue() == 0) {
    switch (aMsg.getGAPort()) {
    case 0:
    case 1:
      m_motors.stop0();
      break;
    case 2:
    case 3:
      m_motors.stop1();
      break;
    case 4:
    case 5:
      m_motors.stop2();
      break;
    case 6:
    case 7:
      m_motors.stop3();
      break;
    }
  } else {
    switch (aMsg.getGAPort()) {
    case 0:
      m_motors.forward0(EEPROM[PWM0], EEPROM[TIMEOUT0]);
      break;
    case 1:
      m_motors.backward0(EEPROM[PWM0], EEPROM[TIMEOUT0]);
      break;
    case 2:
      m_motors.forward1(EEPROM[PWM1], EEPROM[TIMEOUT1]);
      break;
    case 3:
      m_motors.backward1(EEPROM[PWM1], EEPROM[TIMEOUT1]);
      break;
    case 4:
      m_motors.forward2(EEPROM[PWM2], EEPROM[TIMEOUT2]);
      break;
    case 5:
      m_motors.backward2(EEPROM[PWM2], EEPROM[TIMEOUT2]);
      break;
    case 6:
      m_motors.forward3(EEPROM[PWM3], EEPROM[TIMEOUT3]);
      break;
    case 7:
      m_motors.backward3(EEPROM[PWM3], EEPROM[TIMEOUT3]);
      break;
    }
  }
}

void AccessoryMotor4::resetCV() {
  Accessory::resetCV();

  // Timeout: 8*100ms, 0 means no auto switch off
  EEPROM[TIMEOUT0] = 0x08;
  EEPROM[TIMEOUT1] = 0x08;
  EEPROM[TIMEOUT2] = 0x08;
  EEPROM[TIMEOUT3] = 0x08;

  // PWM values, 0 means
  EEPROM[PWM0] = 0xc0;
  EEPROM[PWM1] = 0xc0;
  EEPROM[PWM2] = 0xc0;
  EEPROM[PWM3] = 0xc0;

  if (isDebug()) {
    Serial.println("Motor4 CV reset.");
  }
}

void AccessoryMotor4::listCV(uint16_t aCV) {
  switch (aCV) {
  case 3:
    Serial.print("CV003......: ");
    Serial.print(String(EEPROM[TIMEOUT0], DEC));
    Serial.println("*100ms //autoff1");
    break;
  case 4:
    Serial.print("CV004......: ");
    Serial.print(String(EEPROM[TIMEOUT1], DEC));
    Serial.println("*100ms //autoff2");
    break;
  case 5:
    Serial.print("CV005......: ");
    Serial.print(String(EEPROM[TIMEOUT2], DEC));
    Serial.println("*100ms //autoff3");
    break;
  case 6:
    Serial.print("CV006......: ");
    Serial.print(String(EEPROM[TIMEOUT3], DEC));
    Serial.println("*100ms //autoff4");
    break;
  case 35:
    Serial.print("CV0035.....: ");
    Serial.print(String(EEPROM[PWM0], DEC));
    Serial.println(" //speed1");
    break;
  case 36:
    Serial.print("CV0036.....: ");
    Serial.print(String(EEPROM[PWM1], DEC));
    Serial.println(" //speed2");
    break;
  case 37:
    Serial.print("CV0037.....: ");
    Serial.print(String(EEPROM[PWM2], DEC));
    Serial.println(" //speed3");
    break;
  case 38:
    Serial.print("CV0038.....: ");
    Serial.print(String(EEPROM[PWM3], DEC));
    Serial.println(" //speed4");
    break;
  default:
    Accessory::listCV(aCV);
    break;
  }
}

bool AccessoryMotor4::getCV(uint16_t aCV, uint8_t &aReadValue) {
  bool isGet = true;

  switch (aCV) {
  case 3:
    aReadValue = EEPROM[TIMEOUT0];
    break;
  case 4:
    aReadValue = EEPROM[TIMEOUT1];
    break;
  case 5:
    aReadValue = EEPROM[TIMEOUT2];
    break;
  case 6:
    aReadValue = EEPROM[TIMEOUT3];
    break;
  case 7:
    aReadValue = VERSION;
    break;
  case 35:
    aReadValue = EEPROM[PWM0];
    break;
  case 36:
    aReadValue = EEPROM[PWM1];
    break;
  case 37:
    aReadValue = EEPROM[PWM2];
    break;
  case 38:
    aReadValue = EEPROM[PWM3];
    break;
  default:
    isGet = false;
    break;
  }

  if (!isGet) {
    isGet = Accessory::getCV(aCV, aReadValue);
  } else if(isDebug()) {
    Serial.print("Motor4 CV");
    Serial.print(String(aCV, HEX));
    Serial.print('=');
    Serial.println(String(aCV, HEX));
  }

  return isGet;  
}

bool AccessoryMotor4::setCV(uint16_t aCV, uint8_t aWriteValue) {
  bool isSet = true;
  
  switch (aCV) {
  case 3:
    EEPROM[TIMEOUT0] = aWriteValue;
    break;
  case 4:
    EEPROM[TIMEOUT1] = aWriteValue;
    break;
  case 5:
    EEPROM[TIMEOUT2] = aWriteValue;
    break;
  case 6:
    EEPROM[TIMEOUT3] = aWriteValue;
    break;
  case 35:
    EEPROM[PWM0] = aWriteValue;
    break;
  case 36:
    EEPROM[PWM1] = aWriteValue;
    break;
  case 37:
    EEPROM[PWM2] = aWriteValue;
    break;
  case 38:
    EEPROM[PWM3] = aWriteValue;
    break;
  default:
    isSet = false;
    break;
  }

  // TODO: aknowledge
  if (!isSet) {
    isSet = Accessory::setCV(aCV, aWriteValue);
  } else if(isDebug()) {
    Serial.print("Motor4 CV");
    Serial.print(String(aCV, HEX));
    Serial.print('=');
    Serial.println(String(aCV, HEX));
    Serial.print(" written.");
  }

  return isSet;
}
