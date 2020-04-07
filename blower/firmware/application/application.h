#pragma once

#include <stdint.h>
#include "bspInterface.h"
#include "applicationInterface.h"

namespace papr {
  
  constexpr bool cBatteryProtectionEnabled = false;
  
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
    
    /// Updating the counter with fresh value and checking if just hit lower or upper limit.
    /// return True if any of the counters reach zero or upper limit.
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
      if (!cBatteryProtectionEnabled) {
        mBatteryLevel = BatteryLevel::cBatteryOk;
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
    
    inline bool notLow() const {
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