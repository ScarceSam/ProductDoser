#ifndef Washer_h
#define Washer_h

#include <Arduino.h>

#define NUMBER_OF_PINS 5
#define ID_LIMIT 16 
#define NUMBER_OF_WASHERS 24
#define I2C_START_ADDRESS 0x20
#define INPUTS_PER_EXPANSION 15
#define WASHERS_PER_EXPANSION (INPUTS_PER_EXPANSION / NUMBER_OF_PINS) 
#define USED_PINS_PER_EXPANSION (WASHERS_PER_EXPANSION * NUMBER_OF_PINS)


typedef struct {
  uint8_t number;
  uint8_t i2cAddress;
  uint8_t com_pin[NUMBER_OF_PINS];
  uint8_t valve_pin;
  uint8_t washerSize;
  char washerId[ID_LIMIT];
} washer_t;

extern washer_t washer[NUMBER_OF_WASHERS];

void washer_init(void);

#endif
