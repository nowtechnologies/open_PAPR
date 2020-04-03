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
extern bool isAdcReady();
extern uint32_t getPotmeterValue();
extern uint32_t getBatteryValue();
// Motor
extern void setMotorPwm(uint32_t const aMotorPwm);

namespace papr {
  
  enum class BatteryLevel {
    cBatteryOk = 0u,
    cBatteryWarning = 1u,
    cBatteryLow = 2u
  };
  
  class Counter {
    
  public:
    
    bool increment() {
      uint32_t oldValue = mCounter;
      mCounter = mCounter < mMaximum ? ++mCounter : mMaximum;
      return mCounter == mMaximum && mCounter != oldValue;
    }
    
    bool decrement() {
      uint32_t oldValue = mCounter;
      mCounter = mCounter > 0 ? --mCounter : 0;
      return mCounter == 0 && mCounter != oldValue;
    }
    
    void reset() {
      mCounter = 0;
    }
    
    inline bool isLimitReached() const {
      return mCounter >= mMaximum;
    }
    
    inline bool isZero() const {
      return mCounter == 0;
    }
    
    void setMaximum(uint32_t const aMaximum) {
      mMaximum = aMaximum;
    }
    
  private:
    
    uint32_t mMaximum = 0;
    uint32_t mCounter = 0;
    
  };
  
  class BatteryLevelCounter {
    
  public:
    
    BatteryLevelCounter(uint32_t const aLowerLimit, uint32_t const aUpperLimit, uint32_t const aMaxCount)
      : cLowerLimit(aLowerLimit)
      , cUpperLimit(aUpperLimit) {
      for (auto i = 0; i < rangeCount; i++) {
        ranges[i].setMaximum(aMaxCount);
      }
    }
    
    inline bool operator==(const BatteryLevel &aBatteryLevel) const {
      return mBatteryLevel == aBatteryLevel;
    }
    
    bool update(uint32_t const aValue) {
      bool changed = false;
      if (aValue < cLowerLimit) {
        if (ranges[static_cast<unsigned int>(BatteryLevel::cBatteryLow)].increment()) {
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryWarning)].reset();
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryOk)].reset();
          changed = true;
          mBatteryLevel = BatteryLevel::cBatteryLow;
        }
        else {
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryWarning)].decrement();
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryOk)].decrement();
        }
      }
      else if (aValue >= cLowerLimit && aValue < cUpperLimit) {
        if (ranges[static_cast<unsigned int>(BatteryLevel::cBatteryWarning)].increment()) {
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryLow)].reset();
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryOk)].reset();
          changed = true;
          mBatteryLevel = BatteryLevel::cBatteryWarning;
        }
        else {
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryLow)].decrement();
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryOk)].decrement();
        }
      }
      else if (aValue >= cUpperLimit) {
        if (ranges[static_cast<unsigned int>(BatteryLevel::cBatteryOk)].increment()) {
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryLow)].reset();
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryWarning)].reset();
          changed = true;
          mBatteryLevel = BatteryLevel::cBatteryOk;
        }
        else {
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryLow)].decrement();
          ranges[static_cast<unsigned int>(BatteryLevel::cBatteryWarning)].decrement();
        }
      }
      else {
        // Doin' Nothin'
      }
      return changed;
    }
    
    void reset() {
      for (auto i = 0; i < rangeCount; i++) {
        ranges[i].reset();
      }
    }
    
    BatteryLevel getBatteryLevel() const {
      return mBatteryLevel;
    }
    
    bool notLow() const {
      return mBatteryLevel < papr::BatteryLevel::cBatteryLow;
    }
    
  private:
    
    const uint32_t cLowerLimit;
    const uint32_t cUpperLimit;
    static constexpr unsigned int rangeCount = 3u;
    Counter ranges[rangeCount];
    BatteryLevel mBatteryLevel = BatteryLevel::cBatteryOk;
    
  };
  
}