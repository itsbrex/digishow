# DMX USB to WS2812 LED Controller

This Arduino project acts as a DMX USB interface that receives DMX frames over a USB serial connection and directly maps the channels to a WS2812 (NeoPixel) addressable LED strip.

## Features
* Receives DMX512 data via USB serial (Baudrate: 115200).
* Maps up to 510 DMX channels to 170 RGB LEDs (3 channels per LED: Red, Green, Blue).
* Uses the Adafruit NeoPixel library for LED control.

## Dependencies
To compile and run this project, you will need to install the following Arduino libraries via the Library Manager:
* **DMXUSB** (for handling the DMX USB Pro protocol)
* **Adafruit NeoPixel** (for WS2812 LED strip control)

## Hardware Setup
1. Connect your WS2812 LED strip's **Data IN** pin to the Arduino's **A3** pin.
2. Ensure your LED strip is powered correctly (usually requires an external 5V power supply, as the Arduino cannot provide enough current for many LEDs).
3. Connect the Arduino to your computer via USB.

### Configuration
You can easily adjust the settings at the top of the `dmxusb_led2812.ino` file:
* `LED_PIN`: The data pin connected to the WS2812 strip (Default: `A3`).
* `LED_COUNT`: The total number of LEDs on your strip (Default: `120`).
* `BRIGHTNESS`: The overall brightness level from 0 to 255 (Default: `255`).
* `DMXUSB_BAUDRATE`: The serial communication baud rate (Default: `115200`).

## How it Works
Every 3 DMX channels correspond to one RGB LED:
- Channel 1: LED 1 Red
- Channel 2: LED 1 Green
- Channel 3: LED 1 Blue
- Channel 4: LED 2 Red
- ...and so on.

The sketch listens for incoming DMX frames in the main loop and automatically updates the NeoPixel strip colors in real-time.
