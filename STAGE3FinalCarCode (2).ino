#include <RCSwitch.h>

#define RX_PIN 4

#define LED1_PIN 14
#define LED2_PIN 12
#define BUZZER_PIN 13

#define ID 0xA
#define SECRET_KEY 0x2A5

RCSwitch rc;

unsigned long lastActionTime = 0;
const unsigned long COOLDOWN_MS = 500;

unsigned int lastCounter = 0;
const int WINDOW = 5;

unsigned int f(unsigned int x) {
  unsigned int base = (x * x + 3 * x + 7);
  return (base ^ SECRET_KEY) & 0x1FF;
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  rc.enableReceive(digitalPinToInterrupt(RX_PIN));

  Serial.println("CAR READY (SYNC + REPLAY)");
}

void loop() {

  if (millis() - lastActionTime < COOLDOWN_MS) return;

  if (rc.available()) {

    unsigned long msg = rc.getReceivedValue();

    unsigned int recvID  = (msg >> 17) & 0xF;
    unsigned int cmd     = (msg >> 15) & 0x3;
    unsigned int counter = (msg >> 9) & 0x3F;
    unsigned int result  = msg & 0x1FF;

    if (recvID == ID) {

      bool valid = false;
      unsigned int matchedCounter = lastCounter;

      // check within window
      for (int i = 1; i <= WINDOW; i++) {

        unsigned int testCounter = lastCounter + i;
        unsigned int expected = f(testCounter);

        if (expected == result && testCounter == counter) {
          valid = true;
          matchedCounter = testCounter;
          break;
        }
      }

      if (!valid) {
        Serial.println("REJECTED (OUT OF WINDOW OR INVALID)");
        rc.resetAvailable();
        return;
      }

      //  replay protection
      if (matchedCounter <= lastCounter) {
        Serial.println("REPLAY BLOCKED");
        rc.resetAvailable();
        return;
      }

      lastCounter = matchedCounter;

      Serial.print("SYNCED TO COUNTER: ");
      Serial.println(lastCounter);

      if (cmd == 1) {

        Serial.println("UNLOCK");

        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED2_PIN, HIGH);

        digitalWrite(BUZZER_PIN, HIGH);
        delay(300);
        digitalWrite(BUZZER_PIN, LOW);

        delay(800);

        digitalWrite(LED1_PIN, LOW);
        digitalWrite(LED2_PIN, LOW);
      }

      else if (cmd == 2) {

        Serial.println("LOCK");

        for (int i = 0; i < 2; i++) {
          digitalWrite(LED1_PIN, HIGH);
          digitalWrite(LED2_PIN, HIGH);
          digitalWrite(BUZZER_PIN, HIGH);
          delay(200);

          digitalWrite(LED1_PIN, LOW);
          digitalWrite(LED2_PIN, LOW);
          digitalWrite(BUZZER_PIN, LOW);
          delay(200);
        }
      }

      Serial.println("-------------------");

      lastActionTime = millis();
    }

    rc.resetAvailable();
  }
}