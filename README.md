# TF4Micro Motion Kit for APDS

The original codes are on https://github.com/googlecreativelab/tf4micro-motion-kit .

This repo contains the Arduino Sketch for using APDS based on the original codes.

## Install and Run on Arduino

1. Install the [Arduino IDE ](https://www.arduino.cc/en/software "Arduino IDE")

1. Setup Arduino board:
    - Plug in the board
    - Install the board by navigating to Tools > Board > Boards Manager and search for  Arduino Mbed OS Nano Boards.
    - After the board is installed, select it under to Tools > Board >  Arduino Mbed OS Nano Boards > Arduino Nano 33 BLE
    - Select the port by navigating to Tools -> Port -> ... (Arduino Nano 33 BLE)

1. Install Arduino libraries 
    - Navigate to Tools > Manage Libraries
    - Search for and install:  
        - Arduino_APDS9960  
        - ArduinoBLE  
        - Arduino_TensorFlowLite

1. Open the sketch and flash
    - Download the latest release [here](https://github.com/kuromusha/tf4micro-motion-kit-apds/releases/latest)
    - Open the **arduino/tf4micro-motion-kit-apds** <folder> and double click on <tf4micro-motion-kit-apds.ino> file
    - Click the Right arrow in the top left corner to build and upload the sketch.  
    - **Warning**: This process may take a few minutes. Also, warnings may populate but the upload should still succeed in spite of them.
    - If the sketch is installed, the LED on the board should flash red and green. 

1. Go to:
    - [1-2 Finger Gesture Detection](https://github.com/kuromusha/1-2-finger-gesture-detection)

---

## How to capture your data, and train and test your model

You can capture your data, and train and test your model by using:

- [Tiny Motion Trainer for APDS](https://github.com/kuromusha/tiny-motion-trainer-apds)

---

## What are changed from the original codes

This repo uses the data from APDS:

- Proximity  
Proximity is also used to start/stop captures.
- Gesture (4 ways; up, down, left and right)
- RGB Color  
RGB Color is converted into HSV Color.



The code diffs are [here](https://github.com/kuromusha/tf4micro-motion-kit-apds/compare/cec82953a03d95b9353997d06d9455e829b0312c..master).
