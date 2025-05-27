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
}

void FeedBackServo::setServoControl(byte servoPinNumber)
{
    // Servo control pin attach
    Parallax_.attach(servoPinNumber);
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
        Parallax_.writeMicroseconds(1490);
        return;
    }

    float output = constrain(errorAngle * Kp_, -200.0, 200.0);
    float offset = (errorAngle > 0) ? 30.0 : -30.0;
    float value = output + offset;

    Parallax_.writeMicroseconds(1490 - value);
}

int FeedBackServo::getAngle()
{
    return angle_;
}

int FeedBackServo::Angle()
{
    return getAngle();
}

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
}

void FeedBackServo::handleFeedback()
{
    if (digitalRead(feedbackPinNumber_))
    {
        rise_ = micros();
        tLow_ = rise_ - fall_;

        int tCycle = tHigh_ + tLow_;
        if ((tCycle < 1000) || (tCycle > 1200))
            return;

        float dc = (DUTY_SCALE * tHigh_) / (float)tCycle;
        float theta = ((dc - DC_MIN) * UNITS_FC) / (DC_MAX - DC_MIN);

        if (theta < 0.0)
            theta = 0.0;
        else if (theta > (UNITS_FC - 1.0))
            theta = UNITS_FC - 1.0;

        if ((theta < Q2_MIN) && (thetaPre_ > Q3_MAX))
            turns_++;
        else if ((thetaPre_ < Q2_MIN) && (theta > Q3_MAX))
            turns_--;

        if (turns_ >= 0)
            angle_ = (turns_ * UNITS_FC) + theta;
        else if (turns_ < 0)
            angle_ = ((turns_ + 1) * UNITS_FC) - (UNITS_FC - theta);

        thetaPre_ = theta;
    }
    else
    {
        fall_ = micros();
        tHigh_ = fall_ - rise_;
    }
}

// ISR delegates
void FeedBackServo::isr0() { if (instances[0]) instances[0]->handleFeedback(); }
void FeedBackServo::isr1() { if (instances[1]) instances[1]->handleFeedback(); }
void FeedBackServo::isr2() { if (instances[2]) instances[2]->handleFeedback(); }
void FeedBackServo::isr3() { if (instances[3]) instances[3]->handleFeedback(); }
void FeedBackServo::isr4() { if (instances[4]) instances[4]->handleFeedback(); }
void FeedBackServo::isr5() { if (instances[5]) instances[5]->handleFeedback(); }