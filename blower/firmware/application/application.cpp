#include "common.h"
#include "application.h"

namespace {
  constexpr uint32_t cBatteryOverLimitSampleCount =   2000u; // 1000 = ~12 sec
  constexpr uint32_t cModeLedOnTimeMs =               50u;  // Blink ON time
  constexpr uint32_t cBuzzerOnTimeMs =                100u; // Beep ON time
  constexpr uint32_t cBuzzerOnTimeWhenBatteryDeadMs = 1000u; // Long beep when battery level goes below minimum
  constexpr uint32_t cBuzzerWarningIntervalMs =       10000u; // Battery warning beep interval
  constexpr uint32_t cBuzzerLowIntervalMs =           200u; // Battery warning beep interval
  constexpr uint32_t cPwmLowLimit =                   papr::cPwmMax / 40; // Minimum PWM
  constexpr uint32_t cAdcPotmeterLowLimit =           papr::cAdcMax / 20;
  constexpr uint32_t cBatteryMinOffRawValue =         2794u; // 16.00 V, cut off limit
  constexpr uint32_t cBatteryWarningRawValue =        2968u; // 17.00 V, warning limit
  constexpr uint32_t cBatteryChargedRawValue =        3666u; // 21.00 V
  
  void startupBeep() {
    constexpr auto beeps = 2u;
    constexpr auto delayMs = 100u;
    constexpr auto beepMs = 40u;
    for (auto i = 0u; i < beeps; i++) {
      turnOnBuzzer();
      delay(beepMs);
      turnOffBuzzer();
      if (i < beeps - 1) {
        delay(delayMs);
      }
    }  
  }

}

extern "C" void applicationLoop() {
  uint32_t pwm = 0x0u;
  uint32_t powerLedTimestamp;
  uint32_t powerLedIntervalMs = 50u;
  uint32_t modeLedTimestamp;
  uint32_t modeLedIntervalMs = 100u;
  uint32_t buzzerTimestamp;
  uint32_t buzzerInterval = cBuzzerWarningIntervalMs;
  int32_t batteryWarningSampleCount = 0;
  int32_t batteryMinOffSampleCount = 0;
  uint32_t batteryRawValue;
  uint32_t potmeterRawValue;
  papr::BatteryLevelCounter batteryLevel(cBatteryMinOffRawValue, cBatteryWarningRawValue, cBatteryOverLimitSampleCount);
  
  turnOnPowerLed();
  uint32_t counter = 100u;
  while(getBatteryValue() < cBatteryMinOffRawValue && --counter > 0) {
    delay(10u);
  }
  
  startupBeep();
  
  while(true) {
    uint32_t now = getTick();
    
    // Check ADC
    if (isAdcReady()) {
      batteryRawValue = getBatteryValue();
      potmeterRawValue = getPotmeterValue();
      if (batteryLevel.update(batteryRawValue)) { // Battery state changed
        if (batteryLevel == papr::BatteryLevel::cBatteryLow) {
          turnOnBuzzer();
          delay(cBuzzerOnTimeWhenBatteryDeadMs);
          turnOffBuzzer();
          buzzerInterval = 0;
        }
        else if (batteryLevel == papr::BatteryLevel::cBatteryWarning) {
          turnOnPowerLed();
          turnOffBuzzer();
        }
        else if (batteryLevel == papr::BatteryLevel::cBatteryOk) {
          turnOnPowerLed();
          turnOffBuzzer();
        }
        else {
          // Doin' Nothin'
        }
      }
    }
    
    // Calculate PWM from potmeter
    if (potmeterRawValue < cAdcPotmeterLowLimit) {
      pwm = 0;
    }
    else {
      pwm = papr::map(potmeterRawValue, cAdcPotmeterLowLimit, papr::cAdcMax, cPwmLowLimit, papr::cPwmMax);
    }
    
    // Set PWM to 0 when battery is low
    pwm = batteryLevel.notLow() ? pwm : 0u;
    setMotorPwm(pwm);
    
    // Handle PWM related mode LED blinking
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
    // Handle power led blinking
    if (batteryLevel == papr::BatteryLevel::cBatteryLow && now - powerLedTimestamp > powerLedIntervalMs) {
      togglePowerLed();
      powerLedTimestamp = now;
    }
    
    // Handle buzzer beeps when warning or low
    if (now - buzzerTimestamp > buzzerInterval) {
      if (batteryLevel == papr::BatteryLevel::cBatteryOk) { // Battery OK, no beep
        turnOffBuzzer();
      }
      else if (batteryLevel == papr::BatteryLevel::cBatteryWarning) { // Warning beeps
        if (isBuzzerOn()) {
          buzzerInterval = cBuzzerWarningIntervalMs;
          turnOffBuzzer();
        }
        else {
          buzzerInterval = cBuzzerOnTimeMs;
          turnOnBuzzer();
        }
      }
      else if (batteryLevel == papr::BatteryLevel::cBatteryLow) { // Battery low beeps
        if (isBuzzerOn()) {
          buzzerInterval = cBuzzerLowIntervalMs;
          turnOffBuzzer();
        }
        else {
          buzzerInterval = cBuzzerOnTimeMs;
          turnOnBuzzer();
        }
      }
      else {
        //Doin' nothin'
      }
      buzzerTimestamp = now;
    }
    
    delay(10u);
  }
}