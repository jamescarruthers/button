#ifndef button_h
#define button_h

#include "Arduino.h"

enum State
{
    UP,
    DOWN
};

enum Duration
{
    NONE,
    SHORT,
    LONG,
    EXTRALONG
};

class Button
{
private:
    uint8_t pin;
    uint32_t sr = 0;            // a shift register that captures all the raw button states

    unsigned long timeNow;      // the current time
    unsigned long timeUp;       // the last time the button went up
    unsigned long timeDown;     // the last time the button went down
    unsigned long timeDuration; // the duration the button was down for

    bool eventTrigger; // used to trigger an event
    bool eventInstantTrigger; // used to trigger an event

    uint8_t _press;           // number of times the button has been pressed
    enum Duration _combi[12]; // an array to store length of presses

    unsigned long pollLast;

public:
    enum State state;        // current state of button
    uint8_t press;           // number of times the button has been pressed
    enum Duration duration;  // duration of current press
    enum Duration combi[12]; // an array to store the combination of presses

    unsigned long timeout = 300;        // the after the button is up where an event is triggered, this also affects the time between multiple presses
    unsigned long timeshort = 200;      // the length of a short press
    unsigned long timelong = 600;       // the length of a long press
    unsigned long timeextralong = 1000; // the length of an extra long press

    unsigned long pollDuration = 1;     // how often should it poll - set to zero to poll every call

    void begin(uint8_t _pin)
    {
        pin = _pin;
        pinMode(pin, INPUT_PULLUP);
    }

    void poll()
    {

        timeNow = millis();

        if (timeNow - pollLast > pollDuration || pollDuration == 0)
        {

            // add the current button state to the shift register, inverted because pull up
            sr = (sr << 1) | !digitalRead(pin);

            // button is down
            if (sr == 0xffffffff)
            {
                // change of state to down
                if (state == UP)
                {
                    state = DOWN;
                    timeDown = timeNow;
                    timeUp = 0;
                }
            }

            // button is up
            if (sr == 0x00000000)
            {
                // change of state to up
                if (state == DOWN)
                {
                    state = UP;

                    // set timeUp for up timeout
                    timeUp = timeNow;

                    // figure out the duration and add it to the combi array
                    timeDuration = timeNow - timeDown;
                    if (timeDuration < timeshort)
                        _combi[_press] = SHORT;
                    if (timeDuration >= timeshort && timeDuration <= timeextralong)
                        _combi[_press] = LONG;
                    if (timeDuration > timeextralong)
                        _combi[_press] = EXTRALONG;

                    duration = _combi[_press];

                    // keep track of the number of presses
                    _press++;

                    eventInstantTrigger = true;

                }

                if (timeUp)
                {
                    // if the button has been up for longer than timeout
                    if (timeNow - timeUp > timeout)
                    {
                        // set timeUp to 0 avoid checking timeout all the time
                        timeUp = 0;

                        // transfer private to public variables
                        press = _press;
                        _press = 0;
                        memcpy(combi, _combi, sizeof(_combi));
                        memset(_combi, 0, sizeof(_combi));

                        eventTrigger = true;
                    }
                }
            }
        }
    }

    bool event()
    {
        if (eventTrigger)
        {
            eventTrigger = false;
            return true;
        }
        return false;
    }

    int eventInstant()
    {
        if (eventInstantTrigger)
        {
            eventInstantTrigger = false;
            return true;
        }
        return false;
    }


};
#endif
