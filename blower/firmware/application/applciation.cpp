#include "common.h"
#include "applciation.h"

namespace {
  
  constexpr uint32_t cModeLedOnTimeMs = 50u;
  constexpr uint32_t cPwmLowLimit = papr::cPwmMax / 40;
  constexpr uint32_t cAdcPotmeterLowLimit = papr::cAdcMax / 20;
  constexpr uint32_t cBatteryMinOffRawValue =      2794u; // 16.00 V
  constexpr uint32_t cBatteryMinOnRawValue =       2968u; // 17.00 V
  constexpr uint32_t cBatteryChargedRawValue =     3666u; // 21.00 V
  constexpr bool cRestartAfterBatteryLow = false;
}

extern "C" void loop() {
  uint32_t pwm = 0x0u;
  uint32_t powerLedTimestamp;
  uint32_t powerLedIntervalMs = 50u;
  uint32_t modeLedTimestamp;
  uint32_t modeLedIntervalMs = 100u;
  bool batteryOk = true;
  
  turnOnPowerLed();
  uint32_t counter = 100u;
  while(getBatteryValue() < cBatteryMinOffRawValue && --counter > 0) {
    delay(10u);
  }
  
  while(true) {
    if (getPotmeterValue() < cAdcPotmeterLowLimit) {
      pwm = 0;
    }
    else {
      pwm = papr::map(getPotmeterValue(), cAdcPotmeterLowLimit, papr::cAdcMax, cPwmLowLimit, papr::cPwmMax);
    }
    if (batteryOk && getBatteryValue() < cBatteryMinOffRawValue) {
      batteryOk = 0;
    }
    if (cRestartAfterBatteryLow && !batteryOk && getBatteryValue() > cBatteryMinOnRawValue) {
      batteryOk = 1;
      turnOnPowerLed();
    }
    pwm = batteryOk ? pwm : 0u;
    setMotorPwm(pwm);
    uint32_t now = getTick();
    if (pwm == 0) {
      turnOffModeLed();
    }
    else {
      if (now - modeLedTimestamp > modeLedIntervalMs) {
        if (!isModeLedOn()) {
          turnOnModeLed();
          modeLedIntervalMs = cModeLedOnTimeMs;
        }
        else {
          turnOffModeLed();
          modeLedIntervalMs = (0xFFF - pwm ) / 4;
        }
        modeLedTimestamp = now;
      }
    }
    if (!batteryOk && now - powerLedTimestamp > powerLedIntervalMs)
    {
      togglePowerLed();
      powerLedTimestamp = now;
    }
    delay(10u);
  }
}