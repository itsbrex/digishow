# CC2LED (MIDI CC to WS2812 LED Controller)

This is an Arduino-based MIDI interactive LED strip controller program. It receives MIDI Control Change (CC) signals via USB and controls the brightness, speed, color, and display modes of WS2812 (NeoPixel) LED strips in real-time.

This program is perfect for music performances, interactive art installations, or synchronizing physical lighting effects through DAWs (like Ableton Live, Logic Pro, etc.) and interactive control software (like DigiShow).

## 🛠 Hardware Requirements

*   **Development Board**: An Arduino board with native USB support and compatible with the `MIDIUSB` library (e.g., Arduino Leonardo, Micro, Pro Micro, Zero, Due, etc.).
*   **LED Strip**: WS2812B / NeoPixel or compatible RGB LED strip.

## 📦 Software Dependencies

Before compiling and uploading the code, please ensure the following libraries are installed in your Arduino IDE:

1.  **MIDIUSB**: For receiving and sending MIDI data over USB.
2.  **WS2812FX**: For providing rich dynamic effects for WS2812 LED strips.

## ⚙️ Pin and Parameter Configuration

You can modify the following parameters at the top of the `CC2LED.ino` file to suit your hardware:

*   `LED_COUNT`: Number of LEDs (Default: `180`)
*   `LED_PIN`: LED data control pin (Default: `A3`)

*If your WS2812 strip does not use GRB color order or an 800 KHz frequency, you can also modify the initialization line of the `ws2812fx` object.*

## 🎛 MIDI CC Control Mapping

The program listens for Control Change (CC) messages arriving on the USB port. By sending specific CC numbers and values (0-127), you can dynamically change the state of the LED strip:

| CC Number | Control Parameter | Value Range (MIDI 0-127) | Mapped Range / Effect |
| :--- | :--- | :--- | :--- |
| **CC 20** | Brightness | 0 ~ 127 | Mapped to brightness value 1 ~ 255 |
| **CC 21** | Speed | 0 ~ 127 | Mapped to animation speed 0 ~ 3000 |
| **CC 22** | Red | 0 ~ 127 | Mapped to Red channel value 0 ~ 255 |
| **CC 23** | Green | 0 ~ 127 | Mapped to Green channel value 0 ~ 255 |
| **CC 24** | Blue | 0 ~ 127 | Mapped to Blue channel value 0 ~ 255 |
| **CC 25** | Animation Mode | 0 ~ 127 | Switches between various built-in animation modes in the WS2812FX library |

> **Note**: Upon startup, the default color is sky blue (R:0, G:123, B:255), and the default animation mode is `FX_MODE_TWINKLEFOX`.

## 🚀 Usage Instructions

1. Connect the data line of your WS2812 LED strip to the `A3` pin on your development board according to the pin configuration (please note common ground and independent power supply for the strip).
2. Connect the Arduino board to your computer.
3. Open `CC2LED.ino` in the Arduino IDE.
4. Ensure the `MIDIUSB` and `WS2812FX` libraries are installed.
5. Compile and upload the code to the board.
6. Once uploaded successfully, your computer will recognize the board as a USB MIDI device.
7. Open your MIDI host software or DigiShow, and select this development board as the MIDI output target.
8. Trigger or send CC20 ~ CC25 signals to see the LED strip react in real-time!
