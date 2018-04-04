/* IRLibSAMD21.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This type of content is normally stored in IRLibHardware.h but we have 
 * moved at her because the SAMD21 support is significantly different than the
 * AVR 8-bit hardware support. Separating it out into a separate file
 * will make it easier to include comments and to maintain the code.
 */

/* See IRLibSAMD21.h for details about this implementation.
 */
#if defined (__SAMD21G18A__) || (__SAMD21E18A__)
#include "IRLibHardware.h"

//Save some typing
#define IR_APD g_APinDescription[IR_SEND_PWM_PIN]
#define syncGCLK while (GCLK->STATUS.bit.SYNCBUSY)

//Timer used for PWM. Is initialized in initializeSAMD21PWM(...)
Tcc* IR_TCCx;

/* Types used for the table below */

//Cannot use g_APinDescription to determine PWM channel because we insist on only
//using only TCC0 or TCC1. Therefore we create our own lookup table
typedef struct _IR_PWM_Map
{
  ETCChannel IR_TCC_Channel;
  uint8_t IR_PER_EorF;
} IR_PWM_Map_t;

IR_PWM_Map_t IR_PWM_Map[64]=
{
  //PORTA 0-31
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PA00
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PA01
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PA02
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PA03
  {TCC0_CH0,PORT_PMUX_PMUXE_E}, //PA04	works
  {TCC0_CH1,PORT_PMUX_PMUXO_E}, //PA05	works
  {TCC1_CH0,PORT_PMUX_PMUXE_E}, //PA06	works
  {TCC1_CH1,PORT_PMUX_PMUXO_E}, //PA07	works
  
  {TCC0_CH0,PORT_PMUX_PMUXE_E}, //PA08	works
  {TCC0_CH1,PORT_PMUX_PMUXO_E}, //PA09	works
  {TCC0_CH2,PORT_PMUX_PMUXE_F}, //PA10	works
  {TCC0_CH3,PORT_PMUX_PMUXO_F}, //PA11	works
  {TCC0_CH6,PORT_PMUX_PMUXE_F}, //PA12	works
  {TCC0_CH7,PORT_PMUX_PMUXO_F}, //PA13	untested
  {TCC0_CH4,PORT_PMUX_PMUXE_F}, //PA14	works
  {TCC0_CH5,PORT_PMUX_PMUXO_F}, //PA15	works
  
  {TCC0_CH6,PORT_PMUX_PMUXE_F}, //PA16	works
  {TCC0_CH7,PORT_PMUX_PMUXO_F}, //PA17	works
  {TCC0_CH2,PORT_PMUX_PMUXE_F}, //PA18	works
  {TCC0_CH3,PORT_PMUX_PMUXO_F}, //PA19	works
  {TCC0_CH6,PORT_PMUX_PMUXE_F}, //PA20	works
  {TCC0_CH7,PORT_PMUX_PMUXO_F}, //PA21	works
  {TCC0_CH4,PORT_PMUX_PMUXE_F}, //PA22	works
  {TCC0_CH5,PORT_PMUX_PMUXO_F}, //PA23	works
  
  {TCC1_CH2,PORT_PMUX_PMUXE_F}, //PA24	untested
  {TCC1_CH3,PORT_PMUX_PMUXO_F}, //PA25	untested
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PA26
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PA27
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PA28
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PA29
  {TCC1_CH0,PORT_PMUX_PMUXE_E}, //PA30	untested
  {TCC1_CH1,PORT_PMUX_PMUXO_E}, //PA31	untested
  
   //PORTB 0-31
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB00
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB01
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB02
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB03
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB04
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB05
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB06
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB07
  
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB08
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB09
  {TCC0_CH4,PORT_PMUX_PMUXE_F}, //PB10	works
  {TCC0_CH5,PORT_PMUX_PMUXO_F}, //PB11	works
  {TCC0_CH6,PORT_PMUX_PMUXE_F}, //PB12	untested
  {TCC0_CH7,PORT_PMUX_PMUXO_F}, //PB13	untested
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB14
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB15
  
  {TCC0_CH4,PORT_PMUX_PMUXE_F}, //PB16	untested
  {TCC0_CH5,PORT_PMUX_PMUXO_F}, //PB17	untested
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB18
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB19
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB20
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB21
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB22
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB23
  
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB24
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB25
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB26
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB27
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB28
  {NOT_ON_TIMER,NOT_ON_TIMER},  //PB29
  {TCC0_CH0,PORT_PMUX_PMUXE_E}, //PB30	untested
  {TCC0_CH1,PORT_PMUX_PMUXO_E}  //PB31	untested
};

