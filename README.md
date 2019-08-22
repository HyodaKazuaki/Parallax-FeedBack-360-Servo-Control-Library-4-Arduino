Parallax FeedBack 360 Servo Control Library 4 Arduino
=====

This library facilitates control of [Parallax FeedBack 360° High Speed Servo](https://www.parallax.com/product/900-00360). This servo motor provides feedback via the PWM signal. You can control servo to degree of rotation what you want with this library. Of course, you can read servo's degree of rotation.

日本語のREADMEは[`README_ja.md`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/README_ja.md)をご覧ください。

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
```arduino
#include "FeedBackServo.h"
// define feedback signal pin and servo control pin
#define FEEDBACK_PIN 2
#define SERVO_PIN 3

// set feedback signal pin number
FeedBackServo servo = FeedBackServo(FEEDBACK_PIN);

void setup() {
    // set servo control pin number
    servo.setServoControl(SERVO_PIN);
    // set Kp to proportional controller
    servo.setKp(1.0);
}

void loop() {
    // rotate servo to 270 and -180 degrees(with contains +-4 degrees error) each 1 second.
    servo.rotate(270, 4);
    delay(1000);
    servo.rotate(-180, 4);
    delay(1000);
}
```
### Read
```arduino
#include "FeedBackServo.h"
// define feedback signal pin
#define FEEDBACK_PIN 2

// set feedback signal pin number
FeedBackServo Servo = FeedBackServo(FEEDBACK_PIN);

void setup() {
    // serial communication start with 115200 bps
    Serial.begin(115200);
}

void loop() {
    Serial.print("Now angle: ");
    Serial.println(servo.Angle());
}
```

## License
This library is released under the MIT License.
Please see [`LICENSE`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/LICENSE) file for detail.