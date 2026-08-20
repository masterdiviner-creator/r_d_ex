#include <Arduino.h>

// Таймер для сенсора
unsigned long lastSensorRead = 0;
#define SENSOR_INTERVAL 20000  // 20 секунд

// Структура для зберігання даних сенсора
struct SensorData {
  float temperature;       // 4 байти
  uint8_t status;          // 1 байт — бітові флаги
  unsigned long timestamp; // 4 байти
};

// Біт 0 = Wi-Fi connected
uint8_t checkWifi() {
  return 0b00000001;
}

// Біт 1 = sensor ok
uint8_t checkSensor() {
  return 0b00000010;
}

// Приймає вказівник — не копію структури
void printData(SensorData* data) {
  static int callCount = 0;
  callCount++;

  Serial.println("--- Reading #" + String(callCount) + " ---");
  Serial.println("Temp: " + String(data->temperature) + " C");
  Serial.println("Status: " + String(data->status));
  Serial.println("Time: " + String(data->timestamp) + " ms");
  Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("---");
}

SensorData readSensor() {
  SensorData data;

  data.temperature = 23.5;
  data.timestamp = millis();

  // Бітові флаги
  data.status = 0b00000000 | checkWifi();
  data.status = data.status | checkSensor();

  return data;
}


void setup() {
  Serial.begin(115200);
  delay(500);
}

void loop() {
  unsigned long now = millis();

  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = now;

    SensorData reading;
    reading = readSensor();
    printData(&reading);
  }

}