[comment]: # (# List of development priorities)


# Development environment setup

  * Install Arduino IDE (v1.8.12 used for development)

  * Install required Arduino IDE Lbraries:

    -SX1509 IO Expander v2.0.1 by SparkFun
    -Wire library v1.0 for Teensy
    -SD library v1.2.2 for Teensy
    -SPI library v1.0 for Teensy
    -LiquidCrystalFast v1.1 for Teensy

  * Install Arduino IDE board support for Teensy 4.1:

    -[The Teensy Loader Application](https://www.pjrc.com/teensy/loader.html)

  * Either clone repo with git or download zip

  * Install Git

    -[Git installers](https://git-scm.com/downloads)

    -Create a folder in the Arduino Sketch folder labled "ProductDoser"

    -Navigate to the ~/Ardino/ProductDoser folder in your Command Line Interpreter

    -Clone repository with command "git clone https://github.com/ScarceSam/ProductDoser.git

  * Download Zip

    -[ProductDoser-master.zip](https://github.com/ScarceSam/ProductDoser/archive/master.zip)

    -Unzip file into Arduino Sketch folder

    -Remove "-master" from folder name

  * ProductDoser.ino can now be opened with Arduino IDE and loaded onto an Artemis ATP

# Hardware components

  * Product doser board V2.01
    -The board needs bodge wire soldered to the topside of R400/C400 and connected to terminal labled spr.1
    -The V2.01 board has CAN BUS hardware to comunicate with a Satelite board. The CAN BUS code is missing. 


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
6|- Display R cathode(Not Used)
7|- Display G cathode
8|- Display B cathode
14|- Detergent 4 Level sensor
15|- Detergent 3 Level sensor
16|- Detergent 2 Level sensor
17|- Detergent 1 Level sensor
18|- SX1509 GPIO Expander SDA
19|- SX1509 GPIO Expander SLC
20|- SX1509 GPIO Expander Reset
21|- TCAN1051 S
22|- TCAN1051 RXD
23|- TCAN1051 TXD
24|- Feedline Line-End Drain Valve
25|- Feedline Water Valve
26|- Feedline Manifold Drain Valve
29|- Detergent 1 pump
30|- Detergent 2 pump
31|- Detergent 3 pump
32|- Detergent 4 pump
33|- Neo Pixel
37|- Feedline Flow sensor
40|- SX1509 GPIO Expander interrupt


## I/O Expander for buttons

Pin|Button
---------|----------
GND|Common
1|Return
2|Up
4|Left
5|Select
6|Right
8|Down

[comment]: # (# Glossary of project terms)

# Coding style guidelines

Barr Group: Embedded C Coding standard (BARR-C 2018)

[comment]: # (#Tools to be used for source control, builds, integration, testing, and deployment)

[comment]: # (# High-level organization: projects, components, file locations, and naming conventions)
