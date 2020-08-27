#include "EEPROM.h"

ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_FREQUENTLY_AWAKE);
ZUNO_SETUP_ASSOCIATIONS(ZUNO_ASSOCIATION_GROUP_SET_VALUE, ZUNO_ASSOCIATION_GROUP_SET_VALUE, ZUNO_ASSOCIATION_GROUP_SET_VALUE, ZUNO_ASSOCIATION_GROUP_SET_VALUE);
ZUNO_SETUP_CFGPARAMETER_HANDLER(config_parameter_changed);
ZUNO_DISABLE(SERVICE_LEDS);
ZUNO_ENABLE(NO_LOOP_MINIMALDELAY);
ZUNO_SETUP_BATTERY_LEVELS(2700, 3600);

byte state;
byte value;
byte reason;

byte pins[4] = {17, 18, 19, 20};
byte sent[4] = {0, 0, 0, 0};

void setup() {
  reason = zunoGetWakeReason();
  if (reason != ZUNO_WAKEUP_REASON_INT1 && reason != ZUNO_WAKEUP_REASON_RADIO) {
    for (int i = 0; i <= 3; i++) {
      pinMode(pins[i], INPUT_PULLUP);
      NZ_BYTE(i) = 0;
    }
    zunoSetWUOptions(ZUNO_WUPFLAGS_INT1_KEYSCAN);
  }
}

void loop() {
  if (reason == ZUNO_WAKEUP_REASON_INT1) {
    while (true) {
      delay(20);
      for (int i = 0; i <= 3; i++) {
        value = digitalRead(pins[i]);
        if (sent[i] == 0 && value == LOW) {
          if (NZ_BYTE(i) == 255) {
            state = 0;
          } else {
            state = 255;
          }
          zunoSendToGroupSetValueCommand((i + 1), state);
          sent[i] = 1;
          NZ_BYTE(i) = state;
        }
        else if (sent[i] == 1 && value == HIGH) {
          sent[i] = 0;
        }
      }
      if (sent[0] == 0 && sent[1] == 0 && sent[2] == 0 && sent[3] == 0) {
        break;
      }
      else if (sent[0] == 1 && sent[1] == 1 && sent[2] == 1 && sent[3] == 1) {
        delay(5000);
        if (digitalRead(pins[0]) == LOW && digitalRead(pins[1]) == LOW && digitalRead(pins[2]) == LOW && digitalRead(pins[3]) == LOW) {
          if (zunoInNetwork()) {
            for (int i = 1; i <= 4; i++) {
              zunoSendToGroupSetValueCommand(i, 255);
            }
          }
          zunoStartLearn(30, 0);
          delay(60000);
          zunoReboot();
        }
      }
    }
  }
  zunoSendDeviceToSleep();
}
void config_parameter_changed(byte param, word value) {
  NZ_BYTE((param - 64)) = value;
}
