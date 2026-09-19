#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define DHT_PIN 3
#define DHT_TYPE DHT22

#define TILT_PIN 2
#define BUZZER_PIN 5
#define LIGHT_PIN A0
#define RGB_LED_PIN 8

#define TEMP_THRESHOLD 35.0
#define LIGHT_THRESHOLD 200

DHT dht(DHT_PIN, DHT_TYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_NeoPixel rgb(
    1,
    RGB_LED_PIN,
    NEO_GRB + NEO_KHZ800
);

void setRGB(uint8_t r, uint8_t g, uint8_t b)
{
    rgb.setPixelColor(0, rgb.Color(r, g, b));
    rgb.show();
}

void displayData(
    float temperature,
    float humidity,
    int light,
    const char* status
)
{
    lcd.setCursor(0, 0);

    lcd.print("T:");
    lcd.print(temperature, 1);
    lcd.print((char)223);
    lcd.print("C ");

    lcd.print("H:");
    lcd.print(humidity, 0);
    lcd.print("% ");

    lcd.setCursor(0, 1);

    lcd.print("L:");
    lcd.print(light);
    lcd.print(" ");

    lcd.print(status);
    lcd.print("        ");
}

void tiltAlarm()
{
    for (int i = 0; i < 3; i++)
    {
        tone(BUZZER_PIN, 1000);
        delay(150);

        noTone(BUZZER_PIN);
        delay(150);
    }
}

void setup()
{
    Serial.begin(9600);

    dht.begin();

    pinMode(TILT_PIN, INPUT_PULLUP);

    pinMode(BUZZER_PIN, OUTPUT);
    noTone(BUZZER_PIN);

    rgb.begin();
    rgb.clear();
    rgb.show();

    lcd.begin();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Smart Station");

    lcd.setCursor(0, 1);
    lcd.print("Starting...");

    setRGB(0, 255, 0);

    delay(2000);

    lcd.clear();
}

void loop()
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    int light = analogRead(LIGHT_PIN);

    bool tilted = (digitalRead(TILT_PIN) == LOW);

    Serial.print("Temp: ");
    Serial.print(temperature, 1);

    Serial.print(" C | Humidity: ");
    Serial.print(humidity, 1);

    Serial.print(" % | Light: ");
    Serial.print(light);

    Serial.print(" | Tilt: ");

    if (tilted)
        Serial.println("YES");
    else
        Serial.println("NO");

    // -------------------------
    // DHT ERROR
    // -------------------------

    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("DHT SENSOR ERROR!");

        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("DHT Error!");

        lcd.setCursor(0, 1);
        lcd.print("Check Sensor");

        setRGB(255, 0, 0);

        noTone(BUZZER_PIN);

        delay(2000);

        return;
    }

    // -------------------------
    // HIGH TEMPERATURE
    // -------------------------

    if (temperature > TEMP_THRESHOLD)
    {
        displayData(
            temperature,
            humidity,
            light,
            "HOT"
        );

        // RED
        setRGB(255, 0, 0);

        // Continuous beep
        tone(BUZZER_PIN, 1000);

        delay(2000);

        noTone(BUZZER_PIN);
    }

    // -------------------------
    // TILT
    // -------------------------

    else if (tilted)
    {
        displayData(
            temperature,
            humidity,
            light,
            "TILT"
        );

        // YELLOW
        setRGB(255, 255, 0);

        // 3 short beeps
        tiltAlarm();

        delay(500);
    }

    // -------------------------
    // NIGHT
    // -------------------------

    else if (light < LIGHT_THRESHOLD)
    {
        displayData(
            temperature,
            humidity,
            light,
            "NIGHT"
        );

        // BLUE
        setRGB(0, 0, 255);

        noTone(BUZZER_PIN);

        lcd.backlight();

        delay(2000);
    }

    // -------------------------
    // NORMAL
    // -------------------------

    else
    {
        displayData(
            temperature,
            humidity,
            light,
            "OK"
        );

        // GREEN
        setRGB(0, 255, 0);

        noTone(BUZZER_PIN);

        lcd.backlight();

        delay(2000);
    }
}