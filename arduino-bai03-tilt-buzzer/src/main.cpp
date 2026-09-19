#include <Arduino.h>

#define TILT_PIN 2
#define BUZZ_PIN 5

void setup() {
    pinMode(TILT_PIN, INPUT);
    pinMode(BUZZ_PIN, OUTPUT);

    Serial.begin(9600);
}

void loop() {
    if (digitalRead(TILT_PIN) == LOW) {
        // Nghiêng -> cảnh báo
        Serial.println("TILT DETECTED!");
        tone(BUZZ_PIN, 1000, 200);
        delay(300);
    } else {
        // Bình thường
        Serial.println("NORMAL");
        noTone(BUZZ_PIN);
        delay(100);
    }
}