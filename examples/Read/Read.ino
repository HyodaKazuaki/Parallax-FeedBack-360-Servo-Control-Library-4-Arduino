#include "FeedBackServo.h"

// Use a valid interrupt pin
#define FEEDBACK_PIN 2 

// Create the servo object
FeedBackServo feedbackServo(FEEDBACK_PIN);

void setup() {
  Serial.begin(115200);
}

void loop() {
    
    // Necessary to ensure the measured angle of the servo is updated each iteration
    feedbackServo.update(); 

    // Retrieve angle from servo
    int currentAngle = feedbackServo.getAngle(); 

    // Output angle
    Serial.println(currentAngle); 
}
