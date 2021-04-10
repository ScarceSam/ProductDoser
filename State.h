#pragma once

enum steps{ PAUSE_STEP, IDLE_STEP, DOSE_STEP, FLUSH_STEP, RINSE_STEP };

typedef struct {
  uint8_t current_step = IDLE_STEP;
  uint8_t current_washer = 0;
  uint8_t current_product = 0;
  uint32_t step_length_millis[5] = {1};
  uint32_t step_start_millis = 0;
}system_t;

//TODO: Move to .cpp

bool state_start(uint8_t washer, uint8_t product);

void state_advance(void);

uint8_t state_current_state(void);

bool state_is_step_complete(void);

void state_copy_state_data(system_t*);

uint32_t state_remaining_millis(void);

void state_check_skip_rinse(uint8_t);

bool state_paused(void);

bool state_unpaused(void);
