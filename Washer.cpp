#include <Arduino.h>
#include "pinmap.h"
#include "Washer.h"
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "SDcard.h"
#include "Chars.h"

#define NUMBER_OF_WASHERS 24
#define NUMBER_OF_COM_PINS 4
#define NUMBER_OF_PINS (NUMBER_OF_COM_PINS + 1)
#define I2C_START_ADDRESS 0
#define INPUTS_PER_EXPANSION 16
#define WASHERS_PER_EXPANSION (INPUTS_PER_EXPANSION / NUMBER_OF_PINS) 
#define USED_PINS_PER_EXPANSION (WASHERS_PER_EXPANSION * NUMBER_OF_PINS)
#define VALVE_OPEN HIGH
#define UPDATE_INTERVAL 50
#define RELAY_ACTIVE 0
#define WASHER_DEBOUNCE 100

typedef struct {
  uint8_t number;
  uint8_t i2cAddress;
  uint8_t com_pin[NUMBER_OF_PINS];
  uint8_t valve_pin;
  uint8_t washer_size = 0;
  char washer_id[ID_LIMIT];
} washer_t;


static washer_t washer[NUMBER_OF_WASHERS];
static Adafruit_MCP23017 mcp[8];
static uint8_t washer_queue[24][2];
static uint8_t enqueue_cursor = 0;
static uint8_t dequeue_cursor = 0;
static uint8_t washers_enqueued = 0;

void washer_init(void)
{
  for(int i = 0; i < 8; i++)
  {
    mcp[i].begin(i);
    mcp[i].setupInterrupts(true, true, HIGH);
    pinMode(MCP23017_INT_PIN, INPUT_PULLUP);
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
        mcp[washer[i].i2cAddress].pinMode(washer[i].com_pin[j], INPUT);
        mcp[washer[i].i2cAddress].pullUp(washer[i].com_pin[j], HIGH);
        mcp[washer[i].i2cAddress].setupInterruptPin(washer[i].com_pin[j], FALLING);
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

uint8_t washer_enqueued(void)
{
  return washers_enqueued;
}

void washer_dequeue_next(uint8_t next_in_queue[])
{
  next_in_queue[0] = washer_queue[dequeue_cursor][0];
  washer_queue[dequeue_cursor][0] = 0;
  next_in_queue[1] = washer_queue[dequeue_cursor][1];
  washer_queue[dequeue_cursor][1] = 0;
  dequeue_cursor = ((1 + dequeue_cursor) % NUMBER_OF_WASHERS);
  washers_enqueued--;
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

void washer_pollWashers(void)
{
  if(digitalRead(MCP23017_INT_PIN) == HIGH)
    return;

  static uint32_t last_check = millis();
  static uint32_t time_temp_queue[24][4]; //[washer_number]
  static uint8_t temp_queued[24][4];
  uint32_t current_check = millis();

  if (current_check < (last_check + UPDATE_INTERVAL))
  {
    return;
  }

  for (int qwasher = 0; qwasher < NUMBER_OF_WASHERS; qwasher++)
  {
    for (int qpin = 0; qpin < NUMBER_OF_COM_PINS; qpin++)
    {
      uint8_t check = mcp[washer[qwasher].i2cAddress].digitalRead(washer[qwasher].com_pin[qpin]);

      if (check != RELAY_ACTIVE)
      {
        time_temp_queue[qwasher][qpin] = 0;
        temp_queued[qwasher][qpin] = 0;
      }
      else if ((check == RELAY_ACTIVE) && (time_temp_queue[qwasher][qpin] == 0))
      {
        time_temp_queue[qwasher][qpin] = current_check;
      }
      else if ((check == RELAY_ACTIVE) && (current_check > (time_temp_queue[qwasher][qpin] + WASHER_DEBOUNCE)) && (temp_queued[qwasher][qpin] == 0))
      {
        washer_queue[enqueue_cursor][0] = qwasher + 1;
        washer_queue[enqueue_cursor][1] = qpin + 1;
        temp_queued[qwasher][qpin] = 1;
        enqueue_cursor = ((1 + enqueue_cursor) % NUMBER_OF_WASHERS);
        washers_enqueued++;
      }
    }
  }
  last_check = millis();
}

uint8_t washer_peek_product_in_queue(uint8_t queue_position)
{
  return washer_queue[dequeue_cursor + queue_position][1];
}

uint8_t washer_load(void)
{
  uint8_t washers_loaded = 0;
  char washer_name[9];
  clear_char_array(washer_name, 9);
  const char* washer_word = "washer";
  copy_char_array(washer_name, washer_word, 7);

  for (uint8_t i = 0; i < NUMBER_OF_WASHERS; i++)
  {
    washer_name[6] = '\0';
    char_append_digits(washer_name, (i + 1), 9);

    uint8_t washer_capacity = 0;
    SDcard_read_int(washer_name, "capacitylbs", &washer_capacity);
    if(washer_capacity > 0)
    {
      washer[i].washer_size = washer_capacity;
      washers_loaded++;
    }

    char saved_name[ID_LIMIT];
    clear_char_array(saved_name, 10);
    SDcard_read_string(washer_name,  "label", saved_name, ID_LIMIT);

    if(saved_name[0] != '\0')
    {
      copy_char_array(washer[i].washer_id, saved_name, ID_LIMIT);
    }
  }

  return washers_loaded;
}

char* washer_label(uint8_t washerNumber)
{
  return washer[washerNumber - 1].washer_id;
}
