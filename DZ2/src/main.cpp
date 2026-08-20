#include <Arduino.h>

// Інтервали — через #define, для економії пам'яті 
// препроцессор не займає RAM як окрема змінна)
#define SENSOR_INTERVAL 20000UL  // 20 секунд
#define HEAP_INTERVAL   60000UL  // 1 хвилина

unsigned long lastSensorRead = 0;
unsigned long lastHeapCheck = 0;

// Структура даних сенсора: температура, вологість, часова мітка
struct SensorData {
  float temperature;
  float humidity;
  unsigned long timestamp;
};

// Виділяє SensorData у HEAP (new), заповнює даними.
// Температура і вологість — випадкові, timestamp — реальний час (НЕ випадковий).
// Викликач відповідає за delete (див. loop()).
SensorData* readSensor() {
  SensorData* data = new SensorData();
  data->temperature = random(15, 31);  // [15, 30] включно
  data->humidity = random(30, 66);     // [30, 65] включно
  data->timestamp = millis();
  return data;
}

// Приймає вказівник (не копію), форматує через snprintf, без String
void printSensorData(const SensorData* data) {
  char buffer[80];
  snprintf(buffer, sizeof(buffer),
           "Temp: %.1f C, Humidity: %.1f %%, Time: %lu ms",
           data->temperature, data->humidity, data->timestamp);
  Serial.println(buffer);
}

void printFreeHeap() {
  char buffer[48];
  snprintf(buffer, sizeof(buffer), "[Heap check] Free heap: %u bytes", ESP.getFreeHeap());
  Serial.println(buffer);
}

void setup() {
  Serial.begin(115200);
  delay(500); // дати час порту "піднятися" перед першим виводом
}

void loop() {
  unsigned long now = millis();

  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = now;

    SensorData* reading = readSensor();  // виділили в heap
    printSensorData(reading);
    delete reading;                      // обов'язково звільнили — без цього стався б memory leak
  }

  if (now - lastHeapCheck >= HEAP_INTERVAL) {
    lastHeapCheck = now;
    printFreeHeap();
  }
}