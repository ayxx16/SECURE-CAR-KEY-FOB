#include <RCSwitch.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RX_PIN 27
#define TX_PIN 26

#define CAPTURE_BTN 32
#define SIGNAL1_BTN 33
#define SIGNAL2_BTN 25
#define CLEAR_BTN 14

LiquidCrystal_I2C lcd(0x27, 16, 2);
RCSwitch rc;

unsigned long lastCapturedValue = 0;
unsigned int lastCapturedBitlen = 0;
unsigned int lastCapturedProtocol = 0;

unsigned long signal1Value = 0;
unsigned int signal1Bitlen = 0;
unsigned int signal1Protocol = 0;

unsigned long signal2Value = 0;
unsigned int signal2Bitlen = 0;
unsigned int signal2Protocol = 0;

bool lastCap = HIGH;
bool lastS1 = HIGH;
bool lastS2 = HIGH;
bool lastClear = HIGH;

void showMessage(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void setup() {
  Serial.begin(115200);

  pinMode(CAPTURE_BTN, INPUT_PULLUP);
  pinMode(SIGNAL1_BTN, INPUT_PULLUP);
  pinMode(SIGNAL2_BTN, INPUT_PULLUP);
  pinMode(CLEAR_BTN, INPUT_PULLUP);

  rc.enableReceive(digitalPinToInterrupt(RX_PIN));
  rc.enableTransmit(TX_PIN);
  rc.setPulseLength(350);
  rc.setRepeatTransmit(10);

  lcd.init();
  lcd.backlight();

  showMessage("Replay Attack", "Ready");
  Serial.println("REPLAY ATTACK READY");
}

void loop() {
  bool capNow = digitalRead(CAPTURE_BTN);
  bool s1Now = digitalRead(SIGNAL1_BTN);
  bool s2Now = digitalRead(SIGNAL2_BTN);
  bool clearNow = digitalRead(CLEAR_BTN);

  if (capNow == LOW && lastCap == HIGH) {
    showMessage("Capturing...", "Waiting signal");
    Serial.println("CAPTURE pressed");

    unsigned long start = millis();
    bool captured = false;

    while (millis() - start < 5000) {
      if (rc.available()) {
        unsigned long v = rc.getReceivedValue();

        if (v != 0) {
          lastCapturedValue = v;
          lastCapturedBitlen = rc.getReceivedBitlength();
          lastCapturedProtocol = rc.getReceivedProtocol();

          Serial.print("Captured value: ");
          Serial.println(lastCapturedValue);
          Serial.print("Bit length: ");
          Serial.println(lastCapturedBitlen);
          Serial.print("Protocol: ");
          Serial.println(lastCapturedProtocol);

          showMessage("Signal Captured", "Choose S1 or S2");
          captured = true;
        } else {
          Serial.println("Invalid signal (0)");
          showMessage("Invalid Signal", "Try again");
        }

        rc.resetAvailable();
        break;
      }
    }

    if (!captured) {
      showMessage("No Signal", "Press Capture");
      Serial.println("No signal captured");
    }
  }

  if (s1Now == LOW && lastS1 == HIGH) {
    if (signal1Value == 0) {
      if (lastCapturedValue == 0) {
        showMessage("No Capture Yet", "Press Capture");
        Serial.println("No captured signal to save in S1");
      } else {
        signal1Value = lastCapturedValue;
        signal1Bitlen = lastCapturedBitlen;
        signal1Protocol = lastCapturedProtocol;

        showMessage("Saved as S1", "Ready");
        Serial.println("Saved captured signal as S1");
      }
    } else {
      rc.setProtocol(signal1Protocol);
      rc.send(signal1Value, signal1Bitlen);

      showMessage("Replaying...", "Signal 1");
      Serial.println("Replayed S1");
      delay(800);
      showMessage("Replay Attack", "Ready");
    }
  }

  if (s2Now == LOW && lastS2 == HIGH) {
    if (signal2Value == 0) {
      if (lastCapturedValue == 0) {
        showMessage("No Capture Yet", "Press Capture");
        Serial.println("No captured signal to save in S2");
      } else {
        signal2Value = lastCapturedValue;
        signal2Bitlen = lastCapturedBitlen;
        signal2Protocol = lastCapturedProtocol;

        showMessage("Saved as S2", "Ready");
        Serial.println("Saved captured signal as S2");
      }
    } else {
      rc.setProtocol(signal2Protocol);
      rc.send(signal2Value, signal2Bitlen);

      showMessage("Replaying...", "Signal 2");
      Serial.println("Replayed S2");
      delay(800);
      showMessage("Replay Attack", "Ready");
    }
  }

  if (clearNow == LOW && lastClear == HIGH) {
    lastCapturedValue = 0;
    lastCapturedBitlen = 0;
    lastCapturedProtocol = 0;

    signal1Value = 0;
    signal1Bitlen = 0;
    signal1Protocol = 0;

    signal2Value = 0;
    signal2Bitlen = 0;
    signal2Protocol = 0;

    showMessage("All Cleared", "Ready");
    Serial.println("All stored signals cleared");
    delay(800);
    showMessage("Replay Attack", "Ready");
  }

  lastCap = capNow;
  lastS1 = s1Now;
  lastS2 = s2Now;
  lastClear = clearNow;

  delay(10);
}