#include <Arduino.h>
#include <PDL_Async_Button.h>
#include <Adafruit_TinyUSB.h>

#define PIN_NUM 0

// Create an instance of PDL_Async_Button
PDL_Async_Button button(PIN_NUM, HIGH);

// Callback function for short press
void onShortPress() {
    Serial.println("Event: Short Press detected");
}

// Callback function for long press
void onLongPress() {
    Serial.println("Event: Long Press detected");
}

void setup() {
    // Initialize Serial Communication
    Serial.begin(115200);
    while (!Serial);

    // Configure the button
    button.setDebounceTime(50);
    button.setLongPressTime(1000);
    button.setShortPressCallback(onShortPress);
    button.setLongPressCallback(onLongPress);
    button.init();

    Serial.println("Button initialized.");
}

void loop() {
    // The button press events are handled by the callbacks, nothing to do here
    delay(200);
    if(Serial.available())
    {
      char c = Serial.read();
      if(c == 'd')
      {
        button.disable();
        Serial.println("Button disabled.");
      }
      else if(c == 'e')
      {
        button.enable();
        Serial.println("Button enabled.");
      }

      while(Serial.available())
      {
        Serial.read();
      }
    }
}
