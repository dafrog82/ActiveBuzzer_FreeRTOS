#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class Buzzer {
private:
    int pin;
    TaskHandle_t beepTaskHandle = nullptr;
    volatile bool isRunning = false;

    static void beepTask(void *param);
    void runBeep();

public:
    int onDuration = 0;
    int offDuration = 0;
    int signalsPerCycle = 1;
    int pauseBetweenCycles = 0;
    int cycleCount = -1;
    int remainingCycles = -1;

    Buzzer(int pinNumber);
    void beep(int onMs, int offMs = 0, int signals = 1, int pauseMs = 0, int cycles = -1);
    void stop();
    bool isProcessing() const;
    ~Buzzer();
};
