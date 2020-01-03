#ifndef DCC_H_
#include "Dcc.h"
#endif // DCC_H_

#define VERSION "0.8.2"

void setup() {
  // use high baud rate to not unintendedly
  // blocking receiver
  Serial.begin(115200);

  // put your setup code here, to run once:
  Dcc.setup();

  Serial.println("TimingDebug V" + VERSION + " ready.");
  Dcc.startRecv();
}

// simple timing debug loop
void loop() { Dcc.debugCycles(); }
