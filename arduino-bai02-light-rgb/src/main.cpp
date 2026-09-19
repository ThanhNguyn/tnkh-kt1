#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LIGHT_SENSOR_PIN A0
#define LED_PIN 8

Adafruit_NeoPixel led(1, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(9600);
    led.begin();
    led.clear();
    led.show();
}

void loop() {
    int val = analogRead(LIGHT_SENSOR_PIN);
    Serial.println(val);

    if (val < 300) {
        // Vàng - sáng
        led.setPixelColor(0, led.Color(255, 255, 0));
    }
    else if (val < 700) {
        // Xanh dương - vừa
        led.setPixelColor(0, led.Color(0, 128, 255));
    }
    else {
        // Đỏ - tối
        led.setPixelColor(0, led.Color(255, 0, 0));
    }

    led.show();
    delay(200);
}