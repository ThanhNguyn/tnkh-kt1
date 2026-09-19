#include <Arduino.h>

const int POT_PIN = A0;

const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;

void setColor(int red, int green, int blue) {
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
}

void setup() {
    Serial.begin(9600);

    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
    int potValue = analogRead(POT_PIN);

    int zone = map(potValue, 0, 1023, 0, 5);

    switch (zone) {
        case 0:
            setColor(255, 0, 0);
            break;

        case 1:
            setColor(255, 80, 0);
            break;

        case 2:
            setColor(0, 255, 0);
            break;

        case 3:
            setColor(0, 0, 255);
            break;

        case 4:
            setColor(150, 0, 255);
            break;

        default:
            setColor(255, 0, 255);
            break;
    }

    Serial.print("Potentiometer: ");
    Serial.print(potValue);
    Serial.print(" | Zone: ");
    Serial.println(zone);

    delay(50);
}