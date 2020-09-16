#pragma once

enum steps{ IDLE_STEP, DOSE_STEP, FLUSH_STEP, RINSE_STEP };

typedef struct {
  uint8_t current_step = 0;
  uint8_t current_washer = 0;
  uint8_t current_detergent = 0;
  uint32_t step_length_millis = 0;
  uint32_t step_start_millis = 0;
}system_t;

static system_t system_info;//TODO: Move to .cpp

uint32_t start_dosing(uint8_t washer, uint8_t detergent);

uint32_t advance_step(void);

uint8_t if_idle(void);
