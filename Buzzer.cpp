#include "Buzzer.h"

Buzzer::Buzzer(int pinNumber) : pin(pinNumber) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Buzzer::beepTask(void *param) {
    Buzzer *instance = static_cast<Buzzer *>(param);
    if (instance) {
        instance->runBeep();
    }
}

void Buzzer::runBeep() {
    isRunning = true;
    while (true) {
        if (remainingCycles == 0 && cycleCount > 0) {
            isRunning = false;
            beepTaskHandle = nullptr;
            vTaskDelete(nullptr);
            return;
        }

        for (int i = 0; i < signalsPerCycle; ++i) {
            digitalWrite(pin, HIGH);
            vTaskDelay(pdMS_TO_TICKS(onDuration));
            digitalWrite(pin, LOW);
            vTaskDelay(pdMS_TO_TICKS(offDuration));
        }

        if (remainingCycles > 0) {
            --remainingCycles;
        }

        vTaskDelay(pdMS_TO_TICKS(pauseBetweenCycles));
    }
}

void Buzzer::beep(int onMs, int offMs, int signals, int pauseMs, int cycles) {
    if (beepTaskHandle) {
        stop();
    }

    onDuration = onMs;
    offDuration = offMs > 0 ? offMs : onMs;
    signalsPerCycle = signals;
    pauseBetweenCycles = pauseMs;
    cycleCount = cycles;
    remainingCycles = cycles;

    if (xTaskCreate(beepTask, "BuzzerBeepTask", 2048, this, 1, &beepTaskHandle) != pdPASS) {
        Serial.println("Error: Task creation failed!");
        beepTaskHandle = nullptr;
    }
}

void Buzzer::stop() {
    if (beepTaskHandle) {
        vTaskDelete(beepTaskHandle);
        beepTaskHandle = nullptr;
        isRunning = false;
        digitalWrite(pin, LOW);
    }
}

bool Buzzer::isProcessing() const {
    return isRunning;
}

Buzzer::~Buzzer() {
    stop();
}
