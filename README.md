 # PDL_Async_Button
 
 PDL_Async_Button is an Arduino library that allows you to detect short and long button presses asynchronously. This library helps you handle button presses without blocking your main program loop.
 
 ## Features
 
 - Detect short button presses
 - Detect long button presses
 - Asynchronous, non-blocking operation
 
 ## Installation
 
 1. Download the library from the [GitHub repository](https://github.com/Product-Design-Lab/PDL_Async_Buttons).
 2. Unzip the library and place it in the `libraries` folder of your Arduino sketchbook.
 
 ## API
 
 ### `PDL_Async_Button`
 
 #### Constructor
 
 ```cpp
 PDL_Async_Button(uint8_t pin);
 ```
 Creates a button instance on the specified pin.
 
 #### Methods
 
 - `void setPin(uint8_t pin, uint8_t mode);`
   Sets the pin and its mode (HIGH or LOW).
 - `void setDebounceTime(uint32_t debounceTime);`
   Sets the debounce time in milliseconds.
 - `void setLongPressTime(uint32_t longPressTime);`
   Sets the long press time in milliseconds.
 - `void setShortPressCallback(void (*shortPressCallback)());`
   Sets the callback function for short press detection.
 - `void setLongPressCallback(void (*longPressCallback)());`
   Sets the callback function for long press detection.
 - `void init();`
   Initializes the button.
 - `void update();`
   Updates the button state. Call this method in your `loop()`.
 - `uint8_t getState(int *shortPressCount, int *longPressCount);`
   Gets the current state of the button along with the counts of short and long presses.
 - `bool shortPress();`
   Returns `true` if a short press is detected.
 - `bool longPress();`
   Returns `true` if a long press is detected.
 
 ## License
 
 This library is licensed under the MIT License. See [LICENSE](LICENSE) for more details.
 
 ## Contributing
 
 Feel free to submit issues or pull requests for any bugs or feature requests.
 
 ## Credits
 
 Developed by Xuteng Lin.
