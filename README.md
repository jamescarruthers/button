# button
(Yet another) button library to handle debounce and multiple combinations of short, long and extra-long presses.
The library can keep track of mutliple short, long and extra long presses. It can detect things like a short-then-long press - or a short-short-long press.

## Usage


### Include the library and create a button object
```
#include <button.h>

Button b1;
```


Initialise the button with a pin number
```
void setup() {

  b1.init(11);

}
```


### Poll the button and check the events
Poll should be called every loop, the library internally rate limits the polling to 1ms by default.
* .press is the number of presses that have occured
* .presses[] is an array of the presses and their duration (1 = short, 2 = long, 3 = extra long)
```
void loop() {

  b1.poll();

  if (b1.event()) {
    Serial.print(b1.press);
    Serial.print(" ");
    for (uint8_t i = 0; i < b1.press; i++) {
      Serial.print(b1.presses[i]);
    }
    Serial.println();
  }
}
```


If you want to read button presses instantly instead of waiting for the timeout event then you can use the instant method
```
  if (b1.eventInstant()) {
    Serial.println(b1.duration);
  }
```

Check the .h file and the public variables for more advanced use, such as setting timeout and press length durations.
