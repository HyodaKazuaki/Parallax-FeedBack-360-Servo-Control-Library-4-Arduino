# Parallax FeedBack 360 Servo Control Library 4 Arduino

This library facilitates control of [Parallax FeedBack 360° High Speed Servo](https://www.parallax.com/product/900-00360). This servo motor provides feedback via the PWM signal. You can control servo to degree of rotation what you want with this library. Of course, you can read servo's degree of rotation.

日本語の README は[`README_ja.md`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/README_ja.md)をご覧ください。

## NOTICE

This library introduced breaking changes for non-blocking control and support for multiple servo motors since major version 2.0.0.
For details, see function definitions and examples.

## Support

Please see [`SUPPORTED.md`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/SUPPORTED.md).

## Pin Assign

Assign the feedback signal line of the servomotor to the feedback signal pin that allows external interruption.
Assign the control pin to which the servo motor control line is assigned.

## How to Install

Download this library on [Release page](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/releases) and Install zip file on Arduino IDE.

## Example

This library is contains some examples.

### Rotation

```cpp
#include "FeedBackServo.h"

// Sefine feedback signal pin and servo control pin
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

    // Calculate whether new target input request meets specified time interval requirement to prevent mistarget
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= interval)
    {
        previousTime = currentTime;

        // Prevents improper targetting by providing proper time for relevant calculations to take place
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
```

### Read

```cpp
#include "FeedBackServo.h"

// Use a valid interrupt pin
#define FEEDBACK_PIN 2

// Create the servo object
FeedBackServo feedbackServo(FEEDBACK_PIN);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  // Necessary to ensure the measured angle of the servo is updated each iteration
  feedbackServo.update();

  // Retrieve angle from servo
  int currentAngle = feedbackServo.getAngle();

  // Output angle
  Serial.println(currentAngle);
}
```

### Multi motor controls

```cpp
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
```

## License

This library is released under the MIT License.
Please see [`LICENSE`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/LICENSE) file for detail.

## Contributors

This library is made possible by the valuable contributions of many individuals.
Please see [`CONTRIBUTORS.md`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/CONTRIBUTORS.md) file for a detailed list.
