#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdint>
#include "common.h"
  
// System

/// return Elapsed time in ms since power on.
uint32_t getTick();

/// Waiting for the given time period before returning.
/// param aDelayMs 
void delay(uint32_t const aDelayMs);

// Power LED
void turnOnPowerLed();
void turnOffPowerLed();
void togglePowerLed();
bool isPowerLedOn();

// Mode LED
void turnOnModeLed();
void turnOffModeLed();
bool isModeLedOn();

// Buzzer
void turnOnBuzzer();
void turnOffBuzzer();
bool isBuzzerOn();

// ADC
uint32_t getPotmeterValue();
uint32_t getBatteryValue();

// Motor
void setMotorPwm(uint32_t const aMotorPwm);
  
#ifdef __cplusplus
}
#endif