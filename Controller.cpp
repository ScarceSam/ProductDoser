#include <Arduino.h>
#include "View.h"
#include "State.h"
#include "Buttons.h"
#include "Menutree.h"
#include "chars.h"

void display_state(void);
bool display_menu(int buttons_pressed);
void change_menu_position(int* menu_location, int* menu_selection, int button_pressed);
void assemble_menu_text(char displaied_text[4][21], int menu_location, int menu_selection, int buttons_pressed);

void controller_update_screen(void)
{
  static bool in_menu = false;
  int nav_buttons = buttons_pushed();

  if(nav_buttons || in_menu)
  {
    in_menu = display_menu(nav_buttons);
  }

  if(!in_menu)
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

bool display_menu(int buttons_pressed)
{
  bool return_value = false;
  static int menu_location = -1;
  static int menu_selection = 0;
  static long interaction_at = 0;
  static long menu_timeout = 5000;

  if(buttons_pressed)
  {
    interaction_at = millis();
  }
  else if((millis() - interaction_at) > menu_timeout)
  {
    menu_location = -1;
  }

  change_menu_position(&menu_location, &menu_selection, buttons_pressed);

  if((menu_location >= MENU_ROOT) && buttons_pressed)
  {
    char displaied_text[4][21];
    for(int i = 0; i < 4; i++)
    {
      clear_char_array(displaied_text[i], 21);
    }

    if(menu_is_function(menu_location))
    {
      copy_char_array(displaied_text[0], menu_get_name(menu_location), 21);
    }
    else
    {
      assemble_menu_text(displaied_text, menu_location, menu_selection, buttons_pressed);
    }

    view_clear();
    for(int i = 0; i < 4; i++)
    {
      view_println(displaied_text[i]);
    }
  }

  (menu_location >= MENU_ROOT) ? (return_value = true) : (return_value = false);
  return return_value;
}

void change_menu_position(int* menu_location, int* menu_selection, int buttons_pressed)
{
  if((*menu_location < MENU_ROOT) && (buttons_pressed & BUTTON_RIGHT))
  {
    *menu_location = MENU_ROOT;
    *menu_selection = menu_get_child(*menu_location);
  }
  else if((*menu_location >= MENU_ROOT) && (buttons_pressed & BUTTON_RIGHT))
  {
    int temp = *menu_selection;
    if(temp > MENU_ROOT)
      *menu_location = temp;

    *menu_selection = menu_get_child(*menu_location);
  }
  else if((*menu_location >= MENU_ROOT) && (buttons_pressed & BUTTON_LEFT))
  {
    *menu_selection = *menu_location;
    *menu_location = menu_get_parent(*menu_location);
  }
  else if((*menu_location >= MENU_ROOT) && (buttons_pressed & BUTTON_DOWN))
  {
    int temp = menu_get_next_sibling(*menu_selection);
    if(temp > MENU_ROOT)
      *menu_selection = temp;
  }
  else if((*menu_location >= MENU_ROOT) && (buttons_pressed & BUTTON_UP))
  {
    int temp = menu_get_prev_sibling(*menu_selection);
    if(temp > MENU_ROOT)
      *menu_selection = temp;
  }
}

void assemble_menu_text(char displaied_text[4][21], int menu_location, int menu_selection, int buttons_pressed)
{
  copy_char_array(displaied_text[0], menu_get_name(menu_location), 21);
  copy_char_array(displaied_text[1], menu_get_name(menu_selection), 21);
}
