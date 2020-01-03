#ifndef MOTOR4ACTOR_H_
#define MOTOR4ACTOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define TIMEOUT_MS_FACTOR 100

class Motor4Actor {
private:
  unsigned long m_timeout0, m_timeout1, m_timeout2, m_timeout3;

public:
  Motor4Actor();
  void setup();
  void processTimeouts();

  /*****************************************************************
   * Start motor 0 forward
   * PWM = OC2A/PB3, driver1 port1A = PB2, driver1 port 2A = PB1
   * Automatically disable motor on timeout if called before
   * motor start, in ~100ms steps. 0 is handled as no timeout
   *****************************************************************/
  void forward0(uint8_t speed, uint8_t timeout_hs = 5) __attribute__((always_inline)) {
    // brake first
    digitalWrite(PB1, 0);
    digitalWrite(PB2, 0);
    OCR2A = speed; // 6 bits of DCC optimally used with inv PWM
    digitalWrite(PB2, 1);
    bitClear(TCCR0A, COM2A0);
    bitSet(TCCR0A, COM2A1);
    m_timeout0 = millis() + timeout_hs * TIMEOUT_MS_FACTOR;
  }
  /*****************************************************************
   * Start motor 0 backward
   * PWM = OC2A/PB3, driver1 port 1A = PB2, driver1 port 2A = PB1
   * Automatically disable motor on timeout if called before
   * motor start, in ~100ms steps. 0 is handled as no timeout
   *****************************************************************/
  void backward0(uint8_t speed, uint8_t timeout_hs = 5) __attribute__((always_inline)) {
    // brake first
    digitalWrite(PB1, 0);
    digitalWrite(PB2, 0);
    OCR2A = speed;
    digitalWrite(PB1, 1);
    bitClear(TCCR0A, COM2A0);
    bitSet(TCCR0A, COM2A1);
    m_timeout0 = millis() + timeout_hs * TIMEOUT_MS_FACTOR;
  }
  /*****************************************************************
   * Stop motor 0 by braking and disabling
   * PWM = OC2A/PB3, driver1 port 1A = PB2, driver1 port 2A = PB1
   *****************************************************************/
  void stop0(void) __attribute__((always_inline)) {
    // brake
    PORTB &= ~(bit(PB1) | bit(PB2));
    // and stop
    TCCR2A &= ~(bit(COM2A0) | bit(COM2A1));
    m_timeout0 = 0;
  }

  /*****************************************************************
   * Start motor 1 forward
   * PWM = OC0A/PD6, driver1 port 3A = PB4, driver1 port 4A = PB5
   * Automatically disable motor on timeout if called before
   * motor start, in ~100ms steps. 0 is handled as no timeout
   *****************************************************************/
  void forward1(uint8_t speed, uint8_t timeout_hs = 5) __attribute__((always_inline)) {
    // brake first
    digitalWrite(PB4, 0);
    digitalWrite(PB5, 0);
    OCR0A = speed; // 6 bits of DCC optimally used with inv PWM
    digitalWrite(PB4, 1);
    bitClear(TCCR0A, COM0A0);
    bitSet(TCCR0A, COM0A1);
    m_timeout1 = millis() + timeout_hs * TIMEOUT_MS_FACTOR;
  }
  /*****************************************************************
   * Start motor 1 backward
   * PWM = OC0A/PD6, driver1 port 3A = PB4, driver1 port 4A = PB5
   * Automatically disable motor on timeout if called before
   * motor start, in ~100ms steps. 0 is handled as no timeout
   *****************************************************************/
  void backward1(uint8_t speed, uint8_t timeout_hs = 5) __attribute__((always_inline)) {
    // brake first
    digitalWrite(PB4, 0);
    digitalWrite(PB5, 0);
    OCR0A = speed;
    digitalWrite(PB5, 1);
    bitClear(TCCR0A, COM0A0);
    bitSet(TCCR0A, COM0A1);
    m_timeout1 = millis() + timeout_hs * TIMEOUT_MS_FACTOR;
  }
  /*****************************************************************
   * Stop motor 1 by braking and disabling
   * PWM = OC0A/PD6, driver1 port 3A = PB4, driver1 port 4A = PB5
   *****************************************************************/
  void stop1(void) __attribute__((always_inline)) {
    // brake
    PORTB &= ~(bit(PB4) | bit(PB5));
    // and stop

    m_timeout1 = 0;
  }

