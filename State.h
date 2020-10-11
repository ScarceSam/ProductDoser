#pragma once

enum steps{ IDLE_STEP, DOSE_STEP, FLUSH_STEP, RINSE_STEP };

typedef struct {
  uint8_t current_step = 0;
  uint8_t current_washer = 0;
  uint8_t current_detergent = 0;
  uint32_t step_length_millis[4] = {0};
  uint32_t step_start_millis = 0;
}system_t;

//TODO: Move to .cpp

bool state_start(uint8_t washer, uint8_t detergent);

void state_advance(void);

uint8_t state_currentState(void);

bool state_isStepComplete(void);

void state_copyStateData(system_t*);

uint16_t state_remainingMillis(void);
