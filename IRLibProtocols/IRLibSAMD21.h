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
 * Both sending and receiving use GCLK0 even though GCLK0-GCLK3 are generally reserved
 * the Arduino infrastructure. However we are using the default 48 MHz clock source
 * and a divisor of "1" so we aren't changing any of the GCLK0 set up. It's therefore
 * safe to use.
 * The IRrecv receiver class using a 50 microsecond timer interrupt is also supported. 
 * It defaults to using hardware timer TC3 however TC4 and TC5 can also be used in case 
 * of conflicts with other libraries. The IRrecv and IRrecvLoop classes should be able to use
 * any available input pin. Note that all of our example sketches used pin 2 for 
 * receiver and pin 3 for frequency measurement. However pin 2 is not available for PCI 
 * interrupts on the Arduino.org platforms and neither 2 nor 3 are available on the 
 * Adafruit Feather M0 platforms. We are recommending using 5 and 6 for receiving and
 * frequency measurement respectively.
 * For sending, you can use any pin that supports PWM on your particular 
 * platform. On Arduino Zero that means pins 3, 4, 5, 6, 8, 9, 10, 11, 12, or 13.
 * Additionally Arduino M0 Pro adds pins 2 and 7 to that list. In all cases the default
 * output pin is 9. The code automatically selects TCC0 or TCC1 based on pin number
 * as needed. It has been tested at frequencies of 36, 38, 39, 40 and 57 which
 * are the frequencies of our supported protocols.
 * As of this release the code has been tested on Arduino Zero, Arduino M0 Pro, and 
 * Adafruit Feather M0 BLE version.
 */
#ifndef IRLibSAMD21_h
#define IRLibSAMD21_h
/*
 * This section contains user changeable values. You can probably stick with the defaults
 * but if there are hardware conflicts with other libraries, you can change these values.
 */
//Choose which timer counter to use for the 50 microsecond interrupt
//Un-comment only one of these.
#define IR_TCn 3
//#define IR_TCn 4
//#define IR_TCn 5

//Each section below is the default for each particular board we support.
//Edit the #define IR_SEND_PWM_PIN value to one of the available pin numbers
//There are conditional compile statements after that definition which attempt
// to generate an error if you choose an unsupported pin number.
#if defined (ADAFRUIT_CIRCUITPLAYGROUND_M0)
  //Settings for Adafruit Circuit Playground Express
  //Built-in IR output is pin 25 which is the default.
  //Also available pins: 6/A1, 9/A2, 10/A3.
  #define IR_SEND_PWM_PIN 25
#elif defined(ADAFRUIT_TRINKET_M0)
  //Settings for Adafruit Trinket M0.
  //Default is 0. Only 0, 2, 3, or 4 can be used
  #define IR_SEND_PWM_PIN 0
  #if ((IR_SEND_PWM_PIN > 4) || (IR_SEND_PWM_PIN == 1))
    #error "Unsupported output pin on Adafruit Trinket M0"
  #endif
#elif defined(ADAFRUIT_GEMMA_M0)
  //Settings for Adafruit Gemma M0.
  //Default is 0. Only other option is 2.
  #define IR_SEND_PWM_PIN 0
  #if ((IR_SEND_PWM_PIN > 2) || (IR_SEND_PWM_PIN == 1))
    #error "Unsupported output pin on Adafruit Gemma M0"
  #endif
#elif defined (ARDUINO_SAMD_MKR1000)
  //Settings for Arduino MKR 1000.
  //Default is 6. Only 0-12 or 18-21 (Note: 18-21 is also A3-A6)
  #define IR_SEND_PWM_PIN 6
  #if ((IR_SEND_PWM_PIN > 21) || ( (IR_SEND_PWM_PIN > 12) && (IR_SEND_PWM_PIN < 18)) )
    #error "Unsupported output pin on Arduino MKR 1000." //issue 56 error?
  #endif
#elif (defined(ARDUINO_SAMD_FEATHER_M0) || defined(ARDUINO_SAMD_FEATHER_M0_EXPRESS) )
  //Settings for Adafruit Feather M0 or Adafruit Feather M0 Express
  //Default is 9. Available on all Feather M0 are 0,1,5,6,9-13,17/A3,18/A4
  //On the standard M0 20/SDA, 21/SCL, 22/MISO, 23/MOSI, 24/SCK
  //On the M0 Express  26/SDA, 27/SCL, 28/MISO, 29/MOSI, 30/SCK
  //Only the pin numbers are different so use the labels.
  #define IR_SEND_PWM_PIN 9
  #if (  ( (IR_SEND_PWM_PIN>1)  && (IR_SEND_PWM_PIN<5) )  \
	  || ( (IR_SEND_PWM_PIN>19) && (IR_SEND_PWM_PIN<PIN_WIRE_SDA) ) \
      || ( (IR_SEND_PWM_PIN>13) && (IR_SEND_PWM_PIN<17) ) \
      || (IR_SEND_PWM_PIN==7)   || (IR_SEND_PWM_PIN==8) \
      || (IR_SEND_PWM_PIN>PIN_SPI_SCK) ) 
	#error "Pin unsupported on Adafruit Feather M0"
  #endif
