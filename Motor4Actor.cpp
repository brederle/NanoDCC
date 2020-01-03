#ifndef MOTOR4ACTOR_H_
#include "Motor4Actor.h"
#endif /* MOTOR4ACTOR_H_ */

Motor4Actor::Motor4Actor() {
  m_timeout0 = 0;
  m_timeout1 = 0;
  m_timeout2 = 0;
  m_timeout3 = 0;
}

void Motor4Actor::setupCounter0() {

  // Arduino default clock prescaler  for timer0 f_io/64
  // we can reuse this for out PWM setup
  // TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);

  /* fast PWM normal mode, count always to MAX */
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  TCCR0B &= ~(1 << WGM02); /* rest of fast PWM parameters */

  // initially stopped pwm
  TCCR0A &= ~(bit(COM0A0) | bit(COM0A1));
  TCCR0A &= ~(bit(COM0B0) | bit(COM0B1));
}

void Motor4Actor::setupCounter2() {
  // Arduino default clock prescaler for timer2 f_io/64
  // we can reuse this for out PWM setup
  // TCCR2B |= (0 << CS22) | (1 << CS21) | (1 << CS20);

  /* fast PWM normal mode, count always to MAX */
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2B &= ~(1 << WGM22); /* rest of fast PWM parameters */

  // initially stopped pwm
  TCCR2A &= ~(bit(COM2A0) | bit(COM2A1));
  TCCR2A &= ~(bit(COM2B0) | bit(COM2B1));
}

void Motor4Actor::setup() {
  /* set needed pins to output */
  pinMode(PB5, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(PB3, OUTPUT);
  pinMode(PB2, OUTPUT);
  pinMode(PB1, OUTPUT);
  pinMode(PC5, OUTPUT);
  pinMode(PC4, OUTPUT);
  pinMode(PC3, OUTPUT);
  pinMode(PC2, OUTPUT);
  pinMode(PD6, OUTPUT);
  pinMode(PD5, OUTPUT);
  pinMode(PD3, OUTPUT);
  setupCounter0();
  setupCounter2();
}

void Motor4Actor::processTimeouts() {
  unsigned long now = millis();
  
  if ((m_timeout0 > 0) && (m_timeout0 < now)) {
    stop0();
  }
  if ((m_timeout1 > 0) && (m_timeout1 < now)) {
    stop1();
  }
  if ((m_timeout2 > 0) && (m_timeout2 < now)) {
    stop2();
  }
  if ((m_timeout3 > 0) && (m_timeout3 < now)) {
    stop3();
  }
}