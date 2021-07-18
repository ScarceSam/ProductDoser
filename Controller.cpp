#include <Arduino.h>
#include "View.h"
#include "State.h"
#include "Buttons.h"
#include "Menutree.h"
#include "chars.h"
#include "Feedline.h"
#include "Product.h"
#include "Washer.h"

#define CAL_VOLUME 32;

void display_state(void);
bool display_menu(int buttons_pressed);
void change_menu_position(int* menu_location, int* menu_selection, int button_pressed);
void assemble_menu_text(char displaied_text[4][21], int menu_location, int menu_selection, int buttons_pressed);
long assemble_func_text(char text[4][21], int menu_location, int menu_selection, int* buttons_pressed);
bool toggle_check(int device, bool b_state);
void toggle_device(int device, bool b_state);

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

  if(current_state <= IDLE_STEP)
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
  static bool in_menu = false;
  static int menu_location = -1;
  static int menu_selection = 0;
  static long interaction_at = 0;
  static long menu_timeout = 5000;
  static bool in_function = false;
  static long callback_time = 0;
  static long callback_set = 0;
  static char menu_text[4][21];
  bool callback_flag = false;

  if(buttons_pressed)
  {
    interaction_at = millis();
  }
  else if(!in_function && (millis() - interaction_at) > menu_timeout)
  {
    menu_location = -1;
    in_menu = false;
  }

  if(in_function && callback_time && ((millis() - callback_set) > callback_time))
  {
    callback_flag = true;
  }
  else
  {
    callback_flag = false;
  }

  if(callback_flag || buttons_pressed)
  {
    for(int i = 0; i < 4; i++)
    {
      clear_char_array(menu_text[i], 21);
    }

    if(!in_function)
    {
      change_menu_position(&menu_location, &menu_selection, buttons_pressed);
    }

    if((menu_location >= MENU_ROOT) && (menu_is_function(menu_location)))
    {
      callback_time = assemble_func_text(menu_text, menu_location, menu_selection, &buttons_pressed);
      callback_set = millis();
      in_function = true;

      if(buttons_pressed & BUTTON_RETURN)
      {
        change_menu_position(&menu_location, &menu_selection, buttons_pressed);
        in_function = false;
      }
    }

    if((menu_location >= MENU_ROOT) && (!in_function))
    {
      for(int i = 0; i < 4; i++)
      {
        clear_char_array(menu_text[i], 21);
      }
      assemble_menu_text(menu_text, menu_location, menu_selection, buttons_pressed);
      in_menu = true;
    }
    else
    {
      in_menu = false;
    }

    if(in_menu || in_function)
    {
      view_clear();
      for(int i = 0; i < 4; i++)
      {
        view_println(menu_text[i]);
      }
    }
  }

  //(menu_location >= MENU_ROOT) ? (return_value = true) : (return_value = false);
  return (in_menu || in_function);//return_value;
}

