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

    servo1.SetServoControl(9);
    servo2.SetServoControl(10);

    servo1.SetTarget(500);
    servo2.SetTarget(5000);
}

void loop() 
{  
    servo1.Update();
    servo2.Update();

    Serial.print(servo1.Angle());
    Serial.print(" / ");
    Serial.println(servo2.Angle());
}