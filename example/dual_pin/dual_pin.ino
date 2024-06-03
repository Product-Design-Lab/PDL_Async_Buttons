#include <Arduino.h>
#include <PDL_Async_Button.h>
#include <Adafruit_TinyUSB.h>

// Create instances of PDL_Async_Button
PDL_Async_Button button1(0, HIGH);
PDL_Async_Button button2(1, HIGH);


void setup()
{
    // Initialize Serial Communication
    Serial.begin(115200);
    while (!Serial)
        ;

    // Configure the buttons
    button1.setDebounceTime(50);
    button1.setLongPressTime(1000);
    button1.init();

    button2.setDebounceTime(50);
    button2.setLongPressTime(1000);
    button2.init();

    Serial.println("Buttons initialized.");

}



void loop()
{
    int short_press_count1;
    int long_press_count1;
    uint8_t state1 = button1.getState(&short_press_count1, &long_press_count1);

    int short_press_count2;
    int long_press_count2;
    uint8_t state2 = button2.getState(&short_press_count2, &long_press_count2);

    // Print state and counts only if they change
    if (state1 != PDL_Async_Button::BUTTON_IDLE || state2 != PDL_Async_Button::BUTTON_IDLE)
    {
        switch (state1)
        {
        case PDL_Async_Button::BUTTON_IDLE:
            Serial.print("State1: IDLE");
            break;
        case PDL_Async_Button::BUTTON_DEBOUNCE:
            Serial.print("State1: DEBOUNCE");
            break;
        case PDL_Async_Button::BUTTON_SHORT_PRESS:
            Serial.print("State1: SHORT PRESS");
            break;
        case PDL_Async_Button::BUTTON_LONG_PRESS:
            Serial.print("State1: LONG PRESS");
            break;
        default:
            Serial.print("State1: UNKNOWN");
            break;
        }

        Serial.printf(", Short1: %d, Long1: %d\n", short_press_count1, long_press_count1);

        switch (state2)
        {
        case PDL_Async_Button::BUTTON_IDLE:
            Serial.print("State2: IDLE");
            break;
        case PDL_Async_Button::BUTTON_DEBOUNCE:
            Serial.print("State2: DEBOUNCE");
            break;
        case PDL_Async_Button::BUTTON_SHORT_PRESS:
            Serial.print("State2: SHORT PRESS");
            break;
        case PDL_Async_Button::BUTTON_LONG_PRESS:
            Serial.print("State2: LONG PRESS");
            break;
        default:
            Serial.print("State2: UNKNOWN");
            break;
        }

        Serial.printf(", Short2: %d, Long2: %d\n", short_press_count2, long_press_count2);
    }

}
