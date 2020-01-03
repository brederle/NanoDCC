#ifndef DCC_H_
#include "Dcc.h"
#endif /* DCC_H_ */


/***************************************************************
 * Pre-instantiate the one single DCC receiver and wire
 * Timer1 capture mode/ICP1 to it
 ***************************************************************/
DccReceiver Dcc;

ISR(TIMER1_CAPT_vect) {  
  Dcc.isrReceive();
} /* end timer interrupt INT0 */

/***************************************************************
 * Setup of INT0 pin for clock measured interrupts based
 * on timer 1B. Start first reception cycle
 ***************************************************************/
void DccReceiver::setup() {
  pinMode(PB0, INPUT_PULLUP); /* active low input with pullup */
  //DDRB &= ~(1 << PB0); /* configure input */
  //PORTB |= (1 << PB0); /* active low */

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  bitSet(TCCR1B, ICNC1);   // use noise canceller
  // bitClear(TCCR1B, ICNC1); // no noise canceller
  bitClear(TIFR1, TOV1);   // clear overflow
  
  bitClear(TCCR1B, ICES1); // start with falling edge
  bitSet(TIFR1, ICF1);     // required to enable new edge
  m_numHalfBits = 0;       // init half-bit detection
 }
