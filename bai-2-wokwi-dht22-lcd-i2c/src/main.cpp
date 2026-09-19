#include <Arduino.h>
#include <Wire.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define DHT_PIN 2
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(9600);

    dht.begin();

    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("DHT22 Sensor");
    lcd.setCursor(0, 1);
    lcd.print("Starting...");

    delay(2000);
    lcd.clear();
}

void loop() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read DHT22!");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sensor Error");

        delay(2000);
        return;
    }

    // Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature, 1);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity, 1);
    Serial.println(" %");

    Serial.println("----------------");

    // LCD
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity, 1);
    lcd.print("%");

    delay(2000);
}