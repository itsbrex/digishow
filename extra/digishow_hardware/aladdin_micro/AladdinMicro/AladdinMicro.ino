/**
 * Aladdin Micro - Pin Definitions
 * 
 * Aladdin Micro is compatible with Arduino Micro (Leonardo)
 * 
 * Pin Mapping:
 * - X0 - X7 : 8 Digital Inputs (with 24V support via onboard circuitry)
 * - Y0 - Y5 : 6 Digital Outputs with PWM (500mA, common anode)
 * - GPIO0   : A0, A1 (5V analog/digital, Grove connector)
 * - GPIO1   : SDA, SCL (I2C, Grove connector)
 * - GPIO2   : RX, TX (UART, Grove connector)
 */

// ============================================================
// X0 - X7 : Digital Input Ports
// ============================================================
#define X0 23   // A5
#define X1 22   // A4
#define X2 21   // A3
#define X3 20   // A2
#define X4 4
#define X5 12
#define X6 8
#define X7 7

const int digitalInputPins[] = { X0, X1, X2, X3, X4, X5, X6, X7 };
const int numDigitalInputs = 8;

// ============================================================
// Y0 - Y5 : Digital Output Ports with PWM
// ============================================================
#define Y0 9
#define Y1 11
#define Y2 6
#define Y3 10
#define Y4 5
#define Y5 13

const int pwmOutputPins[] = { Y0, Y1, Y2, Y3, Y4, Y5 };
const int numPwmOutputs = 6;

void setup() {

  // Configure X0 - X7 as digital inputs
  for (int i = 0; i < numDigitalInputs; i++) {
    pinMode(digitalInputPins[i], INPUT);
  }
  
  // Configure Y0 - Y5 as outputs
  for (int i = 0; i < numPwmOutputs; i++) {
    pinMode(pwmOutputPins[i], OUTPUT);
    digitalWrite(pwmOutputPins[i], LOW);  // Initialize to LOW (off)
  }
  
  // TODO: Configure more GPIO pins

}


void loop() {

  // Example
  digitalWrite(Y0, digitalRead(X0));
  delay(10);  

}