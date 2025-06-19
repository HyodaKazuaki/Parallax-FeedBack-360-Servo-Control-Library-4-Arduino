// FeedBack360 Servo Control Library 4 Arduino
// FeedBackServo.cpp
// Copyright © Hyoda Kazuaki
// Parallax Feedback 360° High Speed Servo is made by Parallax Inc.
// This library is released under the MIT License.

#include "FeedBackServo.h"

FeedBackServo* FeedBackServo::instances[MAX_INTERRUPT_NUM] = { nullptr };

// Constructs a new Parallax feedback servo object using a defined feedback pin number.
// The chosen pin MUST support interrupts. Check the attachInterrupt() documentation for a list of valid interrupt pins on respective Arduino products. 
FeedBackServo::FeedBackServo(byte feedbackPinNumber)
{
    // feedback pin number validation
    checkPin(feedbackPinNumber);
    feedbackPinNumber_ = feedbackPinNumber;

    // convert feedback pin number to interrupt number for use on attachInterrupt function
    interruptNumber_ = digitalPinToInterrupt(feedbackPinNumber_);

    if (interruptNumber_ < MAX_INTERRUPT_NUM) {
        instances[interruptNumber_] = this;
        switch (interruptNumber_)
        {
            case 0: attachInterrupt(0, isr0, CHANGE); break;
            case 1: attachInterrupt(1, isr1, CHANGE); break;
            case 2: attachInterrupt(2, isr2, CHANGE); break;
            case 3: attachInterrupt(3, isr3, CHANGE); break;
            case 4: attachInterrupt(4, isr4, CHANGE); break;
            case 5: attachInterrupt(5, isr5, CHANGE); break;
        }
    }

    // Defaults the mode to coast upon initialization
    setMode(Constants::Mode::COAST_CONTROL);
}

// Assigns the servo a signal pin.
void FeedBackServo::setServoControl(byte servoPinNumber)
{
    // Servo control pin attach.
    parallax_.attach(servoPinNumber);
}

// Sets the proportional constant for a closed PID loop.
void FeedBackServo::setKp(float Kp)
{
    FeedBackServo::Kp_ = Kp;
}

// Can be used to enable or disable a Parallax servo motor, toggling responsiveness to the .update() function
void FeedBackServo::setMode(Constants::Mode mode)
{
    mode_ = mode;
}

// Returns current mode of the motor
Constants::Mode FeedBackServo::getMode(){
    return mode_;
}

// Sets target of servo in degrees. 
// For the target to be followed, .update() must be called every loop. If rapidly switching between targets, a "Blink Without Delay" system is recommended (example given in 'Rotation.ino').
void FeedBackServo::setTarget(int target)
{
    targetAngle_ = target;
}

// Returns current calculated error
int FeedBackServo::getError(){
    return targetAngle_ - angle_;
}

// The calling servo motor will mirror the motion actuated by the given servo object.
// NOTE: Requires position control to be enabled by the calling motor.
// TODO: Requires rework to reduces latency.
void FeedBackServo::follow(FeedBackServo servo){
    setTarget(servo.getAngle());
}

// Servo locks to the angle last recorded before calling this function.
void FeedBackServo::lock(){
    setTarget(getAngle());
}


// Updates the error calculated by the encoder; necessary for position control.
// NOTE: Using delay() will also cease update calculations for the Parallax servo motors, potentially causing undesired behavior.
void FeedBackServo::update(int threshold)
{
    // Update angle based on the latest PWM feedback
    updateAngleFromPWM();

    if (checkMode(Constants::Mode::POSITION_CONTROL)) return;

    int errorAngle = targetAngle_ - angle_;
    if (abs(errorAngle) <= threshold)
    {
        parallax_.writeMicroseconds(1490);
        return;
    }

    // NOTE: Using simple P-control.
    // TODO: PID control may improve stability and response.
    float output = constrain(errorAngle * Kp_, -200.0, 200.0);
    float offset = (errorAngle > 0) ? 30.0 : -30.0;
    float value = output + offset;

    parallax_.writeMicroseconds(1490 - value);
}

// Returns current angle of the Feedback Servo
int FeedBackServo::getAngle()
{
    return angle_;
}

/*
 * Legacy blocking rotation function for compatibility.
 *
 * This function sets the target angle and actively blocks until the target is reached
 * within the specified threshold.
 *
 * Note:
 * - Suitable only when controlling a single motor.
 * - Not recommended in multi-motor systems.
 * - For non-blocking control, call `setTarget()` and use `update()` in the main loop.
 */
[[deprecated("Use non-blocking control functions, setTarget(int) and update().")]]
void FeedBackServo::rotate(int degree, int threshold)
{
    setTarget(degree);

    while (abs(degree - angle_) >= threshold)
    {
        update(threshold);
    }
}

[[deprecated("Use getAngle() for future support.")]]
int FeedBackServo::Angle()
{
    return getAngle();
}


// Sets the current direction of rotation.
// Constants::Direction::FORWARD will rotate in the counterclockwise direction.
// Constants::Direction::BACKWARD wil rotate in the clockwise direction.
void FeedBackServo::setDirection(Constants::Direction direction)
{
    switch(direction)
    {
        case Constants::Direction::FORWARD:
            direction_ = false; // Counter-clockwise
            break;
        case Constants::Direction::BACKWARD:
            direction_ = true; // Clockwise
            break;
    }
}

