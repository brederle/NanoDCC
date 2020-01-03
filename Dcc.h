#ifndef DCC_H_
#define DCC_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef DCC_MESSAGE_H_
#include "DccMessage.h"
#endif /*DCC_MESSAGE_H_*/

#define SYNCPHASE 0
#define DATAPHASE 1

// CLKDIV 8 to keep high measurement accuracy
// 16 Mhz Arduino, 1tick = 0.5us
// thus, every measured cycle limit is multiplied by 2
// Another factor 2 is multiplied to measure full cycles
// specification: minimum 52us per half-cycle for decoders,
// 47 looks more stable
#define CYCLE_ONEMIN (47 * 4)
// specification: maximum 64us per half-cycle,
// 67 look more stable for edge detection
#define CYCLE_ONEMAX (67 * 4)
// specification > 90us
#define CYCLE_ZEROMIN (90 * 4)

#define BIT_ZERO 0
#define BIT_ONE 1

#define DETECT_HALFBITS 3

class DccReceiver {
private:
  //volatile uint16_t m_start;  // last edge time
  //volatile uint16_t m_cycles; // last measured cycle
  //volatile int8_t m_phase;
  //volatile uint8_t m_numBits, m_numHalfBits;
  // this is the incomplete package
  //volatile uint8_t m_packet0, m_packet1, m_packet2, m_packet3, m_packet4,
  //    m_packet5;

  //DccMessage m_received;

  uint16_t m_start;  // last edge time
  uint16_t m_cycles; // last measured cycle
  int8_t m_phase;
  uint8_t m_numBits, m_numHalfBits;
  // this is the incomplete package
  uint8_t m_packet0, m_packet1, m_packet2, m_packet3, m_packet4,
      m_packet5;

  DccMessage m_received;

  void nextPacket() __attribute__((always_inline)) {
    // m_received.clear();
    m_numBits = 0;
    m_phase = SYNCPHASE;
  }

public:
  void setup();

  // Start (clean) receiving of a fresh message
  void startRecv() __attribute__((always_inline)) {
    nextPacket();

    TCNT1 = 0;
    m_start = 0;
    bitSet(TIMSK1, ICIE1); // enable ICU interrupt
  };

  // Stop receiving messages, all signals are ignored until restart
  void stopRecv() __attribute__((always_inline)) {
    bitClear(TIMSK1, ICIE1); // disable ICU interrupt
  };

  /*****************************************************************
   * A helper function to output some of the measured DCC cycles to
   * serial output. This method is used to debug timing problems
   * with the DCC signal cycle measurement in main loop.
   *****************************************************************/
  void debugCycles() {
    static uint16_t old = 0;

    if (m_cycles != old) {
      Serial.print(" ");
      Serial.print(m_cycles);
      Serial.print("-");
      Serial.print(m_numBits);
      if (m_phase==DATAPHASE) {
        Serial.print("d");
      }
      old = m_cycles;
    }
  }

  // Check whether a new package has arrived
  bool hasMessage() __attribute__((always_inline)) {
    return (m_received.size() > 0);
  }

  // Deliver (explicitly) a copy of the current received message
  // Make sure the receiver is stopped. Otherwise, you get
  // a message under construction, which may be incosistent.
  void popMessage(DccMessage &aMsg) __attribute__((always_inline)) {
    aMsg.copy(m_received);
    m_received.clear();
  }

  /***************************************************************
   * Wrong edge detection (which detects half-bits).

   * To relax measurement timing, we measure only between
   * two edges in the same direction. Bit if we decide for
   * the wrong edge, we get "0-1" or "1-0" halfbits with about
   * 140us cycle times, detected as invalid cycle times.
   *
   * ----_________-    or -________----
   * |-- ~140us --|       |-- ~140us --|
   *
   * If we measure such a value "frequently", we invert
   * the detection edge once to try to improve situation.
   ***************************************************************/
private:
  void probeEdgeInversion() __attribute__((always_inline)) {
    if (m_numHalfBits < DETECT_HALFBITS) {
      m_numHalfBits++;
    } else if (m_numHalfBits == DETECT_HALFBITS) {
      Serial.println("Inverted edge mode.");
      // only "one time" try to change edge direction
      bitSet(TCCR1B, ICES1); // start with falling edge
      m_numHalfBits++;
    }
  }

