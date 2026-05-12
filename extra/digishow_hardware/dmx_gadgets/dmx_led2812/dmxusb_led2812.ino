#include <DMXUSB.h>
#include <Adafruit_NeoPixel.h>

#define DMXUSB_BAUDRATE 115200
#define LED_PIN         A3     // Data pin connected to the WS2812 LED strip
#define LED_COUNT       120    // Number of LEDs on the WS2812 strip
#define BRIGHTNESS      255    // LED brightness (0-255)

// Declare NeoPixel object
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// receive a DMX frame and map 510 channels to 170 RGB LED pixels
void dmxToLed(int universe, char buffer[512]) {
  
  // Every 3 DMX channels correspond to one RGB LED
  for (int i = 0; i < LED_COUNT; i++) {
    int baseChannel = i * 3; // Starting channel for each LED
    
    // Ensure it does not exceed the DMX channel range
    if (baseChannel + 2 < 512) {
      uint8_t r = buffer[baseChannel];     // Red channel
      uint8_t g = buffer[baseChannel + 1]; // Green channel
      uint8_t b = buffer[baseChannel + 2]; // Blue channel
      
      // Set the color of the corresponding LED
      strip.setPixelColor(i, strip.Color(r, g, b));
    }
  }
  
  // Update the LED strip display
  strip.show();
}

DMXUSB dmx(
  Serial,
  DMXUSB_BAUDRATE,
  0,
  dmxToLed // void (*dmxInCallback)(int universe, char buffer[512])
);

void setup() {

  // usb serial port for receving DMX incoming frames
  Serial.begin(DMXUSB_BAUDRATE);

  // Initialize the NeoPixel strip
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  dmx.listen();
}
