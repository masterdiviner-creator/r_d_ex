#include <Arduino.h>

const int LED_PIN = 4;

const int DOT_DURATION = 200;   // тривалість крапки, мс
const int DASH_DURATION = 600;  // тривалість тире, мс
const int SYMBOL_GAP = 200;     // пауза між символами всередині букви
const int LETTER_GAP = 600;     // пауза між буквами (S, O, S)
const int WORD_GAP = 1400;      // пауза перед повторенням усього сигналу

// Блимнути один символ (крапку або тире) і одночасно вивести його в Serial
void blinkSymbol(char symbol) {
  int duration = (symbol == '.') ? DOT_DURATION : DASH_DURATION;

  digitalWrite(LED_PIN, HIGH);
  Serial.print(symbol);   // вивід синхронний з увімкненням світлодіода
  delay(duration);

  digitalWrite(LED_PIN, LOW);
  delay(SYMBOL_GAP);
}

// Відтворити одну букву (послідовність крапок/тире)
void playLetter(String pattern) {
  for (int i = 0; i < pattern.length(); i++) {
    blinkSymbol(pattern[i]);
  }
  Serial.print(" ");
  delay(LETTER_GAP - SYMBOL_GAP);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Serial.print("SOS: ");

  playLetter("...");  // S
  playLetter("---");  // O
  playLetter("...");  // S

  Serial.println();
  delay(WORD_GAP);     // пауза перед тим, як сигнал повториться знову
}