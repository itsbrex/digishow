#include <DMXUSB.h>
#include <DMXSerial.h>
#include <MIDIUSB.h>

#define DMXUSB_BAUDRATE 115200

void usbMidiToDmx(midiEventPacket_t midiEvent) {
  if (midiEvent.header == 0x0B) {
    // is midi cc event
    int midiChannel = midiEvent.byte1 & 0x0F; // 0 ~ 15
    int midiControl = midiEvent.byte2;        // 0 ~ 127
    int midiCCValue = midiEvent.byte3;        // 0 ~ 127

    int dmxChannel = midiChannel * 64 + (midiControl % 64) + 1; 
    if (dmxChannel > 512) dmxChannel = 512;   // 1 ~ 512
    int dmxValue = midiCCValue * 2;           // 0 ~ 255

    DMXSerial.write(dmxChannel, dmxValue);
  }
}

void usbSerialToDmx(int universe, char buffer[512]) {  
  for (int index=0; index < 512; index++) {
    int channel = index + 1;
    int value = buffer[index];
    DMXSerial.write(channel, value);
  }
}

DMXUSB DmxUSB(
  Serial,
  DMXUSB_BAUDRATE,
  0,
  usbSerialToDmx // void (*dmxInCallback)(int universe, unsigned int index, char buffer[512])
);

void setup() {
  
  // usb serial port
  Serial.begin(DMXUSB_BAUDRATE);
    
  // dmx serial port
  DMXSerial.init(DMXController);
  DMXSerial.maxChannel(512);
}

void loop() {
  // usb serial to dmx
  DmxUSB.listen();

  // usb midi to dmx
  midiEventPacket_t midiEvent;
  do {
    midiEvent = MidiUSB.read();
    usbMidiToDmx(midiEvent);
  } while (midiEvent.header != 0);
}
