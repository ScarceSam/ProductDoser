

# List of development priorities

# Code organization

## -DetergentDoser.ino

### -Detergent.h

* Detergent.cpp

### -Washer.h

* Washer.cpp

### -System.h

* System.cpp


# Development environment

  * Arduino IDE v1.8.12

  * Download Repo

  * Unzip into Arduino Sketch folder

  * File > Preferences > Additional Boards Manager URLs:
	https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json

# Hardware components

  * SparkFun RedBoard Artemis ATP - https://www.sparkfun.com/products/15442

  * 8 of 16-Bit I/O Expander with I2C Interface - MCP23017


# Hardware setup

## Artemis
Pin|Use
---------|----------
SCL & SDA|- I2C Buss
D5(SCK)|-
D6(MISO)|-
D7(MOSI)|-
A13|-
A12|-
A32|-
28|-
27|-
23|-
22|-
4|-
A35|-
24(TX1)|-
25(RX1)|-
14|-
8|-
10|-
9|-
26|-
15|-
19|-
18|-
3|-
36|-
38|-
37|-
44|-
42|-
43|-
0|- Detergetn 4 Valve
1|- Detergetn 3 Valve
2|- Detergetn 2 Valve
45|- Detergetn 1 Valve
41|- Detergent 4 Flow sensor
17|- Detergent 3 Flow sensor
A31|- Detergent 2 Flow sensor
A16|- Detergent 1 Flow sensor
A33|- Detergent 4 Level sensor
A34|- Detergent 3 Level sensor
A11|- Detergent 2 Level sensor
A29|- Detergent 1 Level sensor

## I/O Expander
(W= Washer, R= Relay, VA- Valve)

Pin|Address 0x20|Address 0x21|Address 0x22|Address 0x23|Address 0x24|Address 0x25|Address 0x26|Address 0x27
---------|----------|----------|----------|----------|----------|----------|----------|
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

# Glossary of project terms

# Coding style guidelines

Barr Group: Embedded C Coding standard (BARR-C 2018)

[comment]: # (#Tools to be used for source control, builds, integration, testing, and deployment)

[comment]: # (# High-level organization: projects, components, file locations, and naming conventions)
