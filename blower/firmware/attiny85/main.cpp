/*
 * papr_blower_attiny85.cpp
 *
 * Created: 2020. 04. 05. 12:27:12
 * Author : root
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "applicationInterface.h"

int main(void) {
  applicationSetup();
  applicationLoop();
  while (1) {
  }
}

