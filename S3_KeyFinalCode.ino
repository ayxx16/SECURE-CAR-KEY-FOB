#include <RCSwitch.h>

#define TX_PIN 26
#define LOCK_BTN 33
#define UNLOCK_BTN 32

#define ID 0xA
#define SECRET_KEY 0x2A5

RCSwitch rc;

unsigned int counter = 0;

unsigned int f(unsigned int x) {
  unsigned int base = (x * x + 3 * x + 7);
  return (base ^ SECRET_KEY) & 0x1FF;
}

void setup() {
  Serial.begin(115200);

  pinMode(LOCK_BTN, INPUT_PULLUP);
  pinMode(UNLOCK_BTN, INPUT_PULLUP);

  rc.enableTransmit(TX_PIN);
  rc.setPulseLength(350);
  rc.setRepeatTransmit(3);
}

void loop() {

  if (digitalRead(UNLOCK_BTN) == LOW) {

    counter = (counter + 1) % 64;

    unsigned int result = f(counter);

    unsigned long msg =
      ((unsigned long)ID << 17) |
      ((unsigned long)1 << 15) |
      ((unsigned long)counter << 9) |
      result;

    rc.send(msg, 21);
    Serial.println("UNLOCK SENT");
    delay(500);
  }

  else if (digitalRead(LOCK_BTN) == LOW) {

    counter = (counter + 1) % 64;

    unsigned int result = f(counter);

    unsigned long msg =
      ((unsigned long)ID << 17) |
      ((unsigned long)2 << 15) |
      ((unsigned long)counter << 9) |
      result;

    rc.send(msg, 21);
    Serial.println("LOCK SENT");
    delay(500);
  }
}