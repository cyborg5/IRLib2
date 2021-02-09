/* IRLibSAMD51.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This type of content is normally stored in IRLibHardware.h but we have 
 * moved at her because the SAMD51 support is significantly different than the
 * AVR 8-bit hardware support. Separating it out into a separate file
 * will make it easier to include comments and to maintain the code.
 */
#ifndef IRLibSAMD51_h
#define IRLibSAMD51_h
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
#if defined (ADAFRUIT_METRO_M4_EXPRESS) 
  //Settings for Adafruit Metro M4 
  //Default is 9. Available 0-13
  #define IR_SEND_PWM_PIN 9
  #if (IR_SEND_PWM_PIN > 13)
    #error "Unsupported output pin on Adafruit Metro M4"
  #endif
#elif defined (ADAFRUIT_FEATHER_M4_EXPRESS)
  //Settings for Adafruit Feather M4 Express
  //Default is 9. Available 0,1,4-6,9-13,16-19(A2-A5), 21(SDA), 22(SCL)
  #define IR_SEND_PWM_PIN 9
  #if (! ( (IR_SEND_PWM_PIN==0) || (IR_SEND_PWM_PIN==1) || \
           ((IR_SEND_PWM_PIN>=4) && (IR_SEND_PWM_PIN<=6) ) || \
           ((IR_SEND_PWM_PIN>=9) && (IR_SEND_PWM_PIN<=13) ) || \
           ((IR_SEND_PWM_PIN>=16) && (IR_SEND_PWM_PIN<=19) ) || \
		   (IR_SEND_PWM_PIN==21) || (IR_SEND_PWM_PIN==22) ) )
     #error "Unsupported output pin Adafruit Feather M4 Express"
  #endif
#elif defined (ADAFRUIT_GRAND_CENTRAL_M4)
  //Settings for Adafruit Grand Central Metro M4. Default is 9.
  //Available 2-10, 12-14, 18-19, 23-28, 30-31, 38-39, 59-61(A13-A15), 69(A2)
  #define IR_SEND_PWM_PIN 9
  #if (! ( ((IR_SEND_PWM_PIN>=2) && (IR_SEND_PWM_PIN<=10) ) || \
           ((IR_SEND_PWM_PIN>=12) && (IR_SEND_PWM_PIN<=14) ) || \
		   (IR_SEND_PWM_PIN==18) || (IR_SEND_PWM_PIN==19) || \
           ((IR_SEND_PWM_PIN>=23) && (IR_SEND_PWM_PIN<=28) ) || \
		   (IR_SEND_PWM_PIN==30) || (IR_SEND_PWM_PIN==31) || \
		   (IR_SEND_PWM_PIN==38) || (IR_SEND_PWM_PIN==39) || \
           ((IR_SEND_PWM_PIN>=59) && (IR_SEND_PWM_PIN<=61) ) || (IR_SEND_PWM_PIN== 69)))
     #error "Unsupported output pin Adafruit Grand Central Metro M4"
  #endif
#else //Other generic SAMD 51 boards 
  //Default is 9.
  #define IR_SEND_PWM_PIN 9
#endif

/*
 * Everything below this point should not be changed. It computes needed defines
 * based on the user set values above.
 */


// Turns PWM on and off after already set up
#define IR_SEND_PWM_START IRLibStartPWM51();
#define IR_SEND_MARK_TIME(time)  IRLibDelayUSecs(time)
#define IR_SEND_PWM_STOP IRLibStopPWM51();
#define IR_SEND_CONFIG_KHZ(val) initializeSAMD51PWM(val);

/* These are the definitions for setting up the 50 microsecond
 * timer interrupt for the IRrecv class. 
 */
#define GCLK_TC3  26
#define GCLK_TC4  30
#define GCLK_TC5  30
#if (IR_TCn==3)
  #define IR_TCx      TC3
  #define IR_TCx_ID   GCLK_TC3
  #define IR_RECV_INTR_NAME void TC3_Handler()
  #define IR_IRQ   TC3_IRQn
#elif (IR_TCn==4)
  #define IR_TCx      TC4
  #define IR_TCx_ID   GCLK_TC4
  #define IR_RECV_INTR_NAME void TC4_Handler()
  #define IR_IRQ   TC4_IRQn
#elif (IR_TCn==5)
  #define IR_TCx      TC5
  #define IR_TCx_ID   GCLK_TC5
  #define IR_RECV_INTR_NAME void TC5_Handler()
  #define IR_IRQ   TC5_IRQn
#else
  #error "Invalid IR_TCn value"
#endif

#define IR_RECV_ENABLE_INTR ({NVIC_EnableIRQ(IR_IRQ);\
    IR_TCx->COUNT16.INTENSET.reg = TC_INTENSET_OVF;\
    IR_TCx->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; \
    while (IR_TCx->COUNT16.SYNCBUSY.reg);})
#define IR_RECV_DISABLE_INTR  IR_TCx->COUNT16.INTENCLR.reg = TC_INTENCLR_OVF;  
#define IR_RECV_CONFIG_TICKS() initializeSAMD51timerInterrupt()

//Clear interrupt
#define IR_CLEAR_INTERRUPT 	IR_TCx->COUNT16.INTFLAG.bit.MC0 = 1;

//prototypes
void initializeSAMD51PWM(uint16_t khz);
void initializeSAMD51timerInterrupt(void);
void IRLibStopPWM51(void);
void IRLibStartPWM51(void);
#endif //IRLibSAMD51_h
