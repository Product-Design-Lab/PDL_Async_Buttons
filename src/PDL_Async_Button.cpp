#include "PDL_Async_Button.h"

PDL_Async_Button *PDL_Async_Button::instances[MAX_PIN_NUM] = {nullptr};
uint8_t PDL_Async_Button::instance_count = 0;

PDL_Async_Button::PDL_Async_Button()
{
    if (instance_count >= MAX_PIN_NUM)
    {
        return;
    }
    idx = instance_count++;
    instances[idx] = this;

    pin = 0;
    debounceTime = 50;
    longPressTime = 1000;
    idle_logic_level = LOW;
    short_press_count = 0;
    long_press_count = 0;
    state = BUTTON_IDLE;
    output_state = BUTTON_IDLE;
    timerHandle = xTimerCreate("ButtonTimer", pdMS_TO_TICKS(debounceTime), pdFALSE, (void *)(intptr_t)idx, idx == 0 ? timerCallbackInstance1 : timerCallbackInstance2);
}

void PDL_Async_Button::setPin(uint8_t pin, bool idle_logic_level)
{
    this->pin = pin;
    this->idle_logic_level = idle_logic_level;
    pinMode(pin, idle_logic_level == HIGH ? INPUT_PULLUP : INPUT_PULLDOWN);
}

void PDL_Async_Button::setDebounceTime(uint32_t ms)
{
    debounceTime = ms;
}

void PDL_Async_Button::setLongPressTime(uint32_t ms)
{
    longPressTime = ms;
}

void PDL_Async_Button::setIdleLogicLevel(bool logic_level)
{
    idle_logic_level = logic_level;
    pinMode(pin, logic_level == HIGH ? INPUT_PULLUP : INPUT_PULLDOWN);
}

uint8_t PDL_Async_Button::getState(int *short_press_count, int *long_press_count)
{
    *short_press_count = this->short_press_count;
    *long_press_count = this->long_press_count;
    this->short_press_count = 0;
    this->long_press_count = 0;
    return getState();
}

uint8_t PDL_Async_Button::getState()
{
    uint8_t currentState = output_state;
    output_state = BUTTON_IDLE;
    return currentState;
}

void PDL_Async_Button::init()
{
    setInitialState();
}

void PDL_Async_Button::gpioCallback()
{
    if (state == BUTTON_IDLE)
    {
        setDebounceState();
    }
    else if (state == BUTTON_SHORT_PRESS)
    {
        short_press_count++;
        output_state = BUTTON_SHORT_PRESS;
        if (shortPressCallback)
        {
            shortPressCallback(); // Call the callback
        }
        setInitialState();
    }
    else if (state == BUTTON_LONG_PRESS)
    {
        setInitialState();
    }
}

void PDL_Async_Button::gpioCallbackInstance1()
{
    instances[0]->gpioCallback();
}

void PDL_Async_Button::gpioCallbackInstance2()
{
    instances[1]->gpioCallback();
}

void PDL_Async_Button::timerCallback(TimerHandle_t xTimer)
{
    if (state == BUTTON_DEBOUNCE)
    {
        if (digitalRead(pin) != idle_logic_level)
        {
            setShortPressState();
        }
        else
        {
            setInitialState();
        }
    }
    else if (state == BUTTON_SHORT_PRESS)
    {
        long_press_count++;
        output_state = BUTTON_LONG_PRESS;
        setLongPressState();
    }
}

void PDL_Async_Button::timerCallbackInstance1(TimerHandle_t xTimer)
{
    instances[0]->timerCallback(xTimer);
}

void PDL_Async_Button::timerCallbackInstance2(TimerHandle_t xTimer)
{
    instances[1]->timerCallback(xTimer);
}

void PDL_Async_Button::setInitialState()
{
    state = BUTTON_IDLE;
    attachInterrupt(pin, idx == 0 ? gpioCallbackInstance1 : gpioCallbackInstance2, idle_logic_level == LOW ? RISING : FALLING);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerStopFromISR(timerHandle, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void PDL_Async_Button::setDebounceState()
{
    state = BUTTON_DEBOUNCE;
    detachInterrupt(pin);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerChangePeriodFromISR(timerHandle, pdMS_TO_TICKS(debounceTime), &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void PDL_Async_Button::setShortPressState()
{
    state = BUTTON_SHORT_PRESS;
    attachInterrupt(pin, idx == 0 ? gpioCallbackInstance1 : gpioCallbackInstance2, idle_logic_level == LOW ? FALLING : RISING);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerChangePeriodFromISR(timerHandle, pdMS_TO_TICKS(longPressTime), &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void PDL_Async_Button::setLongPressState()
{
    state = BUTTON_LONG_PRESS;
    attachInterrupt(pin, idx == 0 ? gpioCallbackInstance1 : gpioCallbackInstance2, idle_logic_level == LOW ? FALLING : RISING);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerStopFromISR(timerHandle, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    if (longPressCallback)
    {
        longPressCallback(); // Call the callback
    }
}

void PDL_Async_Button::setShortPressCallback(ButtonCallback callback)
{
    shortPressCallback = callback;
}

void PDL_Async_Button::setLongPressCallback(ButtonCallback callback)
{
    longPressCallback = callback;
}