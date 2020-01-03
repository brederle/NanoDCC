#ifndef DCC_MESSAGE_H_
#include "DccMessage.h"
#endif /* DCC_MESSAGE_H_ */

void DccMessage::serialPrint() {
  // i sacrifice a litte bit RAM to
  // use the String conversion routines
  // ATTENTION: always be careful with String
  // due to difficultly contorllable RAM usage
  // Binary preamble
  Serial.print("[");
  Serial.print(String(m_packet0, BIN));
  Serial.print(" ");
  Serial.print(String(m_packet1, BIN));
  Serial.print(" ");
  Serial.print(String(m_packet2, BIN));
  if (m_numPackets > 3) {
    Serial.print(" ");
    Serial.print(String(m_packet3, BIN));
  }
  if (m_numPackets > 4) {
    Serial.print(" ");
    Serial.print(String(m_packet4, BIN));
  }
  if (m_numPackets > 4) {
    Serial.print(" ");
    Serial.print(String(m_packet5, BIN));
  }
  Serial.print("]=");
  // message dependent output
  if (isGA()) {
    Serial.print("GA ");
    Serial.print(String(getGAddress(), DEC));
    Serial.print(" ");
    Serial.print(String(getGAPort(), DEC));
    Serial.print(" ");
    Serial.print(String(getGAValue(), DEC));
  } else if (isExtGA()) {
    Serial.print("eGA ");
    Serial.print(String(getExtGAddress(), DEC));
    Serial.print(" ");
    Serial.print(String(getExtGAValue(), DEC));
  } else if (isSM()) {
    Serial.print("SM ");
    if (isWriteSM()) {
      Serial.print(String(getSMAddress(), DEC));
      Serial.print("=");
      Serial.print(String(getSMValue(), HEX));
    } else if (isWriteBitSM()) {
      // TODO
    }
  } else if (isReset()) {
    Serial.print("reset");
  } else if (isIdle()) {
    Serial.print("idle");
  } else {
    Serial.print("?? ");
    Serial.print(String(m_packet0, HEX));
    Serial.print(" ");
    Serial.print(String(m_packet1, HEX));
    Serial.print(" ");
    Serial.print(String(m_packet2, HEX));
    if (m_numPackets > 3) {
      Serial.print(" ");
      Serial.print(String(m_packet3, HEX));
    }
    if (m_numPackets > 4) {
      Serial.print(" ");
      Serial.print(String(m_packet4, HEX));
    }
    if (m_numPackets > 4) {
      {
        Serial.print(" ");
        Serial.print(String(m_packet5, HEX));
      }
    }
  }
}

bool DccMessage::operator==(DccMessage &msg) {
  if (size() != msg.size())
    return false;
  if ((m_packet0 != msg.m_packet0) || (m_packet1 != msg.m_packet1) ||
      (m_packet2 != msg.m_packet2))
    return false;
  if ((size() > 3) && (m_packet3 != msg.m_packet3))
    return false;
  if ((size() > 4) && (m_packet4 != msg.m_packet4))
    return false;
  if ((size() > 5) && (m_packet5 != msg.m_packet5))
    return false;

  return true;
}
