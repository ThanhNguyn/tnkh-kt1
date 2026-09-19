#include <Arduino.h>

const int BTN = 2;
const int LED = 8;

bool ledState = false;
int lastBtn = HIGH;

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(BTN, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
    int curBtn = digitalRead(BTN);

    // Phát hiện cạnh xuống: HIGH -> LOW
    if (lastBtn == HIGH && curBtn == LOW) {
        ledState = !ledState;

        digitalWrite(LED, ledState);

        Serial.println(ledState ? "ON" : "OFF");
    }

    // Cập nhật trạng thái nút lần trước
    lastBtn = curBtn;

    // Debounce đơn giản
    delay(20);
}