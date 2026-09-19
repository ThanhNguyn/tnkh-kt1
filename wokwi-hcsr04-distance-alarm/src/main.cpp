#include <Arduino.h>

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int BUZZER_PIN = 8;

float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0) {
        return -1;
    }

    return duration * 0.0343 / 2.0;
}

void setup() {
    Serial.begin(9600);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
    float distance = getDistance();

    Serial.print("Distance: ");

    if (distance < 0) {
        Serial.println("Out of range");
        noTone(BUZZER_PIN);
        delay(100);
        return;
    }

    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 20) {
        tone(BUZZER_PIN, 1000);
        delay(100);
    }
    else if (distance < 50) {
        tone(BUZZER_PIN, 1000);
        delay(150);
        noTone(BUZZER_PIN);
        delay(150);
    }
    else if (distance < 100) {
        tone(BUZZER_PIN, 1000);
        delay(200);
        noTone(BUZZER_PIN);
        delay(800);
    }
    else {
        noTone(BUZZER_PIN);
        delay(200);
    }
}