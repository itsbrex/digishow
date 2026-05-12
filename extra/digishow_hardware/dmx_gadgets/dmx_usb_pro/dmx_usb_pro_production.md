**DigiShow DMX USB Pro Production Process**

1. Install Arduino 1.8.19  
2. Install the following libraries: `elapsedMillis`, `DMXUSB`, `DMXSerial`, `MIDIUSB`  
3. Edit `DMXUSB.cpp`  

```cpp
while (!_serial->available()) {
    // If no data for 0.5 seconds, reset the state
    if (_timeout > 500) state = STATE_START;
    if (_timeout > 3000) return;
}
```

4. Edit `MIDIUSB.cpp`  

```cpp
uint8_t MIDI_::getShortName(char* name)
{
    memcpy(name, "_digishow", 9);
    return 9;
}
```

5. Edit `boards.txt` located at:  
   - `/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/boards.txt`  
   - or `/Users/robin/Library/Arduino15/packages/arduino/hardware/avr/1.8.6/boards.txt`  

Modify the following lines:  

```ini
#leonardo.build.vid=0x2341
#leonardo.build.pid=0x8036
#leonardo.build.usb_product="Arduino Leonardo"
leonardo.build.vid=0xF000
leonardo.build.pid=0x1000
leonardo.build.usb_product="DigiShow USB"
```

6. Upload the sketch `dmxusbpro_v2.ino` using the Arduino IDE.