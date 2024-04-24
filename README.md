# button
(Yet another) button library to handle debounce and multiple combinations of short, long and extra-long presses

## Usage


Include the library and create a button object
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


Poll the button and check the events
```
void loop() {

  b1.poll();

  if (b1.event()) {
    Serial.print(b1.press);
    Serial.print(" ");
    for (uint8_t i = 0; i < sizeof(b1.combi); i++) {
      Serial.print(b1.combi[i]);
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
