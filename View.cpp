#include "View.h"
#include <LiquidCrystalFast.h>
#include "Pinmap.h"

#define DISPLAY_X 20
#define DISPLAY_Y 4

static LiquidCrystalFast lcd(DISPLAY_RS_PIN, DISPLAY_E_PIN, DISPLAY_D4_PIN, DISPLAY_D5_PIN, DISPLAY_D6_PIN, DISPLAY_D7_PIN);
static char line[DISPLAY_Y][DISPLAY_X+1];

static uint8_t messageCursor = 0;

void shiftUp(void);
void copyToLine(int li, char* newMessage);


void view_init(void)
{
  //initialize the screen
  lcd.begin(DISPLAY_X, DISPLAY_Y);

  pinMode(DISPLAY_R_PIN, OUTPUT);
  pinMode(DISPLAY_G_PIN, OUTPUT);
  pinMode(DISPLAY_B_PIN, OUTPUT);

  digitalWrite(DISPLAY_R_PIN, LOW);
  digitalWrite(DISPLAY_G_PIN, LOW);
  digitalWrite(DISPLAY_B_PIN, LOW);

}

void updateScreen(void)
{
  for(int i = 0; i < DISPLAY_Y; i++)
  {
    lcd.setCursor(0, i);
    lcd.print(line[i]);
  }
  
}

void view_println(char* newLine)
{
  if(messageCursor = DISPLAY_Y)
  {
    shiftUp();
    copyToLine(3, newLine);
  }
  else
  {
    copyToLine(messageCursor, newLine);
    messageCursor++;
  }
  updateScreen();
}

void shiftUp(void)
{
  for(int i = 0; i < (DISPLAY_Y - 1); i++)
    {
      for(int j = 0; j < DISPLAY_X; j++)
      {
        line[i][j] = line[i + 1][j];
      }
    }
}

void copyToLine(int li, char* newMessage)
{
  for(int i = 0; i < DISPLAY_X; i++)
  {
    if(i < 20)
      line[li][i] = newMessage[i];
  }
}
