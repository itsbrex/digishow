#include <MIDIUSB.h>
#include <WS2812FX.h>

#define LED_COUNT 180 //30
#define LED_PIN A3

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// WS2812FX parameters
uint8_t  fxBrightness = 30;
uint16_t fxSpeed = 1000; // 0 - 3000
uint8_t  fxColorR = 0x00;
uint8_t  fxColorG = 0x7B;
uint8_t  fxColorB = 0xFF;
uint8_t  fxMode = FX_MODE_TWINKLEFOX; //FX_MODE_STATIC

void setup() {
  // initialize ws2812fx
  ws2812fx.init();
  ws2812fx.setBrightness(fxBrightness);
  ws2812fx.setSpeed(fxSpeed);
  ws2812fx.setColor(fxColorR, fxColorG, fxColorB);
  ws2812fx.setMode(fxMode);
  ws2812fx.start();
}

void loop() {
  
  // usb midi listener
  midiEventPacket_t midiEvent;
  do {
    midiEvent = MidiUSB.read();
    midiToLedFx(midiEvent);
  } while (midiEvent.header != 0);
  
  // led fx handler
  ws2812fx.service();
}

void midiToLedFx(midiEventPacket_t midiEvent) {
  if (midiEvent.header == 0x0B) {
    // is midi cc event
    int midiChannel = midiEvent.byte1 & 0x0F; // 0 ~ 15
    int midiControl = midiEvent.byte2;        // 0 ~ 127
    int midiCCValue = midiEvent.byte3;        // 0 ~ 127

    switch (midiControl) {
    case 20: // CC20 -> brightness
      fxBrightness = map(midiCCValue, 0, 127, 0, 255); 
      ws2812fx.setBrightness(fxBrightness == 0 ? 1 : fxBrightness);
      break;
    case 21: // CC21 -> speed
      fxSpeed = map(midiCCValue, 0, 127, 0, 3000); 
      ws2812fx.setSpeed(fxSpeed); 
      break;
    case 22: // CC22 -> red
      fxColorR = map(midiCCValue, 0, 127, 0, 255); 
      ws2812fx.setColor(fxColorR, fxColorG, fxColorB); 
      break;
    case 23: // CC23 -> green
      fxColorG = map(midiCCValue, 0, 127, 0, 255); 
      ws2812fx.setColor(fxColorR, fxColorG, fxColorB); 
      break;
    case 24: // CC24 -> blue
      fxColorB = map(midiCCValue, 0, 127, 0, 255); 
      ws2812fx.setColor(fxColorR, fxColorG, fxColorB); 
      break;
    case 25: // CC25 -> mode
      fxMode = (midiCCValue >= MODE_COUNT ? FX_MODE_TWINKLEFOX : midiCCValue);
      ws2812fx.setMode(fxMode);
      break;
    }
  }
}

