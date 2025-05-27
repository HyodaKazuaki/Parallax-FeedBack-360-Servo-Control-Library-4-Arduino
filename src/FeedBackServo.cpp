// FeedBack360 Servo Control Library 4 Arduino
// Controll.cpp
// Copyright © Hyoda Kazuaki
// Parallax Feedback 360° High Speed Servo is made by Parallax Inc.
// This library is released under the MIT License.

#include <Arduino.h>
#include <Servo.h>
#include "FeedBackServo.h"

const float FeedBackServo::DC_MIN = 0.029;
const float FeedBackServo::DC_MAX = 0.971;
const int FeedBackServo::Q2_MIN = FeedBackServo::UNITS_FC / 4;
const int FeedBackServo::Q3_MAX = FeedBackServo::Q2_MIN * 3;

FeedBackServo* FeedBackServo::instances[MAX_INTERRUPT_NUM] = { nullptr };

FeedBackServo::FeedBackServo(byte _feedbackPinNumber)
{
    // feedback pin number validation
    checkPin(_feedbackPinNumber);
    feedbackPinNumber = _feedbackPinNumber;

    // convert feedback pin number to interrupt number for use on attachInterrupt function
    interruptNumber = digitalPinToInterrupt(feedbackPinNumber);

    if (interruptNumber < MAX_INTERRUPT_NUM) {
        instances[interruptNumber] = this;
        switch (interruptNumber)
        {
        case 0: attachInterrupt(0, isr0, CHANGE); break;
        case 1: attachInterrupt(1, isr1, CHANGE); break;
        case 2: attachInterrupt(2, isr2, CHANGE); break;
        case 3: attachInterrupt(3, isr3, CHANGE); break;
        case 4: attachInterrupt(4, isr4, CHANGE); break;
        case 5: attachInterrupt(5, isr5, CHANGE); break;
        }
    }
}

void FeedBackServo::setServoControl(byte servoPinNumber)
{
    // Servo control pin attach
    Parallax.attach(servoPinNumber);
}

void FeedBackServo::setKp(float Kp)
{
    FeedBackServo::Kp_ = Kp;
}

void FeedBackServo::setActive(bool isActive)
{
    isActive_ = isActive;
}

void FeedBackServo::setTarget(int target)
{
    targetAngle_ = target;
}

void FeedBackServo::update(int threshold = 4)
{
    if (isActive_ == false) return;

    int errorAngle = targetAngle_ - angle_;
    if (abs(errorAngle) <= threshold)
    {
        Parallax.writeMicroseconds(1490);
        return;
    }

    float output = constrain(errorAngle * Kp_, -200.0, 200.0);
    float offset = (errorAngle > 0) ? 30.0 : -30.0;
    float value = output + offset;

    Parallax.writeMicroseconds(1490 - value);
}

int FeedBackServo::getAngle()
{
    return angle_;
}

int FeedBackServo::Angle()
{
    return getAngle();
}

void FeedBackServo::checkPin(byte _feedbackPinNumber)
{
// Check pin number
#ifdef ARDUINO_AVR_UNO
    if (_feedbackPinNumber != 2 &&
        _feedbackPinNumber != 3)
        exit(1);
#endif
#ifdef ARDUINO_AVR_LEONARDO
    if (_feedbackPinNumber != 0 &&
        _feedbackPinNumber != 1 &&
        _feedbackPinNumber != 2 &&
        _feedbackPinNumber != 3 &&
        _feedbackPinNumber != 7)
        exit(1);
#endif
#ifdef ARDUINO_AVR_MEGA2560
    if (_feedbackPinNumber != 2 &&
        _feedbackPinNumber != 3 &&
        _feedbackPinNumber != 18 &&
        _feedbackPinNumber != 19 &&
        _feedbackPinNumber != 20 &&
        _feedbackPinNumber != 21)
        exit(1);
#endif
}

void FeedBackServo::handleFeedback()
{
    if (digitalRead(feedbackPinNumber))
    {
        rise = micros();
        tLow = rise - fall;

        int tCycle = tHigh + tLow;
        if ((tCycle < 1000) || (tCycle > 1200))
            return;

        float dc = (DUTY_SCALE * tHigh) / (float)tCycle;
        float theta = ((dc - DC_MIN) * UNITS_FC) / (DC_MAX - DC_MIN);

        if (theta < 0.0)
            theta = 0.0;
        else if (theta > (UNITS_FC - 1.0))
            theta = UNITS_FC - 1.0;

        if ((theta < Q2_MIN) && (thetaPre > Q3_MAX))
            turns++;
        else if ((thetaPre < Q2_MIN) && (theta > Q3_MAX))
            turns--;

        if (turns >= 0)
            angle_ = (turns * UNITS_FC) + theta;
        else if (turns < 0)
            angle_ = ((turns + 1) * UNITS_FC) - (UNITS_FC - theta);

        thetaPre = theta;
    }
    else
    {
        fall = micros();
        tHigh = fall - rise;
    }
}

// ISR delegates
void FeedBackServo::isr0() { if (instances[0]) instances[0]->handleFeedback(); }
void FeedBackServo::isr1() { if (instances[1]) instances[1]->handleFeedback(); }
void FeedBackServo::isr2() { if (instances[2]) instances[2]->handleFeedback(); }
void FeedBackServo::isr3() { if (instances[3]) instances[3]->handleFeedback(); }
void FeedBackServo::isr4() { if (instances[4]) instances[4]->handleFeedback(); }
void FeedBackServo::isr5() { if (instances[5]) instances[5]->handleFeedback(); }