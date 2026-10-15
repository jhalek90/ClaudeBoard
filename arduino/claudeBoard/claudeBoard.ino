// claudeBoard - two-button macro keyboard for the Claude Code VS Code extension.
//
// Board: Arduino Pro Micro (ATmega32U4). In the Arduino IDE select
// "Arduino Leonardo" (or "SparkFun Pro Micro, 5V/16MHz" if that core is installed).
//
// Wiring (each switch has two legs, polarity doesn't matter):
//   Left eye  (mic)  : pin 2  <-> switch <-> GND
//   Right eye (send) : pin 3  <-> switch <-> GND
// Internal pull-ups are used, so no resistors are needed.
//
// Left eye  -> Ctrl+D : Claude Code dictation. Mirrors the switch, so a quick
//                       tap toggles recording and holding it is push-to-talk.
// Right eye -> Enter  : sends the prompt.

#include <Keyboard.h>

const uint8_t PIN_MIC  = 2;
const uint8_t PIN_SEND = 3;

const unsigned long DEBOUNCE_MS = 15;

struct Button {
  uint8_t pin;
  bool pressed;              // debounced state
  bool lastReading;
  unsigned long lastChange;
};

Button micButton  = { PIN_MIC,  false, false, 0 };
Button sendButton = { PIN_SEND, false, false, 0 };

// Returns true when the debounced state changes.
bool updateButton(Button &b) {
  bool reading = digitalRead(b.pin) == LOW;  // LOW = pressed (pull-up)
  unsigned long now = millis();

  if (reading != b.lastReading) {
    b.lastReading = reading;
    b.lastChange = now;
  }

  if (reading != b.pressed && now - b.lastChange >= DEBOUNCE_MS) {
    b.pressed = reading;
    return true;
  }
  return false;
}

void setup() {
  pinMode(PIN_MIC, INPUT_PULLUP);
  pinMode(PIN_SEND, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  if (updateButton(micButton)) {
    if (micButton.pressed) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('d');
    } else {
      Keyboard.release('d');
      Keyboard.release(KEY_LEFT_CTRL);
    }
  }

  if (updateButton(sendButton)) {
    if (sendButton.pressed) {
      Keyboard.press(KEY_RETURN);
    } else {
      Keyboard.release(KEY_RETURN);
    }
  }
}
