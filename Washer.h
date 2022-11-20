#pragma once

#define ID_LIMIT 16 

void washer_init(void);
//initialize washers

uint8_t washer_enqueued(void);
//return the number of washers queued up

void washer_dequeue_next(uint8_t next_in_queue[]);
//return the next washer([0]) and Product([1]) in queueco

void washer_open_valve(uint8_t washer_number);
//open washer valve

void washer_close_all_valves(void);
//close all washer valves

uint8_t washer_size(uint8_t washer_number);
//return the size of the washer in pounds

void washer_pollWashers(void);
//check washers for dosage request

uint8_t washer_peek_product_in_queue(uint8_t queue_position);
//return the next product in the queue

uint8_t washer_load(void);
//load washer data from SD card

char* washer_label(uint8_t);
//return the washer name
