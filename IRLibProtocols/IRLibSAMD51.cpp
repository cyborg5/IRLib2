/* IRLibSAMD51.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This type of content is normally stored in IRLibHardware.h but we have 
 * moved at her because the SAMD51 support is significantly different than the
 * AVR 8-bit hardware support. Separating it out into a separate file
 * will make it easier to include comments and to maintain the code.
 */

/* See IRLibSAMD51.h for details about this implementation.
 */
#if defined (__SAMD51__)
#include "IRLibHardware.h"


//Timer used for PWM. Is initialized in initializeSAMD51PWM(...)
Tcc* IR_TCCx;
Tc* IR_PWM_TCx;
uint8_t IR_Use_TC;
uint8_t IR_kHz;

#include "wiring_private.h"

void IRLibStartPWM51(void) {
  if(IR_Use_TC) {
    PinDescription pinDesc = g_APinDescription[IR_SEND_PWM_PIN];
    uint8_t tcChannel = GetTCChannelNumber(pinDesc.ulPWMChannel);
    // -- Configure TC
    Tc* IR_PWM_TCx = (Tc*) GetTC(pinDesc.ulPWMChannel);
    //reset
    IR_PWM_TCx->COUNT8.CTRLA.bit.SWRST = 1;
    while (IR_PWM_TCx->COUNT8.SYNCBUSY.bit.SWRST);
    // Disable TCx
    IR_PWM_TCx->COUNT8.CTRLA.bit.ENABLE = 0;
    while (IR_PWM_TCx->COUNT8.SYNCBUSY.bit.ENABLE);
    // Set Timer counter Mode to 8 bits, normal PWM, prescaler 1/16
    IR_PWM_TCx->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 | TC_CTRLA_PRESCALER_DIV16;// |
   //       TC_CTRLA_PRESCSYNC_PRESC;
    IR_PWM_TCx->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_NPWM;
    while (IR_PWM_TCx->COUNT8.SYNCBUSY.bit.CC0);
    // Set the initial value
    // Each timer counts up to a maximum or TOP value set by the PER register,
    // this determines the frequency of the PWM operation.
    uint32_t cc = 120000000UL/16/(IR_kHz*1000) - 1;
    // The CCx register value corresponds to the pulsewidth in microseconds (us) 
    // Set the duty cycle of the PWM on TCx to 33%
    IR_PWM_TCx->COUNT8.CC[tcChannel].reg = (uint8_t)cc/3;//(uint8_t) cc/3;
    while (IR_PWM_TCx->COUNT8.SYNCBUSY.bit.CC0);
    IR_PWM_TCx->COUNT8.PER.reg = cc;
    while (IR_PWM_TCx->COUNT8.SYNCBUSY.bit.PER);
    IR_PWM_TCx->COUNT8.CTRLA.bit.ENABLE = 1;      //temporarily disable, will enable later
    while (IR_PWM_TCx->COUNT8.SYNCBUSY.bit.ENABLE);
  } else {
    IR_TCCx->CTRLA.bit.ENABLE = 1;
    while (IR_TCCx->SYNCBUSY.bit.ENABLE);
  }
};

