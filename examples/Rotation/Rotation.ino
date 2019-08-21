#include "FeedBackServo.h"
// define feedback signal pin and servo control pin
#define FEEDBACK_PIN 2
#define SERVO_PIN 3

// set feedback signal pin number
FeedBackServo servo = FeedBackServo(FEEDBACK_PIN);

void setup() {
    // set servo control pin number
    servo.setServoControl(SERVO_PIN);
}

void loop() {
    // rotate servo to 300 degrees(with contains +-4 degrees error)
    servo.rotate(300, 4);
}