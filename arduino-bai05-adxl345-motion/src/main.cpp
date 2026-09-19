#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define BUZZER_PIN 5
#define LED_PIN 8

#define FALL_THRESHOLD 0.5
#define TILT_THRESHOLD 0.2

Adafruit_ADXL345_Unified adxl = Adafruit_ADXL345_Unified(12345);
Adafruit_NeoPixel led(1, LED_PIN, NEO_GRB + NEO_KHZ800);

void setColor(uint8_t r, uint8_t g, uint8_t b) {
    led.setPixelColor(0, led.Color(r, g, b));
    led.show();
}

void setup() {
    Serial.begin(9600);

    pinMode(BUZZER_PIN, OUTPUT);

    led.begin();
    led.clear();
    led.show();

    Serial.println("ADXL345 Motion Detector");

    if (!adxl.begin()) {
        Serial.println("ADXL345 not found!");
        setColor(255, 0, 0);

        while (true) {
            delay(100);
        }
    }

    adxl.setRange(ADXL345_RANGE_16_G);

    Serial.println("ADXL345 ready!");

    setColor(0, 255, 0);
}

void loop() {
    sensors_event_t event;
    adxl.getEvent(&event);

    // Adafruit library trả về m/s^2
    // Chuyển sang đơn vị g
    float x = event.acceleration.x / 9.80665;
    float y = event.acceleration.y / 9.80665;
    float z = event.acceleration.z / 9.80665;

    // =========================
    // MỨC 1: IN X Y Z
    // =========================
    Serial.print("X:");
    Serial.print(x, 2);

    Serial.print(" Y:");
    Serial.print(y, 2);

    Serial.print(" Z:");
    Serial.println(z, 2);

    // =========================
    // MỨC 2: PHÁT HIỆN RƠI
    // =========================
    if (fabs(z) < FALL_THRESHOLD) {
        Serial.println("!!! FALL DETECTED !!!");

        setColor(255, 0, 0);

        tone(BUZZER_PIN, 1000);
        delay(300);
        noTone(BUZZER_PIN);
    }

    // =========================
    // MỨC 3: NGHIÊNG TRÁI / PHẢI
    // =========================
    else {
        noTone(BUZZER_PIN);

        if (x > TILT_THRESHOLD) {
            // X > 0 -> RED
            setColor(255, 0, 0);
        }
        else if (x < -TILT_THRESHOLD) {
            // X < 0 -> BLUE
            setColor(0, 0, 255);
        }
        else {
            // Bình thường -> GREEN
            setColor(0, 255, 0);
        }
    }

    delay(200);
}