void IRLibStopPWM51(void) {
  if(IR_Use_TC) {
    analogWrite(IR_SEND_PWM_PIN, 0); 
  } else {
    IR_TCCx->CTRLA.bit.ENABLE = 0;
    while (IR_TCCx->SYNCBUSY.bit.ENABLE);
  }
};
void initializeSAMD51PWM(uint16_t khz) {
  IR_kHz=khz;
  PinDescription pinDesc = g_APinDescription[IR_SEND_PWM_PIN];
  uint32_t attr = pinDesc.ulPinAttribute;
  //If PWM unsupported then do nothing and exit
  if( !(attr & (PIN_ATTR_PWM_E|PIN_ATTR_PWM_F|PIN_ATTR_PWM_G))){
    return;
  }
  uint32_t tcNum = GetTCNumber(pinDesc.ulPWMChannel);
  uint8_t tcChannel = GetTCChannelNumber(pinDesc.ulPWMChannel);

  if(attr & PIN_ATTR_PWM_E)
    pinPeripheral(IR_SEND_PWM_PIN, PIO_TIMER);
  else if(attr & PIN_ATTR_PWM_F)
    pinPeripheral(IR_SEND_PWM_PIN, PIO_TIMER_ALT);
  else if(attr & PIN_ATTR_PWM_G) 
    pinPeripheral(IR_SEND_PWM_PIN, PIO_TCC_PDEC);

  GCLK->PCHCTRL[GCLK_CLKCTRL_IDs[tcNum]].reg = 
    GCLK_PCHCTRL_GEN_GCLK0_Val | (1 << GCLK_PCHCTRL_CHEN_Pos); //use clock generator 0
  if (tcNum >= TCC_INST_NUM) {
    IR_Use_TC= true;
    analogWrite(IR_SEND_PWM_PIN,0);//Not sure if this is necessary but it doesn't hurt
  } else {
    IR_Use_TC= false;
    // Normal (single slope) PWM operation: timers countinuously count up to PER 
    // register value and then is reset to 0
    //Configure TCC
    IR_TCCx = (Tcc*) GetTC(pinDesc.ulPWMChannel);
    //Reset
    IR_TCCx->CTRLA.bit.SWRST = 1;
    while (IR_TCCx->SYNCBUSY.bit.SWRST);
    // Disable TCCx
    IR_TCCx->CTRLA.bit.ENABLE = 0;
    while (IR_TCCx->SYNCBUSY.bit.ENABLE);
    // Sent pre-scaler to 1
    IR_TCCx->CTRLA.reg = TCC_CTRLA_PRESCALER_DIV1 | TCC_CTRLA_PRESCSYNC_GCLK;
    //Set TCCx as normal PWM
    IR_TCCx->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;
    while (IR_TCCx->SYNCBUSY.bit.WAVE);
    while (IR_TCCx->SYNCBUSY.bit.CC0 || IR_TCCx->SYNCBUSY.bit.CC1);

    // Each timer counts up to a maximum or TOP value set by the PER register,
    // this determines the frequency of the PWM operation.
    uint32_t cc = 120000000UL/(khz*1000) - 1;
    // The CCx register value corresponds to the pulsewidth in microseconds (us) 
    // Set the duty cycle of the PWM on TCCx to 33%
    IR_TCCx->CC[tcChannel].reg = (uint32_t) cc/3;      
    while (IR_TCCx->SYNCBUSY.bit.CC0 || IR_TCCx->SYNCBUSY.bit.CC1);

    IR_TCCx->PER.reg = cc;      // Set the frequency of the PWM on IR_TCCx
    while(IR_TCCx->SYNCBUSY.bit.PER);

    IR_TCCx->CTRLA.bit.ENABLE = 0;            //initially off will turn on later
    while (IR_TCCx->SYNCBUSY.bit.ENABLE);
  };
}

/*
* Setup the 50 microsecond timer hardware interrupt for the IRrecv class.
*/
void initializeSAMD51timerInterrupt(void) {
  GCLK->PCHCTRL[IR_TCx_ID].reg = GCLK_PCHCTRL_GEN_GCLK0  | GCLK_PCHCTRL_CHEN;
  while (GCLK->SYNCBUSY.bit.GENCTRL0);
  IR_TCx->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (IR_TCx->COUNT16.SYNCBUSY.reg);
  IR_TCx->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (IR_TCx->COUNT16.CTRLA.bit.SWRST);
  IR_TCx->COUNT16.CTRLA.bit.SWRST |= TC_CTRLA_MODE_COUNT16;
  IR_TCx->COUNT16.WAVE.reg = TC_WAVE_WAVEGEN_MPWM;
  IR_TCx->COUNT16.CC[0].reg = 120000000/(1000000/USEC_PER_TICK) - 1;
  while (IR_TCx->COUNT16.SYNCBUSY.reg);
}
#endif
