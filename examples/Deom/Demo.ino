#include <Arduino.h>
#include <Buzzer.h>

Buzzer buzzer(2); // Подключите ваш пин

void setup() {
    Serial.begin(115200);
    buzzer.beep(500, 500, 3, 1000, 5); // Пять циклов по три сигнала с паузами
}

void loop() {
    if (!buzzer.isProcessing()) {
        Serial.println("Beeping completed!");
        delay(2000);
        buzzer.beep(200, 200, 2, 500, 3); // Новый сигнал
    }
}
