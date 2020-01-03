#ifndef DCC_MESSAGE_H_
#define DCC_MESSAGE_H_
/****************************************************************************
 * Fast DCC signal decoder in C++ for do-it-yourself arduino decoder
 *
 * Author: B. Rederlechner
 ****************************************************************************/


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#ifndef INTTYPES_H_
#include <inttypes.h>
#define INTTYPES_H_
#endif /* INTTYPES_H_ */

// packet1
#define INSTRUCTION_CONTROL (0b000 << 5)
#define INSTRUCTION_ADVANCED (0b001 << 5)
#define INSTRUCTION_SPEED_BACK (0b010 << 5)
#define INSTRUCTION_SPEED_FORWARD (0b011 << 5)
#define INSTRUCTION_FUNCTION1 (0b100 << 5)
#define INSTRUCTION_FUNCTION2 (0b101 << 5)
#define INSTRUCTION_FUTURE_USE (0b110 << 5)
#define INSTRUCTION_CV_ACCESS (0b111 << 5)

#define SM_ID_MASK 0b11110000
#define SM_ID 0b01110000
#define SM_INST_MASK 0b11110011
#define SM_INST_BIT 0b00001000
#define SM_INST_VERIFY 0b00000100
#define SM_INST_WRITE 0b00001100
#define SM_ADDR_MASK1 0b00000011

class DccMessage {
public:
  volatile uint8_t m_numPackets;
  uint8_t m_packet0, m_packet1, m_packet2, m_packet3, m_packet4, m_packet5;

public:
  byte instruction() __attribute__((always_inline)) {
    return (m_packet1 & 0b11100000);
  }

  int size() __attribute__((always_inline)) { return m_numPackets; }

  void clear() __attribute__((always_inline)) { m_numPackets = 0; }

  void saveMessage(uint8_t packet0, uint8_t packet1, uint8_t packet2)
      __attribute__((always_inline)) {
    // CRC check on copy
    if (m_packet2 == (m_packet1 ^ m_packet0)) {
      m_packet0 = packet0;
      m_packet1 = packet1;
      m_packet2 = packet2;
      m_numPackets = 3;
    } else {
      // required to avoid stalled evalution intervalls
      // looks like a race condition with main
      // when an invalid packet is stored 
      m_packet0 = 0;
      m_packet1 = 0;
      m_packet2 = 0;
      m_numPackets = 0;
    }
  }

  void saveMessage(uint8_t packet0, uint8_t packet1, uint8_t packet2,
                   uint8_t packet3) __attribute__((always_inline)) {
    if (m_packet3 == (m_packet2 ^ m_packet1 ^ m_packet0)) {
      m_packet0 = packet0;
      m_packet1 = packet1;
      m_packet2 = packet2;
      m_packet3 = packet3;
      m_numPackets = 4;
    } else {
      // required to avoid stalled evalution intervalls
      // looks like a race condition with main
      // when an invalid packet is stored 
      m_packet0 = 0;
      m_packet1 = 0;
      m_packet2 = 0;
      m_packet3 = 0;
      m_numPackets = 0;
    }
  }

  void saveMessage(uint8_t packet0, uint8_t packet1, uint8_t packet2,
                   uint8_t packet3, uint8_t packet4)
      __attribute__((always_inline)) {
    if (m_packet4 == (m_packet3 ^ m_packet2 ^ m_packet1 ^ m_packet0)) {
      m_packet0 = packet0;
      m_packet1 = packet1;
      m_packet2 = packet2;
      m_packet3 = packet3;
      m_packet4 = packet4;
      m_numPackets = 5;
    } else {
      // required to avoid stalled evalution intervalls
      // looks like a race condition with main
      // when an invalid packet is stored 
      m_packet0 = 0;
      m_packet1 = 0;
      m_packet2 = 0;
      m_packet3 = 0;
      m_packet4 = 0;
      m_numPackets = 0;
    }
  }

