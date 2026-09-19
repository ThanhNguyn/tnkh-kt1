#include <Arduino.h>

const int BTN = 2;
const int LED = 8;

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(BTN, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
    int btn = digitalRead(BTN);

    if (btn == LOW) {  // Đang nhấn
        digitalWrite(LED, HIGH);
        Serial.println("LED ON");
    } else {
        digitalWrite(LED, LOW);
    }

    delay(50);
}