void initializeSAMD21PWM(uint16_t khz) {
  uint8_t port=IR_APD.ulPort;
  uint8_t pin=IR_APD.ulPin;
  uint8_t IR_mapIndex=port*32 + pin;
  //If PWM unsupported then do nothing and exit
  if(IR_PWM_Map[IR_mapIndex].IR_TCC_Channel == NOT_ON_TIMER) {
	  return;
  }
  //println();Serial.print("Port:"); Serial.print(port,DEC); Serial.print(" Pin:"); Serial.println(pin,DEC);
  // Enable the port multiplexer for the PWM channel on pin   
  PORT->Group[port].PINCFG[pin].bit.PMUXEN = 1;
  
  // Connect the TCC timer to the port outputs - port pins are paired odd PMUXO and even PMUXEII 
  // F & E peripherals specify the timers: TCC0, TCC1 and TCC2
  PORT->Group[port].PMUX[pin >> 1].reg |= IR_PWM_Map[IR_mapIndex].IR_PER_EorF; 
  
//  pinPeripheral (IR_SEND_PWM_PIN,PIO_TIMER_ALT);
  // Feed GCLK0 to TCC0 and TCC1
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |       // Enable GCLK0 to TCC0 and TCC1
                     GCLK_CLKCTRL_GEN_GCLK0 |   // Select GCLK0
                     GCLK_CLKCTRL_ID_TCC0_TCC1; // Feed GCLK0 to TCC0 and TCC1
  syncGCLK;                                     // Wait for synchronization

  // Normal (single slope) PWM operation: timers countinuously count up to PER 
  // register value and then is reset to 0
  IR_TCCx = (Tcc*) GetTC(IR_PWM_Map[IR_mapIndex].IR_TCC_Channel);
  IR_TCCx->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;   // Setup single slope PWM on TCCx
  while (IR_TCCx->SYNCBUSY.bit.WAVE);           // Wait for synchronization

  // Each timer counts up to a maximum or TOP value set by the PER register,
  // this determines the frequency of the PWM operation.
  uint32_t cc = F_CPU/(khz*1000) - 1;
  IR_TCCx->PER.reg = cc;      // Set the frequency of the PWM on IR_TCCx
  while(IR_TCCx->SYNCBUSY.bit.PER);

  // The CCx register value corresponds to the pulsewidth in microseconds (us) 
  // Set the duty cycle of the PWM on TCC0 to 33%
  IR_TCCx->CC[GetTCChannelNumber(IR_PWM_Map[IR_mapIndex].IR_TCC_Channel)].reg = cc/3;      
  while (IR_TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
  //while(IR_TCCx->SYNCBUSY.bit.CC3);

  // Enable IR_TCCx timer but do not turn on PWM yet. Will turn it on later.
  IR_TCCx->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV1;     // Divide GCLK0 by 1
  while (IR_TCCx->SYNCBUSY.bit.ENABLE);
  IR_TCCx->CTRLA.reg &= ~TCC_CTRLA_ENABLE;            //initially off will turn on later
  while (IR_TCCx->SYNCBUSY.bit.ENABLE);
}
/*
 * Setup the 50 microsecond timer hardware interrupt for the IRrecv class.
 */
void initializeSAMD21timerInterrupt(void) {
  GCLK->CLKCTRL.reg = (uint16_t)(GCLK_CLKCTRL_CLKEN | 
                      GCLK_CLKCTRL_GEN_GCLK0 | 
                      GCLK_CLKCTRL_ID(IR_GCM));
  syncGCLK;
  IR_TCx->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  syncTC;
  IR_TCx->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (IR_TCx->COUNT16.CTRLA.bit.SWRST);
  IR_TCx->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  IR_TCx->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  IR_TCx->COUNT16.CC[0].reg = F_CPU/(1000000/USEC_PER_TICK) - 1;
  syncTC;
}
#endif