  /*****************************************************************
   * Start motor 2 forward
   * PWM = OC2B/PD3, driver2 port 1A = PC2, driver2 port 2A = PC3
   * Automatically disable motor on timeout if called before
   * motor start, in ~100ms steps. 0 is handled as no timeout
   *****************************************************************/
  void forward2(uint8_t speed, uint8_t timeout_hs = 5) __attribute__((always_inline)) {
    // brake first
    digitalWrite(PC2, 0);
    digitalWrite(PC3, 0);
    OCR2B = speed;
    digitalWrite(PC2, 1);
    bitClear(TCCR0A, COM2B0);
    bitSet(TCCR0A, COM2B1);
    m_timeout2 = millis() + timeout_hs * TIMEOUT_MS_FACTOR;
  }
  /*****************************************************************
   * Start motor 2 backward
   * PWM = OC2B/PD3, driver2 port 1A = PC2, driver2 port 2A = PC3
   * Automatically disable motor on timeout if called before
   * motor start, in ~100ms steps. 0 is handled as no timeout
   *****************************************************************/
  void backward2(uint8_t speed, uint8_t timeout_hs = 5) __attribute__((always_inline)) {
    // brake first
    digitalWrite(PC2, 0);
    digitalWrite(PC3, 0);
    OCR2B = speed;
    digitalWrite(PC3, 1);
    bitClear(TCCR0A, COM2B0);
    bitSet(TCCR0A, COM2B1);
    m_timeout2 = millis() + timeout_hs * TIMEOUT_MS_FACTOR;
  }
  /*****************************************************************
   * Stop motor 2 by braking and disabling
   * PWM = OC2B/PD3, driver2 port 1A = PC2, driver2 port 2A = PC3
   *****************************************************************/
  void stop2(void) __attribute__((always_inline)) {
    // brake
    PORTC &= ~(bit(PC2) | bit(PC3));
    // and stop
    TCCR2A &= ~(bit(COM2B0) | bit(COM2B1));
    m_timeout2 = 0;
  }

  /*****************************************************************
   * Start motor 3 forward
   * PWM = OC0B/PD5, driver2 port 3A = PC5 driver2 port 4A = PC4
   * Automatically disable motor on timeout if called before
   * motor start, in ~100ms steps. 0 is handled as no timeout
   *****************************************************************/
  void forward3(uint8_t speed, uint8_t timeout_hs = 5) __attribute__((always_inline)) {
    // brake first
    digitalWrite(PC5, 0);
    digitalWrite(PC4, 0);
    OCR0B = speed; // 6 bits of DCC optimally used with inv PWM
    digitalWrite(PC5, 1);
    bitClear(TCCR0A, COM0B0);
    bitSet(TCCR0A, COM0B1);
    m_timeout3 = millis() + timeout_hs * TIMEOUT_MS_FACTOR;
  }
  /*****************************************************************
   * Start motor 3 backward
   * PWM = OC0B/PD5, driver2 port 3A = PC5 driver2 port 4A = PC4
   * Automatically disable motor on timeout if called before
   * motor start, in ~100ms steps. 0 is handled as no timeout
   *****************************************************************/
  void backward3(uint8_t speed, uint8_t timeout_hs = 5) __attribute__((always_inline)) {
    // brake first
    digitalWrite(PC5, 0);
    digitalWrite(PC4, 0);
    OCR0B = speed;
    digitalWrite(PC4, 1);
    bitClear(TCCR0A, COM0B0);
    bitSet(TCCR0A, COM0B1);
    m_timeout3 = millis() + timeout_hs * TIMEOUT_MS_FACTOR;
  }
  /*****************************************************************
   * Stop motor 3 by brake and disabling
   * PWM = OC0B/PD5, driver2 port 3A = PC5 driver2 port 4A = PC4
   *****************************************************************/
  void stop3(void) __attribute__((always_inline)) {
    // brake
    PORTC &= ~(bit(PC5) | bit(PC4));
    // and stop
    TCCR0A &= ~(bit(COM0B0) | bit(COM0B1));
    m_timeout2 = 0;
  }

private:
  void Motor4Actor::setupCounter0();
  void Motor4Actor::setupCounter2();
};

#endif /* _MOTOR4ACTOR_H_ */
