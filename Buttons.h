#pragma once

#define BUTTON_UP (1<<0)
#define BUTTON_LEFT (1<<1)
#define BUTTON_ENTER (1<<2)
#define BUTTON_RIGHT (1<<3)
#define BUTTON_DOWN (1<<4)
#define BUTTON_RETURN (1<<5)

void buttons_init(void);

int buttons_pushed(void);
