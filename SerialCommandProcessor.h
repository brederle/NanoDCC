#ifndef SERIALCOMMANDPROCESSOR_H_
#define SERIALCOMMANDPROCESSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define COMMANDLEN 64

class ISerialCommands;

class SerialCommandProcessor {
private:
  ISerialCommands *m_cmdActor = NULL;

  char m_cmdLine[COMMANDLEN + 1] = "";
  char* m_pos;

public:
  SerialCommandProcessor(ISerialCommands &aCommands);

public:
  bool process();

protected:
  void resetCmdLine() __attribute__((always_inline)) {
    m_cmdLine[0] = '\0';
  };

  bool hasCommand();
  bool nextToken(String &aCmd);
  bool nextToken(uint16_t &aValue);
  bool nextToken(bool &aValue);
};

#endif /* SERIALCOMMANDPROCESSOR_H_ */
