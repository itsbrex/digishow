# AladdinMIDI – MIDI to Arduino IO Mapping

This project is designed for the **Aladdin Micro** board (fully compatible with **Arduino Micro** or **Leonardo**). It processes MIDI messages, controls PWM outputs, and reads digital inputs to send MIDI messages.

---

## Features

- Receives MIDI Control Change (CC) messages via USB to control PWM outputs
- Reads digital input pin state changes and sends **MIDI Note On/Off** or **Control Change (CC)** messages
- Compile-time macro to configure digital input mode (`NOTE` or `CC`)
- Automatically maps MIDI CC values (0–127) to PWM output values (0–255)
- Supports **6 PWM output pins** (Y0–Y5) and **8 digital input pins** (X0–X7)
- Optional serial debug output

---

## Hardware Requirements

- **Main Board**: **Aladdin Micro** or Arduino Micro / Leonardo (requires native USB support)
- **Output Devices** (optional): LEDs or other PWM‑controlled devices
- **Input Devices** (optional): Push buttons, switches, or NPN industrial sensors (connect to X0–X7)

---

## Pin Mapping

### MIDI CC → PWM Output (Y0–Y5)

| Aladdin Micro Output Port | Corresponding Arduino Pin | MIDI CC Number |
|---------------------------|---------------------------|----------------|
| Y0                        | 9                         | 20             |
| Y1                        | 11                        | 21             |
| Y2                        | 6                         | 22             |
| Y3                        | 10                        | 23             |
| Y4                        | 5                         | 24             |
| Y5                        | 13                        | 25             |

### Digital Input → MIDI Output (X0–X7)

Digital inputs can be configured to send either **MIDI Note messages** or **MIDI CC messages**.

| Aladdin Micro Input Port | Corresponding Arduino Pin | Note Mode (MIDI Note Number) | CC Mode (CC Number) |
|--------------------------|---------------------------|------------------------------|---------------------|
| X0                       | 23 (A5)                   | C4  (60)                     | CC 20               |
| X1                       | 22 (A4)                   | D4  (62)                     | CC 21               |
| X2                       | 21 (A3)                   | E4  (64)                     | CC 22               |
| X3                       | 20 (A2)                   | F4  (65)                     | CC 23               |
| X4                       | 4                         | G4  (67)                     | CC 24               |
| X5                       | 12                        | A4  (69)                     | CC 25               |
| X6                       | 8                         | B4  (71)                     | CC 26               |
| X7                       | 7                         | C5  (72)                     | CC 27               |

---

## Usage

### 1. Configure Operating Mode
- Open `AladdinMIDI.ino` in the Arduino IDE
- Change the `DIGITAL_INPUT_MODE` macro to `NOTE` or `CC` to select the behavior of the digital inputs

### 2. Upload the Sketch
- Upload the program to your Arduino Micro / Leonardo or **Aladdin Micro**

### 3. Connect the Board
- Connect the board to your computer via USB
- The computer should automatically recognize it as a **MIDI device**

### 4. Start Controlling
- Send MIDI CC messages (numbers 20–25) to control the corresponding PWM outputs (Y0–Y5)
- Connect switches, sensors, etc. to the X0–X7 input ports (supports NPN or dry contact)
  - When a pin changes from **LOW → HIGH**: sends **Note On** or CC value **127**
  - When a pin changes from **HIGH → LOW**: sends **Note Off** or CC value **0**

---

## Debugging

To enable serial debug output:

1. Uncomment the line `#define DEBUG` at the top of the `AladdinMIDI.ino` file
2. Re-upload the sketch
3. Open the **Serial Monitor** in the Arduino IDE and set the baud rate to **115200**

---

## Dependency Library

- **MIDIUSB**  
  Can be installed directly via the Arduino Library Manager (search for `MIDIUSB`)

---

## License

This project is licensed under the **MIT License**.

