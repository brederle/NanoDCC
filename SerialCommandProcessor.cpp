#ifndef SERIALCOMMANDPROCESSOR_H_
#include "SerialCommandProcessor.h"
#endif /* SERIALCOMMANDPROCESSOR_H_ */

#ifndef ISERIALCOMMANDS_H_
#include "ISerialCommands.h"
#endif /* ISERIALCOMMANDS_H_ */

#include <stdlib.h>
#include <errno.h>

SerialCommandProcessor::SerialCommandProcessor(ISerialCommands &aCommands) {
  m_cmdActor = &aCommands;
}

/*******************************************************************
 * A micro terminal implementation to handle serial port inputs
 *
 * It enables the serial terminal to do some basic decoder actions
 * like:
 * reset
 * address <cvLo> <cvHi> <write: long adress>
 * cv <cv> <write: cv value>
 *
 * The implementation is chosen to be implemented with low-level C
 * to minimize RAM usage, but still written with readability in mind.
 *******************************************************************/
bool SerialCommandProcessor::process() {
  if (hasCommand()) {
    String cmd;
    cmd.reserve(COMMANDLEN);
    bool result = false;

    Serial.print('>');
    Serial.println(m_cmdLine);

    if (nextToken(cmd)) {
     if (cmd == "reset") {
        m_cmdActor->cmdReset();
        m_cmdActor->cmdList();
        result = true;
      } else if (cmd == "listcv") {
        m_cmdActor->cmdList();
        result = true;
      } else if (cmd == "cv") {
        uint16_t cv;
        if (nextToken(cv)) {
          Serial.println(m_cmdActor->cmdCVGet(cv));
          result = true;
        }
      } else if (cmd == "setcv") {
        uint16_t cv, val;
        if (nextToken(cv) && nextToken(val)) {
          m_cmdActor->cmdCVSet(cv, (uint8_t)val);
          Serial.println(m_cmdActor->cmdCVGet(cv));
          result = true;
        }
      } else if (cmd == "addr") {
        uint16_t hi, lo;
        if (nextToken(hi) && nextToken(lo)) {
          Serial.println(m_cmdActor->cmdAddressGet(hi, lo));
        } else {
          Serial.println(m_cmdActor->cmdAddressGet());
        }
        result = true;
      } else if (cmd == "setaddr") {
        uint16_t hi, lo, val;
        if (nextToken(hi)) {
          if (nextToken(lo) && nextToken(val)) {
            m_cmdActor->cmdAddressSet((uint8_t)val, hi, lo);
            Serial.println(m_cmdActor->cmdAddressGet(hi, lo));
          } else {
            m_cmdActor->cmdAddressSet((uint8_t)hi);
            Serial.println(m_cmdActor->cmdAddressGet());
          }
          result = true;
        }
      } else if (cmd == "debug") {
        bool state;
        if (nextToken(state)) {
          m_cmdActor->cmdDebug(state);
          Serial.println("OK");
          result = true;
        }
      } else {
        Serial.println("Available commands:");
        Serial.println(" reset");
        Serial.println(" listcv");
        Serial.println(" debug on|off");
        Serial.println(" cv CV");
        Serial.println(" setcv CV VAL");
        Serial.println(" addr");
        Serial.println(" addr HI LO");
        Serial.println(" setaddr VAL");
        Serial.println(" setaddr HI LO VAL");
      }
    }
    if (!result) {
      Serial.println("Fail.");
    }
    resetCmdLine();
    return result;
  }

  return true;
}

bool SerialCommandProcessor::nextToken(String &aCmd) {
  char *token;

  // skip empty tokens
  while (((token = strsep(&m_pos, " ")) != NULL) && (*token==' '));

  if (token != NULL) {
    aCmd = token;
    aCmd.toLowerCase();
    return true;
  } else {
    aCmd = "";
    return false;
  }
}

bool SerialCommandProcessor::nextToken(uint16_t &aValue) {
  String val;

  if (nextToken(val)) {
    const char *csVal = val.c_str();
    if ((val.length() > 2) && (csVal[0] == '0') && (csVal[1] == 'x')) {
      csVal += 2;
      aValue = (uint16_t)strtol(csVal, (char **)NULL, 16);
      if (errno == 0)
        return true;
    } else {
      aValue = (uint16_t)strtol(csVal, (char **)NULL, 10);
      if (errno == 0)
        return true;
    }
  }

  return false;
}

bool SerialCommandProcessor::nextToken(bool &aValue) {
  String val;

  if (nextToken(val)) {
    if ((val == "on") || (val == "true")) {
      aValue = true;
      return true;
    }
    if ((val == "off") || (val == "false")) {
      aValue = false;
      return true;
    }
  }

  return false;
}

bool SerialCommandProcessor::hasCommand() {
  int cmdLen = strlen(m_cmdLine);

  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      // newLine executes command
      m_cmdLine[cmdLen] = '\0';
      m_pos = m_cmdLine;
      return true;
      // execute
    } else if (cmdLen < COMMANDLEN) {
      m_cmdLine[cmdLen] = inChar;
      cmdLen++;
    }
    // additional characters are skipped
  }

  m_cmdLine[cmdLen] = '\0';
  return false;
}
