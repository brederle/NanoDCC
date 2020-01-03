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

  Serial.println("PacketDec V" + VERSION + " ready.");
  Dcc.startRecv();
}

void loop() {
  // put your main code here, to run repeatedly:
  DccMessage msg;

  if (Dcc.hasMessage()) {
      Dcc.popMessage(msg);
      if (msg.isGA() ) {
        msg.serialPrint();
        Serial.println();
      }
  }
}