#include "View.h"
#include <LiquidCrystalFast.h>
#include "Pinmap.h"
#include "State.h"

#define DISPLAY_X 20
#define DISPLAY_Y 4

static LiquidCrystalFast lcd(DISPLAY_RS_PIN, DISPLAY_E_PIN, DISPLAY_D4_PIN, DISPLAY_D5_PIN, DISPLAY_D6_PIN, DISPLAY_D7_PIN);
static char line[DISPLAY_Y][DISPLAY_X+1];

static uint8_t messageCursor = 0;
char* stepNames[4];

void shiftUp(void);
void copyToLine(char* newMessage, char* oldMessage);
void concatinate(char* str1, char* str2, char* str3, uint8_t sizeLimit);
uint8_t charLen(char* string);

void view_init(void)
{
  stepNames[0] = "Idle";
  stepNames[1] = "Dosing";
  stepNames[2] = "Flushing";
  stepNames[3] = "Rinsing";

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
  if(messageCursor == DISPLAY_Y)
  {
    shiftUp();
    copyToLine(newLine, line[3]);
  }
  else
  {
    copyToLine(newLine, line[messageCursor]);
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

void copyToLine(char* newMessage, char* oldMessage)
{
  for(int i = 0; i < DISPLAY_X; i++)
  {
    if (newMessage[i] == '\0')
    {
      while(i < DISPLAY_X)
      {
        oldMessage[i] = ' ';
        i++;
      }
    }
    else
    {
    oldMessage[i] = newMessage[i];
    }
  }
}

void view_clear(void)
{
  for (int i = 0; i < DISPLAY_Y; i++)
  {
    copyToLine(" ", line[i]);
  }
  updateScreen();
  messageCursor = 0;
}

void view_display_state(void)
{
  static bool idle = 0;

  if (state_currentState() == 0 && idle == 0)
  {
    view_clear();
    copyToLine("     System Idle", line[1]);
    copyToLine("      And Ready", line[2]);
    updateScreen();
    idle = 1;
    return;
  }
  else if(state_currentState() != 0)
  {
  char scratch[DISPLAY_X + 1];

  //display line 1
  copyToLine("Wshr: ", line[0]);

  //display line 2
  copyToLine("Prod: ", line[1]);

  //display line 3
  concatinate("Step: ", stepNames[state_currentState()], scratch, 20);
  copyToLine(scratch, line[2]);

  //display line 4
  copyToLine("Time:", line[3]);
  updateScreen();

  idle == 0;
  }
}

void concatinate(char* str1, char* str2, char* str3, uint8_t sizeLimit)
{
  if (sizeLimit > DISPLAY_X)
  {
    sizeLimit = DISPLAY_X;
  }

  uint8_t endLoop = sizeLimit;
  uint8_t strOneLen = charLen(str1);

  for (int i = 0; i <= endLoop; i++)
  {
    //if iteration is less than end of string one
    if (i < strOneLen)
    {
      //copy charater from 1 to returnstring
      str3[i] = str1[i];
    }
    //if past string one end of line
    else if (i >= strOneLen)
    {
      //copy character form str2 to returnstring
      str3[i] = str2[i - strOneLen];
    }

    //if last iteration
    if (i == endLoop)
    {
      //end line
      str3[i] = '\0';
    }

    //if return string EOL
    if (str3[i] == '\0')
    {
      //finish iteration
      i = endLoop;
    }
  }
}

uint8_t charLen(char* string)
{
  for (int i = 0; i < (DISPLAY_X + 1);i++)
  {
    if( string[i] == '\0')
    {
      return i;
    }
  }
}
