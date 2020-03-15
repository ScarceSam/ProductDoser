#ifndef Washer_h
#define Washer_h

#include <Arduino.h>

#define NUMBER_OF_PINS 4
#define ID_LIMIT 16 
#define NUMBER_OF_WASHERS 24

typedef struct {
  uint8_t number;
  uint8_t i2cChannel;
  uint8_t com_pin[NUMBER_OF_PINS];
  uint8_t valve_pin;
  uint8_t washerSize;
  char washerId[ID_LIMIT];
} washer_t;

extern washer_t washer[NUMBER_OF_WASHERS];

#endif
