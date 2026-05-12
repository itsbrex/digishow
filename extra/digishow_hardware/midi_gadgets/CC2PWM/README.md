# CC2PWM - MIDI CC to PWM Converter

Arduino project for Micro/Leonardo. Converts MIDI CC messages to PWM output.

**Features:**
- USB MIDI input
- Maps CC 0-127 to PWM 0-255
- 7 PWM pins: 3,5,6,9,10,11,13

**Pin Mapping:**
- Pin 3→CC20, 5→CC21, 6→CC22, 9→CC23, 10→CC24, 11→CC25, 13→CC26

**Quick Start:**
Upload code → Connect USB → Send CC20-26 → Get PWM output
