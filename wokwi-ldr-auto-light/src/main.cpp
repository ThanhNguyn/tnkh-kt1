#include <Arduino.h>

const int LDR_PIN = A0;
const int LED_PIN = 9;

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    int lightValue = analogRead(LDR_PIN);

    // Ánh sáng yếu -> LED sáng
    // Ánh sáng mạnh -> LED tắt
    int brightness = map(lightValue, 0, 1023, 255, 0);

    analogWrite(LED_PIN, brightness);

    Serial.print("LDR: ");
    Serial.print(lightValue);
    Serial.print(" | LED: ");
    Serial.println(brightness);

    delay(100);
}