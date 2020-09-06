#include "View.h"
#include <LiquidCrystalFast.h>
#include "Pinmap.h"

#define DISPLAY_X 20
#define DISPLAY_Y 4

static LiquidCrystalFast lcd(DISPLAY_RS_PIN, DISPLAY_E_PIN, DISPLAY_D4_PIN, DISPLAY_D5_PIN, DISPLAY_D6_PIN, DISPLAY_D7_PIN);
static char line[DISPLAY_Y][DISPLAY_X];

static uint8_t messageCursor = 0;

void view_init(void)
{
  //initialize the screen
  lcd.begin(DISPLAY_X, DISPLAY_Y);
  view_println("Display Init   - OK");
}

void updateScreen(void)
{
  lcd.println(line[0]);
  lcd.println(line[1]);
  lcd.println(line[2]);
  lcd.println(line[3]);
}

void view_println(char* newLine)
{
  if(messageCursor >= DISPLAY_Y)
  {
    for(int i = 0; i < (DISPLAY_Y - 1); i++)
    {
      for(int j = 0; j < DISPLAY_X; j++)
      {
        line[i][j] = line[i + 1][j];
      }
    }
  }
  else
  {
    for(int i = 0; i < DISPLAY_X; i++)
    {
      line[messageCursor][i] = newLine[i];
    }
    messageCursor++;
  }
  updateScreen();
}
