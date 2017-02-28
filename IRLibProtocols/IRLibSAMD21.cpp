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
#if defined (__SAMD21G18A__)
#include "IRLibHardware.h"

void initializeSAMD21PWM(uint16_t khz) {
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(1) |      // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
                    GCLK_GENDIV_ID(IR_GCLK);  // Select Generic Clock (GCLK)
  syncGCLK;                                   // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |         // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |       // Enable GCLK
                     GCLK_GENCTRL_SRC_DFLL48M | // Set the 48MHz clock source
                     GCLK_GENCTRL_ID(IR_GCLK);  // Select GCLK
  syncGCLK;                                     // Wait for synchronization

  /**** Unsure if you can really change the pin to anything and if so does this code work? ****/
  // Enable the port multiplexer for the PWM channel on pin   
  PORT->Group[IR_APD.ulPort].PINCFG[IR_APD.ulPin].bit.PMUXEN = 1;
  // Connect the TCC timer to the port outputs - port pins are paired odd PMUO and even PMUXE
  // F & E peripherals specify the timers: TCC0, TCC1 and TCC2
  PORT->Group[IR_APD.ulPort].PMUX[IR_APD.ulPin >> 1].reg |= PORT_PMUX_PMUXO_E; 

  /**** This section has GCLK4 and TCC0/TCC1 hardcoded. Need to parameterize. ****/
  // Feed GCLK4 to TCC0 and TCC1
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |       // Enable GCLK4 to TCC0 and TCC1
                     GCLK_CLKCTRL_GEN_GCLK4 |   // Select GCLK4
                     GCLK_CLKCTRL_ID_TCC0_TCC1; // Feed GCLK4 to TCC0 and TCC1
  syncGCLK;                                     // Wait for synchronization

  /**** The remainder of this function is hardcoded to TCC1.  Need to parameterize. ****/
  // Normal (single slope) PWM operation: timers countinuously count up to PER 
  // register value and then is reset to 0
  REG_TCC1_WAVE |= TCC_WAVE_WAVEGEN_NPWM;   // Setup single slope PWM on TCC1
    while (TCC1->SYNCBUSY.bit.WAVE);                // Wait for synchronization

  // Each timer counts up to a maximum or TOP value set by the PER register,
  // this determines the frequency of the PWM operation.
  uint32_t cc = F_CPU/(khz*1000) - 1;
  REG_TCC1_PER = cc;      // Set the frequency of the PWM on TCC1
  while(TCC1->SYNCBUSY.bit.PER);

  // The CCBx register value corresponds to the pulsewidth in microseconds (us) 
  REG_TCC1_CC1 = cc/3;      // Set the duty cycle of the PWM on TCC0 to 33%
  while(TCC1->SYNCBUSY.bit.CC1);

  // Enable TCC1 timer but do not turn on PWM yet. Will turn it on later.
  REG_TCC1_CTRLA |= TCC_CTRLA_PRESCALER_DIV1;     // Divide GCLK4 by 1
  while (TCC1->SYNCBUSY.bit.ENABLE);
  REG_TCC1_CTRLA &= ~TCC_CTRLA_ENABLE;            //initially off will turn on later
  while (TCC1->SYNCBUSY.bit.ENABLE);
}
/*
 * Setup the 50 microseconds timer hardware interrupt for the IRrecv class.
 */
 
void initializeSAMD21timerInterrupt(void) {
  GCLK->CLKCTRL.reg = (uint16_t)(GCLK_CLKCTRL_CLKEN | 
                      GCLK_CLKCTRL_GEN_GCLK0 | 
                      GCLK_CLKCTRL_ID(GCM_TCC2_TC3));
  syncGCLK;
  IR_TC->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  syncTC;
  IR_TC->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (IR_TC->COUNT16.CTRLA.bit.SWRST);
  TC3->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  TC3->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  TC3->COUNT16.CC[0].reg = F_CPU/(1000000/USEC_PER_TICK) - 1;
  syncTC;
}
#endif
