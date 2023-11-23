#pragma once
#include "Config.h"

#if defined(ARDUINO_TEENSY41)

#define DISPLAY_RS_PIN 0
#define DISPLAY_E_PIN 1
#define DISPLAY_D4_PIN 2
#define DISPLAY_D5_PIN 3
#define DISPLAY_D6_PIN 4
#define DISPLAY_D7_PIN 5
//#define DISPLAY_R_PIN 6
#define DISPLAY_G_PIN 6
#define DISPLAY_B_PIN 7
#define MCP23017_SDA_PIN 18
#define MCP23017_SLC_PIN 19
#define MCP23017_INT_PIN 20
#define FEEDLINE_END_DRAIN_VALVE_PIN 24
#define FEEDLINE_WATER_VALVE_PIN 25
#define FEEDLINE_MANIFOLD_DRAIN_VALVE_PIN 26
#define PRODUCT_1_PUMP_PIN 29
#define PRODUCT_2_PUMP_PIN 30
#define PRODUCT_3_PUMP_PIN 31
#define PRODUCT_4_PUMP_PIN 32
#define FLOWSENSOR_PIN 39
#define PRODUCT_1_LEVEL_PIN 17
#define PRODUCT_2_LEVEL_PIN 16
#define PRODUCT_3_LEVEL_PIN 15
#define PRODUCT_4_LEVEL_PIN 14
#define TCAN1051_S 21
#define TCAN1051_RXD 22
#define TCAN1051_TXD 23

#else

#fail

#endif

//Version specific pins
#if defined(MAIN_BOARD_V02_01)

#define SX1509_SDA_PIN 18
#define SX1509_SCL_PIN 19
#define SX1509_INT_PIN 40 //with bodge wire between the top of C400/R400 and Spr.3

#elif defined(MAIN_BOARD_V02_02)

#define SX1509_SDA_PIN 18
#define SX1509_SCL_PIN 19
#define SX1509_INT_PIN 21

#elif defined(MAIN_BOARD_V03_01)

#define NEOPIXEL 9

#else

#fail

#endif
