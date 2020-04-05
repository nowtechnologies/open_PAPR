#pragma once

#include <stdint.h>

namespace papr {

  constexpr uint32_t cPwmMax = 0xFFF;
  constexpr uint32_t cAdcMax = 0xFFF;  

  uint32_t map(uint32_t aValue, uint32_t aInMin, uint32_t aInMax, uint32_t aOutMin, uint32_t aOutMax) {
    if ((aInMax - aInMin) > (aOutMax - aOutMin)) {
      return (aValue - aInMin) * (aOutMax - aOutMin + 1) / (aInMax - aInMin + 1) + aOutMin;
    }
    else {
      return (aValue - aInMin) * (aOutMax - aOutMin) / (aInMax - aInMin) + aOutMin;
    }
  }
 
}