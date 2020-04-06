/*
 * papr_blower_attiny85.cpp
 *
 * Author : vmp
 */ 

#include <stdint.h>
#include <avr/io.h>
#include "../application/applicationInterface.h"

int main(void) {
  applicationSetup();
  applicationLoop();
  while (1) {
  }
}
