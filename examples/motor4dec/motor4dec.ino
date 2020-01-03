#ifndef DCC_H_
#include "Dcc.h"
#endif // DCC_H_

#ifndef ACCESSORYMOTOR4_H_
#include "AccessoryMotor4.h"
#endif /* ACCESSORYMOTOR4_H_ */

#ifndef SERIALCOMMANDPROCESSOR_H_
#include "SerialCommandProcessor.h"
#endif /* SERIALCOMMANDPROCESSOR_H_ */

const String VERSION = String("0.8.3");

AccessoryMotor4 motor4;
SerialCommandProcessor commands(motor4);

void setup() {
  // use high baud rate to not unintendedly
  // blocking receiver
  Serial.begin(115200);

  // put your setup code here, to run once:
  Dcc.setup();
  motor4.setup();

  Serial.println("NanoDCC Motor4 Accessory V" + VERSION + " ready.");
  Dcc.startRecv();
}

void loop() {
  DccMessage msg;
  if (Dcc.hasMessage()) {
      Dcc.popMessage(msg);
      motor4.process(msg);
  }
  
  commands.process();
}
