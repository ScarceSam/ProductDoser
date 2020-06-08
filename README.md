[comment]: # (# List of development priorities)

# Code organization

## -DetergentDoser.ino

### -Detergent.h

* Detergent.cpp

### -Washer.h

* Washer.cpp

### -System.h

* System.cpp


# Development environment setup

  * Install Arduino IDE (v1.8.12 used for development)

  * Install required Arduino IDE Lbraries:

    -[Adafruit MCP23017 Arduino Library](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library) (Use [Arduino Library Manager](https://www.arduino.cc/en/guide/libraries))

  * Install Arduino IDE board support for Teensy 4.1:

    -[The Teensy Loader Application](https://www.pjrc.com/teensy/loader.html)

  * Either clone repo with git or download zip

  * Install Git

    -[Git installers](https://git-scm.com/downloads)

    -Create a folder in the Arduino Sketch folder labled "DetergentDoser"

    -Navigate to the ~/Ardino/DetergentDoser folder in your Command Line Interpreter

    -Clone repository with command "git clone https://github.com/ScarceSam/DetergentDoser.git

  * Download Zip

    -[DetergentDoser-master.zip](https://github.com/ScarceSam/DetergentDoser/archive/master.zip)

    -Unzip file into Arduino Sketch folder

    -Remove "-master" from folder name

  * DetergentDoser.ino can now be opened with Arduino IDE and loaded onto an Artemis ATP

# Hardware components

  * PJRC Teensy 4.1 - https://www.pjrc.com/store/teensy41.html

  * 8 of 16-Bit I/O Expander with I2C Interface - MCP23017


# Hardware setup

## Teensy 4.1
Pin|Use
---------|----------
0|- Display RS
1|- Display E
2|- Display D4
3|- Display D5
4|- Display D6
5|- Display D7
6|- Display R cathode
7|- Display G cathode
8|- Display B cathode
18|- All GPIO Expanders SDA
19|- All GPIO Expanders SLC
24|- Feedline Pump coil A
25|- Feedline Pump coil B
26|- Feedline Line-End Drain Valve
27|- Feedline Water Valve
28|- Feedline Manifold Drain Valve
29|- Detergent 1 Valve
30|- Detergent 2 Valve
31|- Detergent 3 Valve
32|- Detergent 4 Valve
33|- Detergent 1 Flow sensor
34|- Detergent 2 Flow sensor
35|- Detergent 3 Flow sensor
36|- Detergent 4 Flow sensor
37|- Feedline Flow sensor
38|- Detergent 1 Level sensor
39|- Detergent 2 Level sensor
40|- Detergent 3 Level sensor
41|- Detergent 4 Level sensor


## I/O Expanders
(W= Washer, R= Relay, VA- Valve)

Pin|Address 0x20|Address 0x21|Address 0x22|Address 0x23|Address 0x24|Address 0x25|Address 0x26|Address 0x27
---------|----------|----------|----------|----------|----------|----------|----------|----------
PA0|W1 R1|W4 R1|W7 R1|W10 R1|W13 R1|W16 R1|W19 R1|W22 R1
PA1|W1 R2|W4 R2|W7 R2|W10 R2|W13 R2|W16 R2|W19 R2|W22 R2
PA2|W1 R3|W4 R3|W7 R3|W10 R3|W13 R3|W16 R3|W19 R3|W22 R3
PA3|W1 R4|W4 R4|W7 R4|W10 R4|W13 R4|W16 R4|W19 R4|W22 R4
PA4|W1 VA|W4 VA|W7 VA|W10 VA|W13 VA|W16 VA|W19 VA|W22 VA
PA5|W2 R1|W5 R1|W8 R1|W11 R1|W14 R1|W17 R1|W20 R1|W23 R1
PA6|W2 R2|W5 R2|W8 R2|W11 R2|W14 R2|W17 R2|W20 R2|W23 R2
PA7|W2 R3|W5 R3|W8 R3|W11 R3|W14 R3|W17 R3|W20 R3|W23 R3
PB0|W2 R4|W5 R4|W8 R4|W11 R4|W14 R4|W17 R4|W20 R4|W23 R4
PB1|W2 VA|W5 VA|W8 VA|W11 VA|W14 VA|W17 VA|W20 VA|W23 VA
PB2|W3 R1|W6 R1|W9 R1|W12 R1|W15 R1|W18 R1|W21 R1|W24 R1
PB3|W3 R2|W6 R2|W9 R2|W12 R2|W15 R2|W18 R2|W21 R2|W24 R2
PB4|W3 R3|W6 R3|W9 R3|W12 R3|W15 R3|W18 R3|W21 R3|W24 R3
PB5|W3 R4|W6 R4|W9 R4|W12 R4|W15 R4|W18 R4|W21 R4|W24 R4
PB6|W3 VA|W6 VA|W9 VA|W12 VA|W15 VA|W18 VA|W21 VA|W24 VA
PB7|

[comment]: # (# Glossary of project terms)

# Coding style guidelines

Barr Group: Embedded C Coding standard (BARR-C 2018)

[comment]: # (#Tools to be used for source control, builds, integration, testing, and deployment)

[comment]: # (# High-level organization: projects, components, file locations, and naming conventions)
