#include <Arduino.h>
#include "Washer.h"
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define NUMBER_OF_WASHERS 24
#define I2C_START_ADDRESS 0
#define INPUTS_PER_EXPANSION 16
#define WASHERS_PER_EXPANSION (INPUTS_PER_EXPANSION / NUMBER_OF_PINS) 
#define USED_PINS_PER_EXPANSION (WASHERS_PER_EXPANSION * NUMBER_OF_PINS)
#define VALVE_OPEN HIGH

static washer_t washer[NUMBER_OF_WASHERS];
static Adafruit_MCP23017 mcp[7];

void washer_init(void)
{
  for(int i = 0; i < 8; i++)
  {
    mcp[i].begin(i);
  }

  for(int i = 0; i < NUMBER_OF_WASHERS; i++)
  {
    //chronologically label the washers
    washer[i].number = i+1;

    //Assign the GPIO expander I2C address
    washer[i].i2cAddress = I2C_START_ADDRESS + (i/WASHERS_PER_EXPANSION);

    //Assign the GPIO expander pins
    for(int j = 0; j < NUMBER_OF_PINS; j++)
    {
      //First assign the inputs from the washers
      if(j < (NUMBER_OF_PINS - 1))
      {
        washer[i].com_pin[j] = (((i % USED_PINS_PER_EXPANSION) * NUMBER_OF_PINS) + j) % USED_PINS_PER_EXPANSION;
      }
      
      //The last pin is an output to the Washer valve
      else
      {
        washer[i].valve_pin = (((i % USED_PINS_PER_EXPANSION) * NUMBER_OF_PINS) + j) % USED_PINS_PER_EXPANSION;
        mcp[washer[i].i2cAddress].pinMode(washer[i].valve_pin, OUTPUT);
        mcp[washer[i].i2cAddress].digitalWrite(washer[i].valve_pin, !VALVE_OPEN);
      }
    }
  }
}

uint8_t washer_ready(void)
{
  return 1;
}

void washer_get_next(uint8_t next_in_queue[])
{
  next_in_queue[0] = 1;
  next_in_queue[1] = 1;
}

washer_t washer_get_data(uint8_t washer_number)
{
  return washer[washer_number - 1];
}

void washer_open_valve(uint8_t washer_number)
{
  mcp[washer[washer_number - 1].i2cAddress].digitalWrite(washer[washer_number - 1].valve_pin, VALVE_OPEN);
}

void washer_close_valve(uint8_t washer_number)
{
  mcp[washer[washer_number - 1].i2cAddress].digitalWrite(washer[washer_number - 1].valve_pin, !VALVE_OPEN);
}

uint8_t washer_size(uint8_t washer_number)
{
  return washer[washer_number - 1].washer_size;
}
