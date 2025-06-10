#include "FeedBackServo.h"
// define feedback signal pin and servo control pin
#define FEEDBACK_PIN 2
#define SERVO_PIN 3

// set feedback signal pin number
FeedBackServo servo = FeedBackServo(FEEDBACK_PIN);

void setup()
{
    // set servo control pin number
    servo.setServoControl(SERVO_PIN);
    servo.setKp(1.0);
}

void loop()
{
    // rotate servo to 270 and -180 degrees(with contains +-4 degrees error) each 1 second with non-blocking.
    servo.setTarget(270);
    servo.update(4);
    delay(1000);
    servo.setTarget(-180);
    servo.update(4);
    delay(1000);
}
