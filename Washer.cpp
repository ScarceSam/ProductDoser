#include <Arduino.h>
#include "pinmap.h"
#include "Washer.h"
#include "SDcard.h"
#include "Chars.h"
#include <PDCanBus.h>

#define NUMBER_OF_WASHERS 24

typedef struct {
  uint8_t number;
  uint8_t can_bus_address;
  uint8_t washer_size = 0;
  char washer_id[ID_LIMIT];
} washer_t;


static washer_t washer[NUMBER_OF_WASHERS];
static uint8_t washer_queue[24][2];
static uint8_t enqueue_cursor = 0;
static uint8_t dequeue_cursor = 0;
static uint8_t washers_enqueued = 0;

void washer_init(void)
{
  PDCB_init();
  
  for(int i = 0; i < NUMBER_OF_WASHERS; i++)
  {
    //chronologically label the washers
    washer[i].number = i+1;
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
  return; //TODO
}

void washer_close_all_valves(void)
{
  return; //TODO
}

uint8_t washer_size(uint8_t washer_number)
{
  return washer[washer_number - 1].washer_size;
}

void washer_pollWashers(void)
{
  bool canBusFlag = 0;
  canBusFlag = PDCB_PacketReceived();
  if(canBusFlag)
  {

    char message_packet[8];
    uint32_t sender_address = PDCB_PacketRetreive(message_packet);
    uint8_t shortened_address = sender_address;

    for(int qwasher = 0; qwasher < NUMBER_OF_WASHERS; qwasher++)
    {
      //Serial.println(washer[qwasher].can_bus_address);
      if(shortened_address == washer[qwasher].can_bus_address)
      {
        if(message_packet[0] == 'D' && message_packet[1] == 'P')
        {
          washer_queue[enqueue_cursor][0] = qwasher + 1;
          washer_queue[enqueue_cursor][1] = (message_packet[2] - '0');
          enqueue_cursor = ((1 + enqueue_cursor) % NUMBER_OF_WASHERS);
          washers_enqueued++;
        }
      }
    }
  }
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
      //washers_loaded++;
    }

    uint8_t washer_address = 0;
    SDcard_read_int(washer_name, "moduleid", &washer_address);
    if(washer_address > 0)
    {
      washer[i].can_bus_address = washer_address;
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
