/*
 * CC2PWM - MIDI CC to PWM Converter
 * 
 * This project is for Arduino Micro or Leonardo, receiving MIDI CC messages and converting them to PWM output
 * PWM pins (3, 5, 6, 9, 10, 11, 13) correspond to MIDI CC 20-26
 */

// Uncomment this line to enable debug output via Serial
// #define DEBUG

#include <MIDIUSB.h>

// Define PWM pin array
const int pwmPins[] = {3, 5, 6, 9, 10, 11, 13};
const int numPins = sizeof(pwmPins) / sizeof(pwmPins[0]);

// Corresponding MIDI CC controller numbers (20-26)
const int ccNumbers[] = {20, 21, 22, 23, 24, 25, 26};

// Store current PWM values
byte pwmValues[7] = {0, 0, 0, 0, 0, 0, 0};

void setup() {
  // Initialize all PWM pins as output mode
  for (int i = 0; i < numPins; i++) {
    pinMode(pwmPins[i], OUTPUT);
    analogWrite(pwmPins[i], 0); // Initialize to 0
  }
  
#ifdef DEBUG
  // Initialize serial for debugging
  Serial.begin(115200);
  Serial.println("CC2PWM - MIDI CC to PWM Converter");
  Serial.println("Waiting for MIDI CC messages...");
#endif
}

void loop() {
  // Check for MIDI messages
  midiEventPacket_t rx = MidiUSB.read();
  
  // If a controller change message (CC) is received
  if (rx.header != 0) {
    processMidiPacket(rx);
  }
}

void processMidiPacket(midiEventPacket_t p) {
  // Check if it's a controller change message (0xB)
  if (p.byte1 == 0xB0) { // 0xB0 = Controller change message, channel 1
    byte controlNumber = p.byte2;
    byte controlValue = p.byte3;
    
    // Find the corresponding PWM pin
    for (int i = 0; i < numPins; i++) {
      if (controlNumber == ccNumbers[i]) {
        // Map MIDI value (0-127) to PWM value (0-255)
        int pwmValue = map(controlValue, 0, 127, 0, 255);
        pwmValues[i] = pwmValue;
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