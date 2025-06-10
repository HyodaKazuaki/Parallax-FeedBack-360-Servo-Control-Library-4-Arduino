#include "FeedBackServo.h"
// define feedback signal pin
#define FEEDBACK_PIN1 2
#define FEEDBACK_PIN2 3

// set feedback signal pin number
FeedBackServo servo1 = FeedBackServo(FEEDBACK_PIN1);
FeedBackServo servo2 = FeedBackServo(FEEDBACK_PIN2);

void setup()
{
    Serial.begin(115200);

    servo1.setServoControl(9);
    servo2.setServoControl(10);

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