void change_menu_position(int* menu_location, int* menu_selection, int buttons_pressed)
{
  if((*menu_location < MENU_ROOT) && (buttons_pressed & BUTTON_ENTER))
  {
    *menu_location = MENU_ROOT;
    *menu_selection = menu_get_child(*menu_location);
  }
  else if((*menu_location >= MENU_ROOT) && (buttons_pressed & BUTTON_ENTER))
  {
    int temp = *menu_selection;
    if(temp > MENU_ROOT && (!menu_is_function(temp) || state_paused()))
    {
      *menu_location = temp;
      *menu_selection = menu_get_child(*menu_location);
    }
  }
  else if((*menu_location >= MENU_ROOT) && (buttons_pressed & BUTTON_RETURN))
  {
    *menu_selection = *menu_location;
    *menu_location = menu_get_parent(*menu_location);
    if(menu_is_function(*menu_selection))
    {
      state_unpaused();
    }
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
  int displayed_menu = menu_get_child(menu_location);
  bool selection_in_range = false;
  bool at_end = false;

  //where to start display to make selection visible
  while(selection_in_range == false)
  {
    int test_point = displayed_menu;
    for(int i = 1; i < 4; i++)
    {
      if(0 == (menu_selection - test_point))
      {
        selection_in_range = true;
        if(i == (4 - 1))
          at_end = true;
      }
      test_point = menu_get_next_sibling(test_point);
    }

    if(!selection_in_range)
    {
      displayed_menu = menu_get_next_sibling(displayed_menu);
    }
  }

  if(at_end && (menu_get_child(menu_location) != menu_get_next_sibling(menu_selection)))
  {
    displayed_menu = menu_get_next_sibling(displayed_menu);
  }

  char_concatenate(displaied_text[0], "<", menu_get_name(menu_location), 21);
  char_concatenate(displaied_text[0], displaied_text[0], ">", 21);

  for(int i = 1; i < 4; i++)
  {
    if(displayed_menu == menu_selection)// && displayed_menu != 0)
    {
      char_concatenate(displaied_text[i], ">", menu_get_name(displayed_menu), 21);
    }
    else if(displayed_menu)
    {
      char_concatenate(displaied_text[i], " ", menu_get_name(displayed_menu), 21);
    }

    displayed_menu = menu_get_next_sibling(displayed_menu);
  }
}

long assemble_func_text(char text[4][21], int menu_location, int menu_selection, int* buttons_pressed)
{
      static int return_value = 0;
      char_concatenate(text[0], "<", menu_get_name(menu_location), 21);
      char_concatenate(text[0], text[0], ">", 21);
      copy_char_array(text[2], "function = nullptr", 21);
      return_value = menu_function(menu_location, text, buttons_pressed);

      return return_value;
}
enum manual_functions{
  CONT_WATER_VALVE, CONT_MANIFOLD_DRAIN,  CONT_LINE_DRAIN,
  CONT_PUMP1, CONT_PUMP2, CONT_PUMP3, CONT_PUMP4,
  CONT_WASHER1, CONT_WASHER2, CONT_WASHER3, CONT_WASHER4,
  CONT_WASHER5, CONT_WASHER6, CONT_WASHER7, CONT_WASHER8,
  CONT_WASHER9, CONT_WASHER10, CONT_WASHER11, CONT_WASHER12,
  CONT_WASHER13, CONT_WASHER14, CONT_WASHER15, CONT_WASHER16,
  CONT_WASHER17, CONT_WASHER18, CONT_WASHER19, CONT_WASHER20,
  CONT_WASHER21, CONT_WASHER22, CONT_WASHER23, CONT_WASHER24,
  CONT_LAST_ITEM
};

static char manual_functions[CONT_LAST_ITEM][15]{
  "Water valve   ", "Manifold drain", "Line end drain", 
  "Product 1 pump", "Product 2 pump", "Product 3 pump", "Product 4 pump",
  "Washer  1 valv", "Washer  2 valv", "Washer  3 valv", "Washer  4 valv",
  "Washer  5 valv", "Washer  6 valv", "Washer  7 valv", "Washer  8 valv",
  "Washer  9 valv", "Washer 10 valv", "Washer 11 valv", "Washer 12 valv",
  "Washer 13 valv", "Washer 14 valv", "Washer 15 valv", "Washer 16 valv",
  "Washer 17 valv", "Washer 18 valv", "Washer 19 valv", "Washer 20 valv",
  "Washer 21 valv", "Washer 22 valv", "Washer 23 valv", "Washer 24 valv"
};

int controller_manual_func(char displaied_text[4][21], int* buttons_pressed)
{
  static bool b_inited = false;  //used to initialize on first run.
  static bool b_running = false; //used to ignore button input used to enter function.
  static int cursor_position = 0;
  static int screen_position = 1;
  static bool b_position[CONT_LAST_ITEM];

  if(!b_inited)
  {
    for(int i = 0; i < CONT_LAST_ITEM; i++)
    {
      b_position[i] = false;
    }
    b_inited = true;
  }

  if(b_running == false)
    *buttons_pressed = 0;

  switch(*buttons_pressed)
  {
    case BUTTON_DOWN:
      cursor_position < (CONT_LAST_ITEM - 1) ? cursor_position++ : cursor_position = (CONT_LAST_ITEM - 1);
      cursor_position == (CONT_LAST_ITEM - 1) ? screen_position = 3 : screen_position = 2;
      break;
    case BUTTON_UP:
      cursor_position > 0 ? cursor_position-- : cursor_position = 0;
      cursor_position == 0 ? screen_position = 1 : screen_position = 2;
      break;
    case BUTTON_ENTER:
      b_position[cursor_position] = toggle_check(cursor_position, !b_position[cursor_position]);
      toggle_device(cursor_position, b_position[cursor_position]);
      break;
    case BUTTON_RETURN:
      for(int i = 1; i < CONT_LAST_ITEM; i++)
      {
        b_position[i-1] = toggle_check(i-1, false);
        toggle_device(i-1, false);
      }
      break;
  }

  for( int i = 1; i < 4; i++)
  {
    if(screen_position == i)
      char_concatenate(displaied_text[i], "", ">", 21);
    else
      char_concatenate(displaied_text[i], "", " ", 21);

    char_concatenate(displaied_text[i], displaied_text[i], manual_functions[cursor_position + i - screen_position], 21);
    char_concatenate(displaied_text[i], displaied_text[i], (b_position[cursor_position + i - screen_position] ? " ON  " : " OFF "), 21);
  }

  if(*buttons_pressed == BUTTON_RETURN)
  {
    b_running = false;
    cursor_position = 0;
    screen_position = 1;
  }
  else
  {
    b_running = true;
  }
}

bool toggle_check(int device, bool b_state)
{
  static bool input = false;
  static int input_device = 0;
  static bool output = false;
  static int output_device = 0;
  bool return_value = false;

  bool device_is_input[CONT_LAST_ITEM]{
    true, false, false,
    true, true, true, true,
    false, false, false, false,
    false, false, false, false,
    false, false, false, false,
    false, false, false, false,
    false, false, false, false,
    false, false, false, false
  };

  if(!device_is_input[device])
  {
    if(!output && b_state && !output_device)
    {
      output = b_state;
      output_device = device;
    }
    else if(!b_state && !input)
    {
      output = b_state;
      output_device = 0;
    }

    if(device == output_device)
    {
      return_value = output;
    }
  }
  else if(output)
  {
    if(!input && b_state)
    {
      input = b_state;
      input_device = device;
    }
    else if(!b_state)
    {
      input = b_state;
      input_device = 0;
    }

    if(device == input_device)
    {
      return_value = input;
    }
  }

  return return_value;
}

void toggle_device(int device, bool b_state)
{
  if(device == CONT_WATER_VALVE)
  {
    feedline_valve(WATER_VALVE, b_state);
  }
  if(device == CONT_MANIFOLD_DRAIN)
  {
    feedline_valve(MANIFOLD_DRAIN_VALVE, b_state);
  }
  if(device == CONT_LINE_DRAIN)
  {
    feedline_valve(LINE_DRAIN_VALVE, b_state);
  }
  if((device >= CONT_PUMP1) && (device <= CONT_PUMP4))
  {
    if(b_state == true)
    {
      product_pump_on(device - CONT_PUMP1 + 1);
    }
    else
    {
      product_all_pumps_off();
    }
  }
  if((device >= CONT_WASHER1) && (device <= CONT_WASHER24))
  {
    if(b_state == true)
    {
      washer_open_valve(device - CONT_WASHER1 + 1);
    }
    else
    {
      washer_close_all_valves();
    }
  }
}

long controller_calibrate_product(char displaied_text[4][21], int* buttons_pressed)
{
  /*
   * Calibration steps;
   * 1. Select pump to calibrate
   * 2. start calibration?
   * 3. pump 30 oz with option to stop.
   * 4. if stopped, continue pumping or enter amount?
   * 5. Display amount calculated. user can adjust and/or confirm calibration.
   * 6. return to 2.
   */
  static bool b_running = false; //used to indicate if this function is in use.
  enum cal_states{ CAL_SELECT, CAL_BEGIN, CAL_PRIME, CAL_RUN, CAL_CHECK };
  static char cal_state_names[CAL_CHECK+1][20] {"Select Pump     ", "Confirm", "Priming:", "Pumping:", "Check"};
  static int cal_state = CAL_SELECT;
  static int cal_pump = 1;
  static int millis_pumped = 0;
  static float ozs_pumped = 0;
  int return_value = 0;

  //clear the button push that entered this function
  if(b_running == false)
    *buttons_pressed = 0;

  if(cal_state == CAL_SELECT)
  {
    char_concatenate(displaied_text[1], "", cal_state_names[0], 21);
    clear_char_array(displaied_text[2], 21);

    if(*buttons_pressed == BUTTON_RIGHT && cal_pump < 4)
      cal_pump++;
    if(*buttons_pressed == BUTTON_LEFT && cal_pump > 1)
      cal_pump--;

    if(cal_pump == 1)
      char_concatenate(displaied_text[3], "", " >1<   2    3    4  ", 21);
    if(cal_pump == 2)
      char_concatenate(displaied_text[3], "", "  1   >2<   3    4  ", 21);
    if(cal_pump == 3)
      char_concatenate(displaied_text[3], "", "  1    2   >3<   4  ", 21);
    if(cal_pump == 4)
      char_concatenate(displaied_text[3], "", "  1    2    3   >4< ", 21);

    if(*buttons_pressed == BUTTON_ENTER)
    {
      cal_state++;
    }
    if(*buttons_pressed != BUTTON_RETURN)
      *buttons_pressed = 0;
  }


  if(cal_state == CAL_BEGIN)
  {
    static bool b_confirm = false;

    if(*buttons_pressed == BUTTON_RIGHT && b_confirm == true)
      b_confirm = false;
    else if(*buttons_pressed == BUTTON_LEFT && b_confirm == false)
      b_confirm = true;
    if(*buttons_pressed == BUTTON_ENTER  && b_confirm == true)
    {
      cal_state++;
      b_confirm = false;
    }
    else if(*buttons_pressed == BUTTON_ENTER  && b_confirm == false)
    {
      cal_state--;
      return_value = 1;
    }

    if(*buttons_pressed != BUTTON_RETURN)
      *buttons_pressed = 0;
    else
      b_confirm = false;

    char_concatenate(displaied_text[1], cal_state_names[1], ", Pump ", 21);
    char_append_digits(displaied_text[1], cal_pump, 21);
    char_concatenate(displaied_text[1], displaied_text[1], "?", 21);

    clear_char_array(displaied_text[2], 21);

    if(b_confirm == true)
      char_concatenate(displaied_text[3], "", "   >Yes<    Back    ", 21);
    if(b_confirm == false)
      char_concatenate(displaied_text[3], "", "    Yes    >Back<   ", 21);
  }


  if(cal_state == CAL_PRIME)
  {
    static bool b_priming = false;
    static bool b_prime_selection = true; //true == left side selected
    static bool b_prime_next = false;

    if(*buttons_pressed == BUTTON_LEFT && !b_prime_selection)
      b_prime_selection = true;
    else if(*buttons_pressed == BUTTON_RIGHT && b_prime_selection)
      b_prime_selection = false;
    else if(*buttons_pressed == BUTTON_ENTER && b_prime_selection && b_priming)
      b_priming = false;
    else if(*buttons_pressed == BUTTON_ENTER && b_prime_selection && !b_priming)
      b_priming = true;
    else if(*buttons_pressed == BUTTON_ENTER && !b_prime_selection)
      b_prime_next = true;

    if(*buttons_pressed != BUTTON_RETURN)
      *buttons_pressed = 0;

    char_concatenate(displaied_text[1], "", cal_state_names[2], 21);
    clear_char_array(displaied_text[2], 21);

    if(b_priming)
    {
      char_concatenate(displaied_text[1], displaied_text[1], " Running", 21);
      if(b_prime_selection)
        char_concatenate(displaied_text[3], "", "  >STOP <    NEXT   ", 21);
      else
        char_concatenate(displaied_text[3], "", "   STOP     >NEXT<  ", 21);
    }else if(!b_priming)
    {
      char_concatenate(displaied_text[1], displaied_text[1], " Paused", 21);
      if(b_prime_selection)
        char_concatenate(displaied_text[3], "", "  >START<    NEXT   ", 21);
      else
        char_concatenate(displaied_text[3], "", "   START    >NEXT<  ", 21);
    }

    if(b_prime_next == true || *buttons_pressed == BUTTON_RETURN)
    {
      cal_state++;
      b_priming = false;
      b_prime_selection = true;
      b_prime_next = false;
    }

    if(b_priming)
    {
      product_pump_on(cal_pump);
      feedline_valve(MANIFOLD_DRAIN_VALVE, VALVE_OPEN);
    }
    else
    {
      product_all_pumps_off();
      feedline_valve(MANIFOLD_DRAIN_VALVE, VALVE_CLOSE);
    }
  }


  if(cal_state == CAL_RUN)
  {
    static bool b_started = false;
    static bool b_calibrating = false;
    static bool b_cal_selection = true;
    static bool b_cal_next = false;
    static int cal_volume = CAL_VOLUME;
    static int32_t cal_time = 0;
    static int mills_per_oz = 0;
    static bool b_cal_pumping = false;
    static uint32_t pump_start = 0;
    static uint32_t old_pump_start = 0;

    if(!b_started)
    {
      cal_volume = CAL_VOLUME;
      cal_time = product_pump_millis(cal_pump, cal_volume);
      mills_per_oz = (cal_time / cal_volume);
      b_started = true;
      millis_pumped = 0;
    }

    if(*buttons_pressed == BUTTON_LEFT && !b_cal_selection)
      b_cal_selection = true;
    else if(*buttons_pressed == BUTTON_RIGHT && b_cal_selection)
      b_cal_selection = false;
    else if(*buttons_pressed == BUTTON_ENTER && b_cal_selection && b_calibrating)
      b_calibrating = false;
    else if(*buttons_pressed == BUTTON_ENTER && b_cal_selection && !b_calibrating)
      b_calibrating = true;
    else if(*buttons_pressed == BUTTON_ENTER && !b_cal_selection)
      b_cal_next = true;

    if(*buttons_pressed != BUTTON_RETURN)
      *buttons_pressed = 0;

    if(b_cal_pumping && b_calibrating)
    {
      b_cal_selection = true;
      uint32_t time_passed = (millis() - pump_start);
      cal_time -= time_passed;
      millis_pumped += time_passed;
      pump_start = millis();
      return_value = (cal_time < mills_per_oz ? cal_time : mills_per_oz);
      cal_volume = 1 + (cal_time / mills_per_oz);
    }
    else if(!b_cal_pumping && b_calibrating)
    {
      b_cal_selection = true;
      pump_start = millis();
      return_value = (cal_time < mills_per_oz ? cal_time : mills_per_oz);
    }
    else if(b_cal_pumping && !b_calibrating)
    {
      uint32_t time_passed = (millis() - pump_start);
      cal_time -= time_passed;
      millis_pumped += time_passed;
    }
    else if(!b_cal_pumping && !b_calibrating)
    {
    }

    if(cal_time <= 0)
    {
      b_calibrating = false;
      b_cal_selection = false;
      cal_volume = 0;
    }

    char_concatenate(displaied_text[1], "", cal_state_names[3], 21);
    char_concatenate(displaied_text[2], "Volume remaining: ", "", 21);
    char_append_digits(displaied_text[2], cal_volume, 21);

    if(b_calibrating)
    {
      char_concatenate(displaied_text[1], displaied_text[1], " Running", 21);
      if(b_cal_selection)
        char_concatenate(displaied_text[3], "", "  >STOP <    NEXT   ", 21);
      else
        char_concatenate(displaied_text[3], "", "   STOP     >NEXT<  ", 21);
    }else if(!b_calibrating)
    {
      char_concatenate(displaied_text[1], displaied_text[1], " Paused", 21);
      if(b_cal_selection)
        char_concatenate(displaied_text[3], "", "  >START<    NEXT   ", 21);
      else
        char_concatenate(displaied_text[3], "", "   START    >NEXT<  ", 21);
    }

    if(b_cal_next == true || *buttons_pressed == BUTTON_RETURN)
    {
      cal_state++;
      b_calibrating = false;
      b_cal_selection = true;
      b_cal_next = false;
      b_started = false;
    }

    if(b_calibrating)
    {
      product_pump_on(cal_pump);
      feedline_valve(MANIFOLD_DRAIN_VALVE, VALVE_OPEN);
      b_cal_pumping = true;
    }
    else
    {
      product_all_pumps_off();
      feedline_valve(MANIFOLD_DRAIN_VALVE, VALVE_CLOSE);
      b_cal_pumping = false;
    }
  }

  if(cal_state == CAL_CHECK)
  {
    static bool b_inited = false;
    static int cursor_position = 1;
    static bool b_save = false;
    static bool b_saving = false;
    static bool b_cal_next = false;
    float difference = 0;

    if(b_inited == false)
    {
      ozs_pumped = ((float)millis_pumped / (float)product_pump_millis(cal_pump, 1));
      b_inited = true;
    }

    if(*buttons_pressed == BUTTON_LEFT && cursor_position < 3)
      cursor_position++;
    else if(*buttons_pressed == BUTTON_RIGHT && cursor_position > 0)
      cursor_position--;
    else if(*buttons_pressed == BUTTON_DOWN)
    {
      cursor_position == 1 ? difference -= 0.1:0;
      cursor_position == 2 ? difference -= 1.0:0;
      cursor_position == 3 ? difference -= 10.0:0;
    }
    else if(*buttons_pressed == BUTTON_UP)
    {
      cursor_position == 1 ? difference += 0.1:0;
      cursor_position == 2 ? difference += 1.0:0;
      cursor_position == 3 ? difference += 10.0:0;
    }
    else if(*buttons_pressed == BUTTON_ENTER && cursor_position == 0)
      b_save = true;

    if(*buttons_pressed != BUTTON_RETURN)
      *buttons_pressed = 0;

    clear_char_array(displaied_text[1], 21);
    char_concatenate(displaied_text[1], "", "Enter Volume Pumped", 21);

    clear_char_array(displaied_text[2], 21);
    char_concatenate(displaied_text[2], "", "Ounces : ", 21);
    if((ozs_pumped + difference > 0) && (ozs_pumped + difference < 100))
      ozs_pumped += difference;
    char buf[7];
    sprintf(buf, "%04.1f", ozs_pumped);
    char_concatenate(displaied_text[2], displaied_text[2], buf, 21);

    clear_char_array(displaied_text[3], 21);
    if(cursor_position == 0)
      char_concatenate(displaied_text[3], "", "             >SAVE< ", 21);
    if(cursor_position == 1)
      char_concatenate(displaied_text[3], "", "            ^ SAVE  ", 21);
    if(cursor_position == 2)
      char_concatenate(displaied_text[3], "", "          ^   SAVE  ", 21);
    if(cursor_position == 3)
      char_concatenate(displaied_text[3], "", "         ^    SAVE  ", 21);

    if(b_save)
    {
      clear_char_array(displaied_text[1], 21);
      char_concatenate(displaied_text[1], "", " Saving Calibration ", 21);

      clear_char_array(displaied_text[2], 21);
      char_concatenate(displaied_text[2], "", "  oz/min: ", 21);
      long oz_min = ((60000.0 / (double)millis_pumped) * ((double)ozs_pumped));
      sprintf(buf, "%d", oz_min);
      char_concatenate(displaied_text[2], displaied_text[2], buf, 21);

      clear_char_array(displaied_text[3], 21);

      if(b_saving)
      {
        if(product_save_calibration((cal_pump - 1), oz_min))
          char_concatenate(displaied_text[3], "", " Saved ", 21);
        else
          char_concatenate(displaied_text[3], "", " ERROR ", 21);
        b_cal_next = true;
        return_value = 5000;
      }
      else
      {
        b_saving = true;
        return_value = 1;
      }
    }

    if(b_cal_next == true || *buttons_pressed == BUTTON_RETURN)
    {
      b_inited = false;
      cursor_position = 1;
      b_save = false;
      b_saving = false;
      b_cal_next = false;
      difference = 0;
      cal_state = CAL_SELECT;
      ozs_pumped = 0;
      return_value += 1;
    }
    if(*buttons_pressed != BUTTON_RETURN)
      *buttons_pressed = 0;
  }

  //clean up if leaving the function else set the 'running' flag.
  if(*buttons_pressed == BUTTON_RETURN)
  {
    b_running = false;
    cal_pump = 1;
    cal_state = CAL_SELECT;
  }
  else
  {
    b_running = true;
  }

  return return_value;
}