  void saveMessage(uint8_t packet0, uint8_t packet1, uint8_t packet2,
                   uint8_t packet3, uint8_t packet4, uint8_t packet5)
      __attribute__((always_inline)) {
    if (m_packet5 ==
        (m_packet4 ^ m_packet3 ^ m_packet2 ^ m_packet1 ^ m_packet0)) {
      m_packet0 = packet0;
      m_packet1 = packet1;
      m_packet2 = packet2;
      m_packet3 = packet3;
      m_packet4 = packet4;
      m_packet5 = packet5;
      m_numPackets = 6;
    } else {
      // required to avoid stalled evalution intervalls
      // looks like a race condition with main
      // when an invalid packet is stored 
      m_packet0 = 0;
      m_packet1 = 0;
      m_packet2 = 0;
      m_packet3 = 0;
      m_packet4 = 0;
      m_packet5 = 0;
      m_numPackets = 0;
    }
  }

  void copy(DccMessage& aMsg) __attribute__((always_inline)) {
      m_numPackets = aMsg.m_numPackets;
      m_packet0 = aMsg.m_packet0;
      m_packet1 = aMsg.m_packet1;
      m_packet2 = aMsg.m_packet2;
      m_packet3 = aMsg.m_packet3;
      m_packet4 = aMsg.m_packet4;
      m_packet5 = aMsg.m_packet5;    
  }


  /*** general methods ***/
  bool isReset() __attribute__((always_inline)) {
    return ((size() == 3) && (m_packet0 == 0) && (m_packet1 == 0));
  }

  bool isIdle() __attribute__((always_inline)) {
    return ((size() == 3) && (m_packet0 == 0xff) && (m_packet1 == 0));
  }

  /*** Accessory related methods ***/
  bool isGA() __attribute__((always_inline)) {
    return ((size() == 3) && 
            ((m_packet0 & 0b11000000) == 0b10000000) &&
            ((m_packet1 & 0b10000000) == 0b10000000));
  }

  bool isGABroadcast() __attribute__((always_inline)) {
    return ((m_packet0 == 0b10111111) &&
            ((m_packet1 & 0b11110000) == 0b1000000));
  }

  bool isExtGA() __attribute__((always_inline)) {
    return ((size() == 4) && ((m_packet0 & 0b11000000) == 0b10000000) &&
            ((m_packet1 & 0b10001001) == 0b00000001) &&
            ((m_packet2 & 0b11100000) == 0b00000000));
  }

  bool isExtGABroadcast() __attribute__((always_inline)) {
    return ((m_packet0 == 0b10111111) && (m_packet1 == 0b00000111));
  }

  uint16_t getGAddress() __attribute__((always_inline)) {
      return (((uint16_t)m_packet0 & 0b00111111) |
              (((uint16_t)(~m_packet1 & 0b01110000)) << 2));
  }

  uint16_t getExtGAddress() __attribute__((always_inline)) {
    return ((((uint16_t)m_packet0 & 0b00111111) << 5) |
            (((uint16_t)m_packet1 & 0b01110000) >> 2) |
            (((uint16_t)m_packet1 & 0b00000110) >> 1));
  }

  byte getGAPort() __attribute__((always_inline)) {
    return (m_packet1 & 0b00000111);
  }

  byte getGAValue() __attribute__((always_inline)) {
    return ((m_packet1 & 0b00001000) >> 3);
  }

  byte getExtGAValue() __attribute__((always_inline)) {
    return (m_packet2 & 0b00011111);
  }

  /* CV/ServiceMode related methods */
  bool isSM() __attribute__((always_inline)) {
    return ((m_packet0 & SM_ID_MASK) == SM_ID);
  }

  bool isWriteSM() __attribute__((always_inline)) {
    return ((m_packet1 & SM_INST_MASK) == SM_INST_WRITE);
  }

  bool isWriteBitSM() __attribute__((always_inline)) {
    return ((m_packet1 & SM_INST_MASK) == SM_INST_BIT);
  }

  bool isVerifySM() __attribute__((always_inline)) {
    return ((m_packet1 & SM_INST_MASK) == SM_INST_VERIFY);
  }

  uint16_t getSMAddress() __attribute__((always_inline)) {
    return (((uint16_t)(m_packet0 & SM_ADDR_MASK1) << 8) | (m_packet1));
  }

  uint16_t getSMValue() __attribute__((always_inline)) { return (m_packet2); }

  void serialPrint();
  bool operator==(DccMessage &msg);
  bool operator!=(DccMessage &msg) __attribute__((always_inline)) {
    return !operator==(msg);
  }
};

#endif /*DCC_MESSAGE_H_*/
