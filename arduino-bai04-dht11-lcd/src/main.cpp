#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHT_PIN 3
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    dht.begin();

    lcd.begin();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Mini Weather");

    lcd.setCursor(0, 1);
    lcd.print("Station");

    delay(2000);
    lcd.clear();
}

void loop() {
    float temp = dht.readTemperature();
    float humi = dht.readHumidity();

    if (isnan(temp) || isnan(humi)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sensor Error!");
        delay(2000);
        return;
    }

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp, 1);
    lcd.print((char)223);
    lcd.print("C   ");

    lcd.setCursor(0, 1);
    lcd.print("Humi: ");
    lcd.print(humi, 1);
    lcd.print("%   ");

    delay(2000);
}