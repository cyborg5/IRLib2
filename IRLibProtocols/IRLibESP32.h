/* IRLibESP32.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This type of content is normally stored in IRLibHardware.h but we have 
 * moved at her because the ESP32 support is significantly different than the
 * AVR 8-bit hardware support. Separating it out into a separate file
 * will make it easier to include comments and to maintain the code.
 */

#ifndef IRLibESP32_h
#define IRLibESP32_h

// Output GPIO pin for IR LED. Any GPIO pin which can be output may be used.
#define IR_SEND_ESP32           26
    
#define IR_SEND_PWM_START       ledcAttachPin(IR_SEND_ESP32, 1);
#define IR_SEND_MARK_TIME(time) IRLibDelayUSecs(time)
#define IR_SEND_PWM_STOP        ledcDetachPin(IR_SEND_ESP32);
// Setup (LED) PWM channel 1 (0-15) with 8-bits resolution.
// Set the duty cycle to 50%.
#define IR_SEND_CONFIG_KHZ(val) ({ \
                                    ledcSetup(1, val * 1000, 8); \
                                    ledcWrite(1, 128); \
                                })
#define IR_SEND_PWM_PIN	        IR_SEND_ESP32

#define IR_RECV_ENABLE_INTR     startESP32Timer()
#define IR_RECV_DISABLE_INTR    stopESP32Timer()
#define IR_RECV_INTR_NAME       void IRAM_ATTR onESP32Timer()
#define IR_RECV_CONFIG_TICKS()  initializeESP32TimerInterrupt()

void IRAM_ATTR onESP32Timer(void);
void startESP32Timer(void);
void stopESP32Timer(void);
void initializeESP32TimerInterrupt(void);

#endif