  /***************************************************************
   * Detect start of data phase (by receiving enough 1s)
   ***************************************************************/
private:
  void checkSync(uint8_t bit) __attribute__((always_inline)) {
    if (bit == BIT_ONE) {
      m_numBits++;
    } else {
      if (m_numBits > 9) {
        // accept at least 10 preamble bits
        // if enough 1 bits received, switch to data phase
        m_numBits = 0;
        m_packet0 = 0;
        m_phase = DATAPHASE;
      } else {
        // something went wrong, so restart sync phase detection
        nextPacket();
      }
    }
  }

  /*******************************************************************
   * Helper function that encapsulates the bit filling of packets
   *
   * It is optimized for minimal number of requires instructions and
   * computations in the ISR routine to store the bit in a proper
   * place
   *******************************************************************/
private:
  void addDataBit(uint8_t bit) __attribute__((always_inline)) {
    uint8_t numBits = m_numBits;
    m_numBits++;

    if (numBits < 27) {
      /* division optimization by duplicated code and decision */
      if (numBits < 17) {
        if (numBits < 8) {
          m_packet0 <<= 1;
          m_packet0 |= bit;
        } else if (numBits == 8) {
          /* Zero expected */
          if (bit == 0) {
            m_packet1 = 0;
          } else {
            // packet too short, so restart
            m_phase = SYNCPHASE;
            m_numBits = 0;
          }
        } else { // [9..16]
          m_packet1 <<= 1;
          m_packet1 |= bit;
        }
      } else if (numBits == 17) {
        /* Zero expected */
        if (bit == 0) {
          m_packet2 = 0;
        } else {
          // packet too short, so restart
          nextPacket();
        }
      } else if (numBits < 26) {
        m_packet2 <<= 1;
        m_packet2 |= bit;
      } else { // == 26
        /* one marks packet end */
        if (bit == 0) {
          m_packet3 = 0;
        } else {
          nextPacket();
          m_received.saveMessage(m_packet0, m_packet1, m_packet2);
        }
      }
    } else if (numBits < 44) {
      if (numBits < 35) {
        m_packet3 <<= 1;
        m_packet3 |= bit;
      } else if (numBits == 35) {
        /* one marks packet end */
        if (bit == 0) {
          m_packet4 = 0;
        } else {
          nextPacket();
          m_received.saveMessage(m_packet0, m_packet1, m_packet2, m_packet3);
        }
      } else {
        m_packet4 <<= 1;
        m_packet4 |= bit;
      }
    } else {               /* >= 44 */
      if (numBits == 44) { /* one marks packet end */
        if (bit == 0) {
          m_packet5 = 0;
        } else {
          nextPacket();
          m_received.saveMessage(m_packet0, m_packet1, m_packet2, m_packet3,
                                 m_packet4);
        }
      } else if (numBits < 53) {
        m_packet5 <<= 1;
        m_packet5 |= bit;
      } else {
        /* one marks packet end */
        if ((bit == 1) && (numBits == 53)) {
          nextPacket();
          m_received.saveMessage(m_packet0, m_packet1, m_packet2, m_packet3,
                                 m_packet4, m_packet5);
          // DEBUG: Serial.println(m_received.toString());
        } else {
          // there should not be a larger message, so something went wrong
          // try to detect a completely new message
          nextPacket();
        }
      } /* end 44 part */
    }   /* end 27 part */
  }

  /***************************************************************
   * Interrupt service routine for fast and unbiased bit/packet
   * reading from DCC signal stream.
   *
   * The ISR routine for interrupt (on pin change) has to
   * be public, although it should be never called outside this class
   * context
   ***************************************************************/
public:
  void isrReceive() __attribute__((always_inline)) {
    uint16_t clock = ICR1; // copy first

    if ((TIFR1 & (1 << TOV1)) == 0) { // no overflow
      m_cycles = clock - m_start;
    } else {                       // overflown
      bitClear(TIFR1, TOV1);       // clear overflow flag
      m_cycles = ~m_start + clock; // same as 0xffff - dccr->raising + clock
    }
    m_start = clock;

    uint8_t v = 2;
    if (m_cycles > CYCLE_ONEMIN) {
      if (m_cycles < CYCLE_ONEMAX) {
        v = BIT_ONE;
      } else if (m_cycles > CYCLE_ZEROMIN) {
        v = BIT_ZERO;
      } else {
        probeEdgeInversion();
      }
    } 
    // else let´s hope it´s only a spike
    //else {
    //  Serial.print("s");
    //}

    if (v < 2) {
      if (m_phase == DATAPHASE) { /* DATAPHASE */
        addDataBit(v);
      } else { /* SYNCPHASE, check each interrupt to sync reception */
        checkSync(v);
      }
    }
  }
};

extern DccReceiver Dcc;

#endif /* DCC_H_*/
