#include "FeedBackServo.h"

// Define feedback signal pin and servo control pin
#define FEEDBACK_PIN 2
#define SERVO_PIN 3

// Set feedback signal pin number
FeedBackServo servo = FeedBackServo(FEEDBACK_PIN);

int target = 0;             // State selection
const long interval = 2000; // 2 seconds (in milliseconds)
unsigned long previousTime = 0;

void setup()
{
    // Set servo control pin number
    servo.setServoControl(SERVO_PIN);

    // Adjust Kp as needed
    servo.setKp(1.0);
}

void loop()
{
    // Rotate servo from 0 to 180 (w/ +-2 threshold) using non-blocking.
    servo.update(2);

    // Calculate whether new target input request meets specified time interval requirement to prevent mistarget.
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= interval)
    {
        previousTime = currentTime;

        // Prevents improper targetting by providing proper time for relevant calculations to take place.
        switch (target)
        {
        case 0:
            target = 1;
            servo.setTarget(0);
            break;
        case 1:
            target = 0;
            servo.setTarget(180);
            break;
        }
    }
}
