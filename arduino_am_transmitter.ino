/*This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.*/

// OCR2 values for available Transmit frequencies: OCR2A/B = 8000/Ft[Khz] - 1
#define F_8MHZ 0
#define F_4MHZ 1
#define F_2666 2
#define F_2000 3
#define F_1600 4
#define F_1333 5
#define F_1143 6
#define F_1000 7
#define F_880 8
#define F_800 9
#define F_727 10
#define F_666 11
#define F_615 12
#define F_571 13
#define F_533 14
#define F_500 15

#define FRQ F_1600 //Replace OCR2 value for frequency here, if necessary

#define ANTENNA_1 PD3 //Arduino Nano/Uno Pin D3
#define ANTENNA_2 PB3 //Arduino Nano/Uno Pin D11


void setup() {
  //Timer 2: Carrier Frequency generation
  TCCR2A = bit(COM2A0) + bit(COM2B0); // Toggle OC0A (Pin 11) and OC0B (Pin 3) on Compare Match
  bitSet(TCCR2A, WGM21); // CTC Mode
  TCCR2B = bit(CS20); // No clock Prescaling
  OCR2A = FRQ;
  OCR2B = FRQ;
  bitSet(TCCR2B, FOC2B);  // Force Output Compare B; Toggle inverted
  
  //Timer 1: PWM
  pinMode(9, OUTPUT);
  TCCR1A =  bit(WGM11) + bit(WGM10); //Fast PWM, 10 Bit
  TCCR1B = bit(WGM12) + bit(CS10); // No Prescaling
  TIMSK1 = bit(OCIE1A) + bit(TOIE1);// Output Compare A Match and overflow Interrupt Enable
  //TCCR1A += bit(COM1A0) + bit(COM1A1); //Toggle Pin 9 on compare match

  //ADC Settings
  ADMUX = bit(REFS1) + bit(REFS0); //Reference internal 1.1V
  ADCSRA = bit(ADEN) + bit(ADSC) + bit(ADATE); //Auto Trigger enable, free running
  ADCSRA += bit(ADPS2) + bit(ADPS1); //Divide by 64 -> 18.5k Sample rate
  DIDR0 = bit(ADC0D);
}

/* Overflow Interrupt*/
ISR(TIMER1_OVF_vect) {
  uint16_t adc = ADC;
  OCR1A = adc;
  bitSet(DDRD, ANTENNA_1); // timer 2 outputs on
  bitSet(DDRB, ANTENNA_2);
}

/* Output Compare A Match Interrupt */
ISR(TIMER1_COMPA_vect) {
  bitClear(DDRD, ANTENNA_1); // timer 2 outputs off
  bitClear(DDRB, ANTENNA_2);
}

void loop() 
{
}
