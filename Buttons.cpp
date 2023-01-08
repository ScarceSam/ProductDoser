#include <Arduino.h>
#include "Buttons.h"
#include "Pinmap.h"

#if defined(MAIN_BOARD_V02_01) || defined(MAIN_BOARD_V02_02)

#include <Wire.h>
#include <SparkFunSX1509.h>

SX1509 expander;

static const int BUTTONS[] = {2, 4, 5, 6, 8, 1};
static const int BUTTON_COUNT = (sizeof(BUTTONS)/sizeof(BUTTONS[0]));
static const int DEBOUNCE = 64;
static const int INTERRUPT_PIN = SX1509_INT_PIN;

void buttons_init(void)
{
  expander.begin(0x3E);

  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    expander.pinMode(BUTTONS[i], INPUT_PULLUP);
    expander.enableInterrupt(BUTTONS[i], CHANGE);
  }

  expander.debounceTime(DEBOUNCE);
  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    expander.debouncePin(BUTTONS[i]);
  }

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
}

int buttons_pushed(void)
{
  static int last_pushed = 0;
  int pushed = 0;

  if(digitalRead(INTERRUPT_PIN) == 0)
  {
    for(int i = 0; i < BUTTON_COUNT; i++)
    {
      if(expander.digitalRead(BUTTONS[i]) == LOW)
      {
        pushed |= (1<<i);
      }
      else
      {
        last_pushed &= ~(1<<i);
      }
    }
    pushed = pushed - last_pushed;
    last_pushed += pushed;
  }

  return pushed;
}

#elif defined(MAIN_BOARD_V03_01)

#define BUTTON_PUSHED 0

static const int BUTTONS[] = {BUTTON_UP, BUTTON_LEFT, BUTTON_ENTER, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_RETURN};
static const int BUTTON_COUNT = (sizeof(BUTTONS)/sizeof(BUTTONS[0]));
static const int DEBOUNCE = 64;

void buttons_init(void)
{
  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
}

int buttons_pushed(void)
{
  int pushed = 0;
  //static uint32_t last_check = millis();
  uint32_t current_check = millis();
  static uint32_t pushed_buttons[BUTTON_COUNT][2];// 0=button state 1=button time

  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    bool button_state = digitalRead(BUTTONS[i]);

    if (button_state != BUTTON_PUSHED)
    {
      pushed_buttons[i][0] = !BUTTON_PUSHED;
      pushed_buttons[i][1] = 0;
    }
    else if ((button_state == 0) && (pushed_buttons[i][1] == 0))
    {
      pushed_buttons[i][1] = current_check;
    }
    else if ((button_state == 0) && (current_check > (pushed_buttons[i][1] + DEBOUNCE)) && (pushed_buttons[i][0] != BUTTON_PUSHED))
    {
      pushed = BUTTONS[i];
      pushed_buttons[i][0] = BUTTON_PUSHED;
    }
  }

  return pushed;
}

#else

#fail

#endif
