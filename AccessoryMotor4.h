#ifndef ACCESSORYMOTOR4_H_
#define ACCESSORYMOTOR4_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#ifndef DCC_MESSAGE_H_
#include "DccMessage.h"
#endif /*DCC_MESSAGE_H_*/

#ifndef ACCESSORY_H_
#include "Accessory.h"
#endif  /* ACCESSORY_H_ */

#ifndef MOTOR4ACTOR_H_
#include "Motor4Actor.h"
#endif /* MOTOR4ACTOR_H_ */


class AccessoryMotor4 : public Accessory {
public:
    virtual void resetCV();
    virtual void listCV(uint16_t aCV);
    virtual bool getCV(uint16_t aCV, uint8_t& aReadValue);
    virtual bool setCV(uint16_t aCV, uint8_t aWriteValue);

    virtual void setup();
    virtual void process(DccMessage &aMsg);

protected:
    Motor4Actor m_motors;

    // Methods to overwrite
    virtual void handle(DccMessage& aMsg);
};

#endif /* ACCESSORY_H_ */
