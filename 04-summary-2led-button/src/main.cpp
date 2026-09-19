#include <Arduino.h>

const int BUTTON = 2;
const int GREEN_LED = 8;
const int RED_LED = 9;

bool state = false;       // false = xanh, true = đỏ
int lastButton = HIGH;

void setup() {
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    Serial.begin(9600);

    // Trạng thái mặc định
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    Serial.println("GREEN ON");
    Serial.println("RED OFF");
}

void loop() {
    int currentButton = digitalRead(BUTTON);

    // Phát hiện cạnh xuống: HIGH -> LOW
    if (lastButton == HIGH && currentButton == LOW) {
        state = !state;

        if (state) {
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);

            Serial.println("GREEN OFF");
            Serial.println("RED ON");
        } else {
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, LOW);

            Serial.println("GREEN ON");
            Serial.println("RED OFF");
        }

        delay(50); // debounce
    }

    lastButton = currentButton;
}