// Determines whether current mode facilitates desired form of motion.
bool FeedBackServo::checkMode(Constants::Mode mode){    
    return mode_ != mode;

}

// Rotate servo according to a given percent representing power output.
// Direction can be toggled through setDirection().
// Note: Recommended for speed control. Does NOT utilize position control.
void FeedBackServo::move(float percent){

    if(checkMode(Constants::Mode::VELOCITY_CONTROL)) return;

    float speed;
    if (percent == 0.0f){
        speed = 0.0f;
    } else {
        speed = direction_ ? Constants::LOWER_NEUTRAL_BOUND - Constants::POWER_CONVERSION * percent : 
                             Constants::UPPER_NEUTRAL_BOUND + Constants::POWER_CONVERSION * percent;
    }
    parallax_.writeMicroseconds(speed);
}

// Rotate servo according to a desired rotations per minute.
// Direction can be toggled through setDirection().
// Note: Recommended for speed control. Does NOT utilize position control.
void FeedBackServo::move(unsigned int rpm){

    if(checkMode(Constants::Mode::VELOCITY_CONTROL)) return;

    float speed;
    switch (rpm){
        case 0:
            speed = 0;
            break;
        default:
            speed = direction_ ? Constants::LOWER_NEUTRAL_BOUND - Constants::RPM_CONVERSION * rpm : 
                                 Constants::UPPER_NEUTRAL_BOUND + Constants::RPM_CONVERSION * rpm;
            break;
    }
    parallax_.writeMicroseconds(speed);
}

// Controls raw speed of the servo. Not recommended unless familiarized with specification sheet. 
void FeedBackServo::writeMicroseconds(int value){
    parallax_.writeMicroseconds(value);
}

// Checks whether given pin number is a valid interrupt pin.
void FeedBackServo::checkPin(byte feedbackPinNumber)
{
// Check pin number
#ifdef ARDUINO_AVR_UNO
    if (feedbackPinNumber != 2 &&
        feedbackPinNumber != 3)
        exit(1);
#endif
#ifdef ARDUINO_AVR_LEONARDO
    if (feedbackPinNumber != 0 &&
        feedbackPinNumber != 1 &&
        feedbackPinNumber != 2 &&
        feedbackPinNumber != 3 &&
        feedbackPinNumber != 7)
        exit(1);
#endif
#ifdef ARDUINO_AVR_MEGA2560
    if (feedbackPinNumber != 2 &&
        feedbackPinNumber != 3 &&
        feedbackPinNumber != 18 &&
        feedbackPinNumber != 19 &&
        feedbackPinNumber != 20 &&
        feedbackPinNumber != 21)
        exit(1);
#endif
#ifdef ARDUINO_SAMD_NANO_33_IOT
    if (feedbackPinNumber != 2 &&
        feedbackPinNumber != 3 &&
        feedbackPinNumber != 9 &&
        feedbackPinNumber != 10 &&
        feedbackPinNumber != 11 &&
        feedbackPinNumber != 13 &&
        feedbackPinNumber != 15 && // A1
        feedbackPinNumber != 19 && // A5
        feedbackPinNumber != 21)   // A7
        exit(1);
#endif
}

// Calculate duty cycle
void FeedBackServo::handleFeedback()
{
    // Interrupt Service Routine: triggered on pin CHANGE
    // Records timing only — actual decoding happens in main loop
    if (digitalRead(feedbackPinNumber_))
    {
        rise_ = micros();
        tLow_ = rise_ - fall_;
    }
    else
    {
        fall_ = micros();
        tHigh_ = fall_ - rise_;
        feedbackUpdated_ = true;
    }
}

// Calculates the current angle of the Parallax servo motor using the duty cycle based on the Parallax servo motor specifications sheet.
void FeedBackServo::updateAngleFromPWM()
{
    if (!feedbackUpdated_) return;
    feedbackUpdated_ = false;

    int tCycle = tHigh_ + tLow_;
    if ((tCycle < 1000) || (tCycle > 1200))
        return;

    float dc = (Constants::DUTY_SCALE * tHigh_) / (float)tCycle;
    float theta = ((dc - Constants::DC_MIN) * Constants::UNITS_FC) / (Constants::DC_MAX - Constants::DC_MIN);

    if (theta < 0.0)
        theta = 0.0;
    else if (theta > (Constants::UNITS_FC - 1.0))
        theta = Constants::UNITS_FC - 1.0;

    if ((theta < Constants::Q2_MIN) && (thetaPre_ > Constants::Q3_MAX))
        turns_++;
    else if ((thetaPre_ < Constants::Q2_MIN) && (theta > Constants::Q3_MAX))
        turns_--;

    if (turns_ >= 0)
        angle_ = (turns_ * Constants::UNITS_FC) + theta;
    else if (turns_ < 0)
        angle_ = ((turns_ + 1) * Constants::UNITS_FC) - (Constants::UNITS_FC - theta);

    thetaPre_ = theta;
}

// ISR delegates
void FeedBackServo::isr0() { if (instances[0]) instances[0]->handleFeedback(); }
void FeedBackServo::isr1() { if (instances[1]) instances[1]->handleFeedback(); }
void FeedBackServo::isr2() { if (instances[2]) instances[2]->handleFeedback(); }
void FeedBackServo::isr3() { if (instances[3]) instances[3]->handleFeedback(); }
void FeedBackServo::isr4() { if (instances[4]) instances[4]->handleFeedback(); }
void FeedBackServo::isr5() { if (instances[5]) instances[5]->handleFeedback(); }
