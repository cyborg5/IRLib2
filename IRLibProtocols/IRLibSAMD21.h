/* IRLibSAMD21.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This type of content is normally stored in IRLibHardware.h but we have 
 * moved at her because the SAMD21 support is significantly different than the
 * AVR 8-bit hardware support. Separating it out into a separate file
 * will make it easier to include comments and to maintain the code.
 */

/* This file provides hardware support for the SAMD21G18A processor or as we 
 * will refer to it simply as SAMD 21. This processor is used in the Arduino Zero, 
 * Arduino M0, Arduino M0 Pro, and the Adafruit Feather M0 series boards as well as
 * the upcoming Adafruit Circuit Playground Express.
 * Most of the code has been adapted from messages on the Arduino.cc support forum 
 * and from the code at:
 *	https://github.com/manitou48/ZERO/tree/master/IRtest
 * Receiving is supported through all three receiver classes. As you would expect 
 * the IRrecvLoop class can be used with any available input pin. The IRrecvPCI and 
 * IRfrequency classes can be used on any pin that supports "attachInterrupt()". 
 * Specifically that means everything except pin 4 on the Arduino Zero. And everything 
 * except pin 2 on the boards from Arduino.org such as the Arduino M0, M0 Pro, and 
 * Zero Pro. For details on supported pins see these links:
 *	https://www.arduino.cc/en/Reference/AttachInterrupt
 *	http://www.arduino.org/learning/reference/AttachInterrupt
 * Note that although we support Arduino.org hardware, you should only use the 
 * Arduino.cc IDE for compiling your sketches.
 * The IRrecv receiver class using a 50 microsecond timer interrupt is also supported. 
 * It uses hardware timer TC3. The IRrecv and IRrecvLoop classes should be able to use
 * any available input pin. Note that all of our example sketches used pin 2 for 
 * receiver and pin 3 for frequency measurement. However pin 2 is not available for PCI 
 * interrupts on the Arduino.org platforms and neither 2 nor 3 are available on the 
 * Adafruit Feather M0 platforms. We are recommending using 5 and 6 for receiving and
 * frequency measurement respectively.
 * but For sending, output is on pin 9 and uses timer TCC1 to set the PWM frequency. It
 * has been tested at frequencies of 36, 38, 39, 40 and 57 which are the frequencies 
 * of our supported protocols.
 * If anyone who is more knowledgeable about the SAMD 21 platform and can adapt the code 
 * other timers or other output pins, pull requests to make this code more flexible are 
 * welcome. Please try to maintain the general structure of the code and limit your 
 * changes to adding new timers, pin numbers etc. without making other changes to the code.
 * As of this release the code has been tested on Arduino Zero, Arduino M0 Pro, and 
 * Adafruit Feather M0 BLE version.
 */
#ifndef IRLibSAMD21_h
#define IRLibSAMD21_h

// Choose the timers and clocks we will use. WARNING: Most of the timer and
// clock and pin number settings are hardcoded but we are setting this up for the future
// when we can adapt the code to be more parameterized.
#define IR_TCC  TCC1
#define IR_TC   TC3
#define IR_GCLK 4
#define IR_SAMD21_PWM_PIN 9
#define IR_APD g_APinDescription[IR_SAMD21_PWM_PIN]

// Saves us a lot of typing when synchronizing
#define syncTC   while (IR_TC->COUNT16.STATUS.bit.SYNCBUSY)
#define syncGCLK while (GCLK->STATUS.bit.SYNCBUSY)

// Turns PWM on and off after already set up
/*** Hardcoded to TCC1. Need to parameterize. ****/
#define IR_SEND_PWM_START {REG_TCC1_CTRLA |= TCC_CTRLA_ENABLE;\
  while (TCC1->SYNCBUSY.bit.ENABLE);}
#define IR_SEND_MARK_TIME(time)  IRLibDelayUSecs(time)
#define IR_SEND_PWM_STOP {REG_TCC1_CTRLA &= ~TCC_CTRLA_ENABLE;\
  while (TCC1->SYNCBUSY.bit.ENABLE);}
#define IR_SEND_PWM_PIN	IR_SAMD21_PWM_PIN
#define IR_SEND_CONFIG_KHZ(val) initializeSAMD21PWM(val);

/* These are the definitions for setting up the 50 microseconds
 * timer interrupt for the IRrecv class. 
 */
#define IR_RECV_ENABLE_INTR ({NVIC_EnableIRQ(TC3_IRQn);\
    REG_TC3_INTENSET = TC_INTENSET_OVF;\
    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; syncTC;})
#define IR_RECV_DISABLE_INTR  REG_TC3_INTENCLR = TC_INTENCLR_OVF;  
#define IR_RECV_INTR_NAME void TC3_Handler()
#define IR_RECV_CONFIG_TICKS() initializeSAMD21timerInterrupt()
  
#define BLINKLED       13
#define BLINKLED_ON()  (digitalWrite(BLINKLED, HIGH))
#define BLINKLED_OFF() (digitalWrite(BLINKLED, LOW))

//prototypes
void initializeSAMD21PWM(uint16_t khz);
void initializeSAMD21timerInterrupt(void);

#endif //IRLibSAMD21_h
