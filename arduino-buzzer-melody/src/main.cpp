#include <Arduino.h>

#define BUZZER_PIN 5

// Nốt nhạc: Do Re Mi Fa Sol La Si
int notes[] = {
  262, 294, 330, 349, 392, 440, 494
};

int beats[] = {
  4, 4, 4, 4, 4, 4, 8
};

int tempo = 120;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  for (int i = 0; i < 7; i++) {
    int dur = (60000 / tempo) * beats[i] / 4;

    tone(BUZZER_PIN, notes[i], dur);
    delay(dur * 1.3);

    noTone(BUZZER_PIN);
  }

  delay(1000);
}