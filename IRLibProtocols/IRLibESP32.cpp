/* IRLibESP32.cpp
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This type of content is normally stored in IRLibHardware.h but we have 
 * moved at her because the ESP32 support is significantly different than the
 * AVR 8-bit hardware support. Separating it out into a separate file
 * will make it easier to include comments and to maintain the code.
 */

/* See IRLibESP32.h for details about this implementation.
 */
#if defined (ESP32)
#include "IRLibHardware.h"
#include "IRLibESP32.h"

hw_timer_t *esp32Timer;

void startESP32Timer() {
    timerAlarmEnable(esp32Timer);
}

void stopESP32Timer() {
    timerAlarmDisable(esp32Timer);
}

void initializeESP32TimerInterrupt() {
    // Configure timer 1 (0-3) with the prescaler/divider
    // so that every tick is one microsecond.
    esp32Timer = timerBegin(1, (SYSCLOCK / 1000000), true);
    // Attach timer to the interrupt function.
    timerAttachInterrupt(esp32Timer, &onESP32Timer, true);
    // Set the interval time of the timer.
    timerAlarmWrite(esp32Timer, USEC_PER_TICK, true);
}

#endif