#elif defined(ADAFRUIT_ITSYBITSY_M0)
  //Settings for Adafruit Itsy-Bitsy M0
  //Default is 9. Available are 0-5, 7, 9-13, 17/A3, 18/A4,
  //  26/SDA, 27/SCL, 28/MSIO, 29/MOSI, 30/SCI
  #define IR_SEND_PWM_PIN 9
  #if ( (IR_SEND_PWM_PIN==6)   || (IR_SEND_PWM_PIN==8) \
      || ( (IR_SEND_PWM_PIN>13) && (IR_SEND_PWM_PIN<17) ) \
 	  || ( (IR_SEND_PWM_PIN>18) && (IR_SEND_PWM_PIN<26) ) \
      || (IR_SEND_PWM_PIN>30) ) 
	#error "Pin unsupported on Adafruit Itsy-Bitsy M0"
  #endif
#elif defined (ARDUINO_SAM_ZERO)
  //Settings for Arduino M0 Pro
  //Default is 9. Available pins 0-13, 27/A3, 28/A4, 16/SDA, 17/SCL, 
  //  18/MISO, 21/MOSI, 20/SCK 
  #define IR_SEND_PWM_PIN 9
  #if ( ( (IR_SEND_PWM_PIN > 21) && (IR_SEND_PWM_PIN < 27) ) \
       || (IR_SEND_PWM_PIN > 28) || (IR_SEND_PWM_PIN == 14)  \
	   || (IR_SEND_PWM_PIN == 15) )
    #error "Unsupported output pin on Arduino M0 Pro"
  #endif
#elif defined (ARDUINO_SAMD_ZERO) 
  //Settings for Arduino Zero 
  //Default is 9. Available 0-13, 17/A3, 18/A4, 20/SDA, 21/SCL, 22/MISO, 23/MOSI, 24/SCK 
  #define IR_SEND_PWM_PIN 9
  #if ( (IR_SEND_PWM_PIN > 24) \
   || ( (IR_SEND_PWM_PIN > 13) && (IR_SEND_PWM_PIN < 17)) \
   || (IR_SEND_PWM_PIN == 19) )
    #error "Unsupported output pin on Arduino Zero"
  #endif
#else //Other generic SAMD 21 boards 
  //Default is 9.
  #define IR_SEND_PWM_PIN 9
#endif

/*
 * Everything below this point should not be changed. It computes needed defines
 * based on the user set values above.
 */


//Timer used for PWM
extern Tcc* IR_TCCx;

//Save some typing
#define syncTC   while (IR_TCx->COUNT16.STATUS.bit.SYNCBUSY)

// Turns PWM on and off after already set up
#define IR_SEND_PWM_START {IR_TCCx->CTRLA.reg |= TCC_CTRLA_ENABLE;\
  while (IR_TCCx->SYNCBUSY.bit.ENABLE);}
#define IR_SEND_MARK_TIME(time)  IRLibDelayUSecs(time)
#define IR_SEND_PWM_STOP {IR_TCCx->CTRLA.reg &= ~TCC_CTRLA_ENABLE;\
  while (IR_TCCx->SYNCBUSY.bit.ENABLE);}
#define IR_SEND_CONFIG_KHZ(val) initializeSAMD21PWM(val);

/* These are the definitions for setting up the 50 microsecond
 * timer interrupt for the IRrecv class. 
 */
#if (IR_TCn==3)
  #define IR_TCx   TC3
  #define IR_GCM   GCM_TCC2_TC3
  #define IR_RECV_INTR_NAME void TC3_Handler()
  #define IR_IRQ   TC3_IRQn
#elif (IR_TCn==4)
  #define IR_TCx   TC4
  #define IR_GCM   GCM_TC4_TC5
  #define IR_RECV_INTR_NAME void TC4_Handler()
  #define IR_IRQ   TC4_IRQn
#elif (IR_TCn==5)
  #define IR_TCx   TC5
  #define IR_GCM   GCM_TC4_TC5
  #define IR_RECV_INTR_NAME void TC5_Handler()
  #define IR_IRQ   TC5_IRQn
#else
  #error "Invalid IR_TCn value"
#endif

#define IR_RECV_ENABLE_INTR ({NVIC_EnableIRQ(IR_IRQ);\
    IR_TCx->COUNT16.INTENSET.reg = TC_INTENSET_OVF;\
    IR_TCx->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; syncTC;})
#define IR_RECV_DISABLE_INTR  IR_TCx->COUNT16.INTENCLR.reg = TC_INTENCLR_OVF;  
#define IR_RECV_CONFIG_TICKS() initializeSAMD21timerInterrupt()

//Clear interrupt
#define IR_CLEAR_INTERRUPT 	IR_TCx->COUNT16.INTFLAG.bit.MC0 = 1;

//prototypes
void initializeSAMD21PWM(uint16_t khz);
void initializeSAMD21timerInterrupt(void);

#endif //IRLibSAMD21_h
