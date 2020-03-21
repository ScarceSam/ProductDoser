

# List of development priorities

# Code organization

  * main.c

  * washer.h

  * pump.h

  * detergent.h

  * log.h

  * display.h

  * flowmeter.h


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

  * Washer output relays
    * Input to port expander w/pullup
  * Washer valves
    * Outputs from port expanders

# Pinouts
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


# Glossary of project terms

# Coding style guidelines

Barr Group: Embedded C Coding standard (BARR-C 2018)

[comment]: # (#Tools to be used for source control, builds, integration, testing, and deployment)

[comment]: # (# High-level organization: projects, components, file locations, and naming conventions)
