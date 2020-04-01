#pragma once

#include <cstdint>

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  if ((in_max - in_min) > (out_max - out_min)) {
    return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
  }
  else {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
}

// System
extern uint32_t getTick();
extern void delay(uint32_t const aDelayMs);
// Power LED
extern void turnOnPowerLed();
extern void turnOffPowerLed();
extern void togglePowerLed();
extern bool isPowerLedOn();
// Mode LED
extern void turnOnModeLed();
extern void turnOffModeLed();
extern bool isModeLedOn();
// Buzzer
extern void turnOnBuzzer();
extern void turnOffBuzzer();
extern bool isBuzzerOn();
// ADC
extern uint32_t getPotmeterValue();
extern uint32_t getBatteryValue();
// Motor
extern void setMotorPwm(uint32_t const aMotorPwm);