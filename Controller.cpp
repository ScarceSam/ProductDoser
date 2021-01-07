#include <Arduino.h>
#include "View.h"
#include "State.h"
#include "Buttons.h"

void display_state(void);

void controller_update_screen(void)
{
  static bool in_menu = false;
  static long interaction_at = 0;
  static long menu_timeout = 5000;

  int nav_buttons = buttons_pushed();
  if(nav_buttons)
  {
    interaction_at = millis();
  }

  if(in_menu)
  {
    if((millis() - interaction_at) > menu_timeout)
    {
      in_menu = false;
    }
  }
  else if(nav_buttons & (1<<3))
  {
    view_clear();
    view_println("        MENU        ");
    view_println("//under construction");
    in_menu = true;
  }
  else
  {
    display_state();
  }
}

void display_state(void){
  static uint8_t previous_state = 0;
  static uint16_t previous_sec = 0;

  uint8_t current_state = state_current_state();
  uint16_t current_sec = 0;

  if(current_state == IDLE_STEP)
  {
    current_sec = (millis() / 1000);
  }
  else
  {
    current_sec = (state_remaining_millis() / 1000);
  }

  if ((current_state != previous_state) || (current_sec != previous_sec))
  {
    view_display_state();
    previous_state = current_state;
    previous_sec = current_sec;
  }
}
