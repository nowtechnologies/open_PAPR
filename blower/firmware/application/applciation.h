#pragma once

#include <cstdint>

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