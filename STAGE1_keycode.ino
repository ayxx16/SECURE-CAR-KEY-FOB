#include <RCSwitch.h>

#define TX_PIN 26
#define UNLOCK_BTN 32
#define LOCK_BTN 33

RCSwitch rc;

const unsigned long KEY_ID = 777;
const unsigned long CODE_UNLOCK = 111111;
const unsigned long CODE_LOCK = 222222;

bool lastUnlockState = HIGH;
bool lastLockState = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(UNLOCK_BTN, INPUT_PULLUP);
  pinMode(LOCK_BTN, INPUT_PULLUP);

  rc.enableTransmit(TX_PIN);
  rc.setPulseLength(350);
  rc.setRepeatTransmit(10);

  Serial.println("KEYFOB READY (FIXED CODE)...");
}

void loop() {
  bool unlockNow = digitalRead(UNLOCK_BTN);
  bool lockNow = digitalRead(LOCK_BTN);

  if (unlockNow == LOW && lastUnlockState == HIGH && lockNow == HIGH) {
    unsigned long txValue = KEY_ID * 1000000UL + CODE_UNLOCK;
    Serial.println("UNLOCK SIGNAL SENT");
    rc.send(txValue, 30);
  } 
  else if (lockNow == LOW && lastLockState == HIGH && unlockNow == HIGH) {
    unsigned long txValue = KEY_ID * 1000000UL + CODE_LOCK;
    Serial.println("LOCK SIGNAL SENT");
    rc.send(txValue, 30);
  }

  lastUnlockState = unlockNow;
  lastLockState = lockNow;

  delay(10);
}