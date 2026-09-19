#include <Arduino.h>
#include <DHT.h>
#include <Servo.h>

#define DHT_PIN 2
#define DHT_TYPE DHT22
#define SERVO_PIN 9

DHT dht(DHT_PIN, DHT_TYPE);
Servo ventilationServo;

void setup() {
    Serial.begin(9600);

    dht.begin();
    ventilationServo.attach(SERVO_PIN);

    ventilationServo.write(0);
}

void loop() {
    float temperature = dht.readTemperature();

    if (isnan(temperature)) {
        Serial.println("Failed to read DHT22!");
        delay(2000);
        return;
    }

    int angle;

    if (temperature < 25) {
        angle = 0;
    } else if (temperature < 30) {
        angle = 45;
    } else if (temperature < 35) {
        angle = 90;
    } else {
        angle = 180;
    }

    ventilationServo.write(angle);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C | Servo: ");
    Serial.print(angle);
    Serial.println(" degrees");

    delay(1000);
}