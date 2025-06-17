#include "FeedBackServo.h"
// Define feedback signal pin
#define FEEDBACK_PIN1 2
#define FEEDBACK_PIN2 3
// Define servo control pin number
#define SERVO_PIN1 9
#define SERVO_PIN2 10

// Set feedback signal pin number
FeedBackServo servo1 = FeedBackServo(FEEDBACK_PIN1);
FeedBackServo servo2 = FeedBackServo(FEEDBACK_PIN2);

void setup()
{
    Serial.begin(115200);

    servo1.setServoControl(SERVO_PIN1);
    servo2.setServoControl(SERVO_PIN2);

    servo1.setTarget(300);
    servo2.setTarget(300);

    servo1.setKp(0.5);
    servo2.setKp(0.5);
}

void loop()
{
    servo1.update();
    servo2.update();

    Serial.print(servo1.getAngle());
    Serial.print(" / ");
    Serial.println(servo2.getAngle());
}
