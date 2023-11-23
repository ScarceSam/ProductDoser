#pragma once
#include "Config.h"

#if defined(MAIN_BOARD_V02_01) || defined(MAIN_BOARD_V02_02)

#define BUTTON_UP (1<<0)
#define BUTTON_LEFT (1<<1)
#define BUTTON_ENTER (1<<2)
#define BUTTON_RIGHT (1<<3)
#define BUTTON_DOWN (1<<4)
#define BUTTON_RETURN (1<<5)

#elif defined(MAIN_BOARD_V03_01)

#define BUTTON_UP 37
#define BUTTON_DOWN 36
#define BUTTON_LEFT 35
#define BUTTON_RIGHT 34
#define BUTTON_ENTER 33
#define BUTTON_RETURN 38

#else

#fail

#endif

void buttons_init(void);

int buttons_pushed(void);
