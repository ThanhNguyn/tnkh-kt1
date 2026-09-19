#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

// =========================
// PIN CONFIGURATION
// =========================

#define DHT_PIN 3
#define DHT_TYPE DHT22   // Wokwi simulation; hardware thực tế: DHT11

#define TILT_PIN 2
#define BUZZER_PIN 5
#define LIGHT_PIN A0
#define RGB_LED_PIN 8

// =========================
// THRESHOLDS
// =========================

#define TEMP_THRESHOLD 35.0
#define LIGHT_THRESHOLD 200

// =========================
// DEVICES
// =========================

DHT dht(DHT_PIN, DHT_TYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_NeoPixel rgb(
    1,
    RGB_LED_PIN,
    NEO_GRB + NEO_KHZ800
);

// =========================
// RGB LED
// =========================

void setRGB(uint8_t r, uint8_t g, uint8_t b)
{
    rgb.setPixelColor(0, rgb.Color(r, g, b));
    rgb.show();
}

// =========================
// LCD
// =========================

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

    // Clear remaining characters
    lcd.print("        ");
}

// =========================
// TILT ALARM
// 3 short beeps
// =========================

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

// =========================
// SETUP
// =========================

void setup()
{
    Serial.begin(9600);

    // DHT
    dht.begin();

    // Tilt switch
    pinMode(TILT_PIN, INPUT_PULLUP);

    // Buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    noTone(BUZZER_PIN);

    // RGB
    rgb.begin();
    rgb.clear();
    rgb.show();

    // LCD
    lcd.begin();
    lcd.backlight();
    lcd.clear();

    // Startup screen
    lcd.setCursor(0, 0);
    lcd.print("Smart Station");

    lcd.setCursor(0, 1);
    lcd.print("Starting...");

    setRGB(0, 255, 0);

    delay(2000);

    lcd.clear();

    // Give DHT enough time before first reading
    delay(1000);
}

// =========================
// LOOP
// =========================

void loop()
{
    // Read DHT
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Read light
    int light = analogRead(LIGHT_PIN);

    // Read tilt
    bool tilted = (digitalRead(TILT_PIN) == LOW);

    // =========================
    // SERIAL MONITOR
    // =========================

    Serial.print("Temp: ");
    Serial.print(temperature, 1);

    Serial.print(" C | Humidity: ");
    Serial.print(humidity, 1);

    Serial.print(" % | Light: ");
    Serial.print(light);

    Serial.print(" | Tilt: ");

    if (tilted)
    {
        Serial.println("YES");
    }
    else
    {
        Serial.println("NO");
    }

    // =========================
    // DHT ERROR
    // =========================

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

    // =========================
    // 1. HIGH TEMPERATURE
    // T > 35 C
    // RED + CONTINUOUS BUZZER
    // =========================

    if (temperature > TEMP_THRESHOLD)
    {
        displayData(
            temperature,
            humidity,
            light,
            "HOT"
        );

        setRGB(255, 0, 0);

        tone(BUZZER_PIN, 1000);

        delay(2000);

        noTone(BUZZER_PIN);
    }

    // =========================
    // 2. TILT DETECTION
    // YELLOW + 3 SHORT BEEPS
    // =========================

    else if (tilted)
    {
        displayData(
            temperature,
            humidity,
            light,
            "TILT"
        );

        setRGB(255, 255, 0);

        tiltAlarm();

        delay(500);
    }

    // =========================
    // 3. NIGHT MODE
    // LIGHT < 200
    // BLUE + LCD BACKLIGHT
    // =========================

    else if (light < LIGHT_THRESHOLD)
    {
        displayData(
            temperature,
            humidity,
            light,
            "NIGHT"
        );

        setRGB(0, 0, 255);

        noTone(BUZZER_PIN);

        lcd.backlight();

        delay(2000);
    }

    // =========================
    // 4. NORMAL
    // GREEN
    // =========================

    else
    {
        displayData(
            temperature,
            humidity,
            light,
            "OK"
        );

        setRGB(0, 255, 0);

        noTone(BUZZER_PIN);

        lcd.backlight();

        delay(2000);
    }
}