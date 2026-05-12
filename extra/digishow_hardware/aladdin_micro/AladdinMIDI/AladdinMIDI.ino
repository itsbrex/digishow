/**
 * @file AladdinMIDI.ino
 * @brief MIDI to Arduino IO Mapping for Aladdin Micro
 * 
 * This project is designed for Aladdin Micro (compatible with Arduino Micro or Leonardo).
 * It enables the board to:
 * 1. Receive MIDI CC messages and map them to PWM outputs (Y0 - Y5).
 * 2. Read changes in digital inputs (X0 - X7) and generate corresponding MIDI messages (Note On/Off or CC).
 */

// Uncomment this line to enable debug output via Serial
// #define DEBUG

// Define the mode for digital inputs: NOTE or CC
#define NOTE 0
#define CC 1
#define DIGITAL_INPUT_MODE NOTE

#include <MIDIUSB.h>

// Define PWM pin array (Y0 - Y5)
const int pwmPins[] = {9, 11, 6, 10, 5, 13};
const int numPwmPins = sizeof(pwmPins) / sizeof(pwmPins[0]);

// Corresponding MIDI CC controller numbers (20-25)
const int ccNumbers[] = {20, 21, 22, 23, 24, 25};

// Define digital input pin array (X0 - X7)
const int digitalPins[] = {23, 22, 21, 20, 4, 12, 8, 7};
const int numDigitalPins = sizeof(digitalPins) / sizeof(digitalPins[0]);

// Corresponding MIDI notes (C3, D3, E3, F3, G3, A3, B3, C4)
const int midiNotes[] = {60, 62, 64, 65, 67, 69, 71, 72};

// Corresponding MIDI CC numbers for digital inputs
const int digitalCcNumbers[] = {20, 21, 22, 23, 24, 25, 26, 27};

// Store current digital pin states
int pinStates[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

void setup() {
  // Initialize all PWM pins as output mode
  for (int i = 0; i < numPwmPins; i++) {
    pinMode(pwmPins[i], OUTPUT);
    analogWrite(pwmPins[i], 0); // Initialize to 0
  }

  // Initialize all digital pins as input
  for (int i = 0; i < numDigitalPins; i++) {
    pinMode(digitalPins[i], INPUT);
  }
  
#ifdef DEBUG
  // Initialize serial for debugging
  Serial.begin(115200);
  Serial.println("Aladdin MIDI Controller");
  Serial.println("Waiting for MIDI messages or pin changes...");
#endif
}

void loop() {
  // Check for MIDI messages
  midiEventPacket_t rx = MidiUSB.read();
  
  // If a controller change message (CC) is received
  if (rx.header != 0) {
    processMidiPacket(rx);
  }

  // Check for digital input changes
  checkDigitalInputs();
}

void processMidiPacket(midiEventPacket_t p) {
  // Check if it's a controller change message (0xB)
  if (p.byte1 == 0xB0) { // 0xB0 = Controller change message, channel 1
    byte controlNumber = p.byte2;
    byte controlValue = p.byte3;
    
    // Find the corresponding PWM pin
    for (int i = 0; i < numPwmPins; i++) {
      if (controlNumber == ccNumbers[i]) {
        // Map MIDI value (0-127) to PWM value (0-255)
        int pwmValue = map(controlValue, 0, 127, 0, 255);
        analogWrite(pwmPins[i], pwmValue);
        
#ifdef DEBUG
        // Print debug information
        Serial.print("CC #");
        Serial.print(controlNumber);
        Serial.print(" = ");
        Serial.print(controlValue);
        Serial.print(" -> PWM Pin ");
        Serial.print(pwmPins[i]);
        Serial.print(" = ");
        Serial.println(pwmValue);
#endif
        break;
      }
    }
  }
}

void checkDigitalInputs() {
  for (int i = 0; i < numDigitalPins; i++) {
    int currentState = digitalRead(digitalPins[i]);
    if (currentState != pinStates[i]) {
      pinStates[i] = currentState;

#if (DIGITAL_INPUT_MODE == NOTE)
      if (currentState == HIGH) {
        // Send Note On message
        noteOn(0, midiNotes[i], 127);
        MidiUSB.flush();
#ifdef DEBUG
        Serial.print("Pin ");
        Serial.print(digitalPins[i]);
        Serial.print(" -> Note On: ");
        Serial.println(midiNotes[i]);
#endif
      } else {
        // Send Note Off message
        noteOff(0, midiNotes[i], 0);
        MidiUSB.flush();
#ifdef DEBUG
        Serial.print("Pin ");
        Serial.print(digitalPins[i]);
        Serial.print(" -> Note Off: ");
        Serial.println(midiNotes[i]);
#endif
      }
#elif (DIGITAL_INPUT_MODE == CC)
      if (currentState == HIGH) {
        // Send CC message with value 127
        controlChange(0, digitalCcNumbers[i], 127);
        MidiUSB.flush();
#ifdef DEBUG
        Serial.print("Pin ");
        Serial.print(digitalPins[i]);
        Serial.print(" -> CC On: ");
        Serial.println(digitalCcNumbers[i]);
#endif
      } else {
        // Send CC message with value 0
        controlChange(0, digitalCcNumbers[i], 0);
        MidiUSB.flush();
#ifdef DEBUG
        Serial.print("Pin ");
        Serial.print(digitalPins[i]);
        Serial.print(" -> CC Off: ");
        Serial.println(digitalCcNumbers[i]);
#endif
      }
#endif
    }
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}