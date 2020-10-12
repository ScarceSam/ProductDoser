#pragma once

#define ID_LIMIT 16 

void washer_init(void);

uint8_t washer_enqueued(void);

void washer_dequeue_next(uint8_t next_in_queue[]);

void washer_open_valve(uint8_t washer_number);

void washer_close_all_valves(void);

uint8_t washer_size(uint8_t washer_number);

void washer_pollWashers(void);

uint8_t washer_peek_detergent_in_queue(uint8_t queue_position);

uint8_t washer_load(void);

char* washer_label(uint8_t);
