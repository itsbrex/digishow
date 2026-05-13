### Aladdin Micro – Developer Board

**Aladdin Micro** is a development board with core compatibility with Arduino Micro (and Arduino Leonardo).

---

### Repackaged Digital & Analog Pins

#### X0 – X7: 8-Channel Digital Input Ports
- Each port provides three pins: **+24V / IN / GND**
- Supports connection of **NPN-type industrial sensors** or **dry contact inputs** (short IN and GND)
- Pin mapping to Arduino Micro:

| Input Port | Corresponding Arduino Micro Pin |
|------------|----------------------------------|
| X0         | 23 (A5)                          |
| X1         | 22 (A4)                          |
| X2         | 21 (A3)                          |
| X3         | 20 (A2)                          |
| X4         | 4                                |
| X5         | 12                               |
| X6         | 8                                |
| X7         | 7                                |

---

#### Y0 – Y5: 6-Channel Digital Output Ports
- Each port provides two pins: **+24V / OUT** (common anode)
- Single-channel drive capacity: **500mA**
- Supports **PWM output**
- Pin mapping to Arduino Micro:

| Output Port | Corresponding Arduino Micro Pin |
|-------------|----------------------------------|
| Y0          | 9                                |
| Y1          | 11                               |
| Y2          | 6                                |
| Y3          | 10                               |
| Y4          | 5                                |
| Y5          | 13                               |

---

### Onboard 5V GPIO Ports (Grove Connectors)

Aladdin Micro also retains **6 x 5V I/O pins**, arranged across 3 **Grove connectors**:

#### GPIO0
- Pins: **18 (A0)** , **19 (A1)**
- Can be used for:
  - 2-channel 5V digital I/O
  - 2-channel 5V analog sensor input

#### GPIO1
- Pins: **2 (SDA)** , **3 (SCL)**
- Can be used for:
  - 2-channel 5V digital I/O
  - I2C interface sensors

#### GPIO2
- Pins: **0 (RX)** , **1 (TX)**
- Can be used for:
  - 2-channel 5V digital I/O
  - External UART serial connection

---

### Software Support & Extended Applications

- **DigiShow RIOC Program**  
  After uploading via Arduino IDE, the Aladdin board can be used as an **Arduino interface device** in **DigiShow software**.

- **AladdinMIDI Program**  
  Once uploaded, the board is recognized by a computer or smartphone as a **USB MIDI device**, mapping onboard I/O to **MIDI CC** or **MIDI Note** messages.

- **Custom Arduino Sketches**  
  You can also upload your own Arduino programs, turning the Aladdin Micro into a **controller for custom applications**.

---

### Aladdin Micro + DigiShow Quick Start Guide

Aladdin Micro paired with DigiShow software allows you to convert industrial-grade I/O ports into interactive control signals — no programming required.

**Quick Setup:**

1. Connect Aladdin Micro to your computer via USB
2. Search and install **DigiShow RIOC** via Arduino IDE Library Manager
3. Open the example: `DigiShow RIOC / RiocArduino`
4. Select board type **Arduino Micro** and upload the sketch
5. In DigiShow software's Interface Manager, create a new Arduino interface, select **Aladdin Micro** as the model, and set the serial port to **Auto**

Once completed, you're ready to go.
