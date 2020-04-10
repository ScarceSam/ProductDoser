#include <Arduino.h>
#include "Washer.h"
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define NUMBER_OF_WASHERS 24
#define NUMBER_OF_COM_PINS 4
#define NUMBER_OF_PINS (NUMBER_OF_COM_PINS + 1)
#define I2C_START_ADDRESS 0
#define INPUTS_PER_EXPANSION 16
#define WASHERS_PER_EXPANSION (INPUTS_PER_EXPANSION / NUMBER_OF_PINS) 
#define USED_PINS_PER_EXPANSION (WASHERS_PER_EXPANSION * NUMBER_OF_PINS)
#define VALVE_OPEN HIGH

typedef struct {
  uint8_t number;
  uint8_t i2cAddress;
  uint8_t com_pin[NUMBER_OF_PINS];
  uint8_t valve_pin;
  uint8_t washer_size = 20;
  char washer_id[ID_LIMIT];
} washer_t;

static washer_t washer[NUMBER_OF_WASHERS];
static Adafruit_MCP23017 mcp[8];

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
  static uint8_t test_zero = 0;
  static uint8_t test_one = 4;
  test_zero++;
  test_one--;
  test_zero = (test_zero > 3) ? 1 : test_zero;
  test_one = (test_one < 1) ? 4 : test_one; 
  next_in_queue[0] = test_zero;
  next_in_queue[1] = test_one;
}

void washer_open_valve(uint8_t washer_number)
{
  mcp[washer[washer_number - 1].i2cAddress].digitalWrite(washer[washer_number - 1].valve_pin, VALVE_OPEN);
}

void washer_close_all_valves(void)
{
  for(int i = 0; i < NUMBER_OF_WASHERS; i++)
  {
    mcp[washer[i].i2cAddress].digitalWrite(washer[i].valve_pin, !VALVE_OPEN);
  }
}

uint8_t washer_size(uint8_t washer_number)
{
  return washer[washer_number - 1].washer_size;
}
