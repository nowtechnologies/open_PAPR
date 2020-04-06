#include <avr/io.h>
#include <util/delay.h>
#include "../application/bspInterface.h"
#include "../application/applicationInterface.h"

namespace {
  constexpr uint32_t cAdcChannelCount = ((uint32_t)  4);
  constexpr uint32_t cAdcSampleCount = ((uint32_t)  64);
  constexpr uint32_t cAdcBufferSize =   ((uint32_t)  cAdcSampleCount * cAdcChannelCount);
  constexpr uint32_t cIndexAdcBattery =  0u;
  constexpr uint32_t cIndexAdcPotmeter = 1u;
  constexpr uint32_t cIndexAdcTemp =     2u;
  constexpr uint32_t cIndexAdcVref =     3u;
  
  volatile uint16_t adcBuffer[cAdcBufferSize];
  volatile uint32_t adcData[cAdcChannelCount];
  
  /// Calculating ADC oversample average
  void calculateAndCopyAdcData(uint32_t const start, uint32_t const end) {
    for (unsigned int i = start; i < end; i++) {
      adcData[i % cAdcChannelCount] += adcBuffer[i];
      adcData[i % cAdcChannelCount] /= 2;
    }
  }
  
}

/// Callbacks


extern "C" void applicationSetup() {
  // Calibrate ADC

  // Start ADC IT

  // PWM Timer setup

}

// System
uint32_t papr::getTick() {
  return 0;
}

void papr::delayMs(uint32_t const aDelayMs) {
  //_delay_ms(aDelayMs);
}

// Power LED
void papr::turnOnPowerLed() {
  
}

void papr::turnOffPowerLed() {
  
}

void papr::togglePowerLed() {
  
}

bool papr::isPowerLedOn() {
  return false;
}

// Mode LED
void papr::turnOnModeLed() {
  
}

void papr::turnOffModeLed() {
  
}

void papr::toggleModeLed() {
  
}

bool papr::isModeLedOn() {
  return false;
}

// Buzzer
void papr::turnOnBuzzer() {
  
}

void papr::turnOffBuzzer() {
  
}

bool papr::isBuzzerOn() {
  return false;
}

// ADC
uint32_t papr::getPotmeterValue() {
  return adcData[cIndexAdcPotmeter];
}

uint32_t papr::getBatteryValue() {
  return adcData[cIndexAdcBattery];
}

// Motor
void papr::setMotorPwm(uint32_t const aMotorPwm) {
  
}