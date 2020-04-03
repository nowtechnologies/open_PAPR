#include "common.h"
#include "applciation.h"

namespace {
  constexpr uint32_t cBatteryOverLimitSampleCount =   1000u; // 1000 = ~12 sec
  constexpr uint32_t cModeLedOnTimeMs =               50u;  // Blink ON time
  constexpr uint32_t cBuzzerOnTimeMs =                100u; // Beep ON time
  constexpr uint32_t cBuzzerOnTimeWhenBatteryDeadMs = 1000u; // Long beep when battery level goes below minimum
  constexpr uint32_t cWarningBuzzerIntervalMs =       10000u; // Battery warning beep interval
  constexpr uint32_t cOffBuzzerIntervalMs =           2000u; // Battery warning beep interval
  constexpr uint32_t cPwmLowLimit =                   papr::cPwmMax / 40; // Minimum PWM
  constexpr uint32_t cAdcPotmeterLowLimit =           papr::cAdcMax / 20;
  constexpr uint32_t cBatteryMinOffRawValue =         2794u; // 16.00 V, cut off limit
  constexpr uint32_t cBatteryWarningRawValue =        2968u; // 17.00 V, warning limit
  constexpr uint32_t cBatteryChargedRawValue =        3666u; // 21.00 V
  constexpr bool cRestartAfterBatteryLow =            false; // Restart if battery level goes back above cut off limit?
  
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
  uint32_t buzzerInterval = cWarningBuzzerIntervalMs;
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
    
    // Check ADC
    if (isAdcReady()) {
      batteryRawValue = getBatteryValue();
      potmeterRawValue = getPotmeterValue();
      if (batteryLevel.update(batteryRawValue)) { // State changed
        turnOnBuzzer();
        delay(50u);
        turnOffBuzzer();
      }
    }
    
    if (potmeterRawValue < cAdcPotmeterLowLimit) {
      pwm = 0;
    }
    else {
      pwm = papr::map(potmeterRawValue, cAdcPotmeterLowLimit, papr::cAdcMax, cPwmLowLimit, papr::cPwmMax);
    }
    
    pwm = batteryLevel.notLow() ? pwm : 0u;
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
    if (batteryLevel == papr::BatteryLevel::cBatteryLow && now - powerLedTimestamp > powerLedIntervalMs)
    {
      togglePowerLed();
      powerLedTimestamp = now;
    }
    
    if (now - buzzerTimestamp > buzzerInterval) {
      if (batteryLevel == papr::BatteryLevel::cBatteryOk) {
        turnOffBuzzer();
      }
      else if (batteryLevel == papr::BatteryLevel::cBatteryWarning) {
        
      }
      else if (batteryLevel == papr::BatteryLevel::cBatteryWarning) {
        
      }
      else {
        //Doin' nothin'
      }
    }
    
    delay(10u);
  }
}