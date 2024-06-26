#pragma once

#include <Arduino.h>
#include "FreeRTOS.h"
#include "timers.h"

#define MAX_PIN_NUM 2

class PDL_Async_Button
{
public:
    

    enum ButtonState
    {
        BUTTON_IDLE,
        BUTTON_DEBOUNCE,
        BUTTON_SHORT_PRESS,
        BUTTON_LONG_PRESS,
        BUTTON_MAX_STATES
    };

private:
    static PDL_Async_Button *instances[MAX_PIN_NUM];
    static uint8_t instance_count;

    uint8_t idx;
    const uint8_t pin;
    const bool idle_logic_level;
    uint32_t debounceTime;
    uint32_t longPressTime;
    int short_press_count;
    int long_press_count;
    uint8_t state;
    uint8_t output_state;
    bool initialized;

    TimerHandle_t timerHandle;
    using ButtonCallback = void (*)(); // Define a function pointer type for callbacks
    ButtonCallback shortPressCallback; // Callback for short press
    ButtonCallback longPressCallback;  // Callback for long press

    void gpioCallback();
    static void gpioCallbackInstance1();
    static void gpioCallbackInstance2();

    void timerCallback(TimerHandle_t xTimer);
    static void timerCallbackInstance1(TimerHandle_t xTimer);
    static void timerCallbackInstance2(TimerHandle_t xTimer);

    void setInitialState();
    void setDebounceState();
    void setShortPressState();
    void setLongPressState();

public:
    PDL_Async_Button(uint8_t pin, bool idle_logic_level = HIGH);
    ~PDL_Async_Button();

    void setDebounceTime(uint32_t ms);
    void setLongPressTime(uint32_t ms);
    uint8_t getState(int *short_press_count, int *long_press_count);
    uint8_t getState();
    void init();
    void deinit();
    void disable();
    void enable();

    void setShortPressCallback(ButtonCallback callback); // Set short press callback
    void setLongPressCallback(ButtonCallback callback);  // Set long press callback
};
