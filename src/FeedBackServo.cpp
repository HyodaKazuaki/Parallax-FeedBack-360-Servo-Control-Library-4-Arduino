// FeedBack360 Servo Control Library 4 Arduino
// Controll.cpp
// Copyright © Hyoda Kazuaki
// Parallax Feedback 360° High Speed Servo is made by Parallax Inc.
// This library is released under the MIT License.

#include <Arduino.h>
#include <Servo.h>
#include "FeedBackServo.h"

const int Kp = 1;
const int unitsFC = 360;
const float dcMin = 0.029;
const float dcMax = 0.971;
const int dutyScale = 1;
const int q2min = unitsFC / 4;
const int q3max = q2min * 3;

byte feedbackPinNumber = 2;
volatile int angle;
float thetaPre;
unsigned int tHigh, tLow;
unsigned long rise, fall;
int turns = 0;

FeedBackServo::FeedBackServo(byte _feedbackPinNumber = 2)
{
    // feedback pin number validation
    pinCheck(_feedbackPinNumber);
    feedbackPinNumber = _feedbackPinNumber;

    // convert feedback pin number for use on attachInterrupt function
    byte internalPinNumber = convertFeedbackPin();

    attachInterrupt(internalPinNumber, feedback, CHANGE);
}

void FeedBackServo::setServoControl(byte servoPinNumber = 3)
{
    // Servo control pin attach
    Parallax.attach(servoPinNumber);
}

void FeedBackServo::rotate(int degree, int threshold = 4)
{
    int output, offset, value;
    for(int errorAngle = degree - angle; abs(errorAngle) > threshold;) {
        output = errorAngle * Kp;
        if(output > 200)
            output = 200;
        if(output < -200)
            output = -200;
        if(errorAngle > 0)
            offset = 30;
        else if(errorAngle < 0)
            offset = -30;
        else
            offset = 0;
        
        value = output + offset;
        Parallax.writeMicroseconds(1490 - value);
    }
}

int FeedBackServo::Angle()
{
    return angle;
}

void FeedBackServo::pinCheck(byte _feedbackPinNumber)
{
    // Check pin number
    #ifdef ARDUINO_AVR_UNO
    if(_feedbackPinNumber != 2 && _feedbackPinNumber != 3)
        exit(1);
    #endif
    #ifdef ARDUINO_AVR_LEONARDO
    if(_feedbackPinNumber != 0 && _feedbackPinNumber != 1 && _feedbackPinNumber != 2 && _feedbackPinNumber != 3 && _feedbackPinNumber != 7)
        exit(1);
    #endif
}

byte FeedBackServo::convertFeedbackPin()
{
    byte internalPinNumber;
    #ifdef ARDUINO_AVR_UNO
    switch (feedbackPinNumber)
    {
    case 2:
        internalPinNumber = 0;
        break;
    case 3:
        internalPinNumber = 1;
        break;
    default:
        internalPinNumber = 0;
        break;
    }
    #endif
    #ifdef ARDUINO_AVR_LEONARDO
    switch (feedbackPinNumber)
    {
    case 0:
        internalPinNumber = 2;
        break;
    case 1:
        internalPinNumber = 3;
        break;
    case 2:
        internalPinNumber = 1;
        break;
    case 3:
        internalPinNumber = 0;
        break;
    case 7:
        internalPinNumber = 4;
        break;
    default:
        internalPinNumber = 0;
        break;
    }
    #endif
    return internalPinNumber;
}

static void feedback() {
    if(digitalRead(feedbackPinNumber)) {
        rise = micros();
        tLow = rise - fall;

        int tCycle = tHigh + tLow;
        if((tCycle < 1000) || (tCycle > 1200))
            return;
        
        float dc = (dutyScale * tHigh) / (float)tCycle;
        float theta = ((dc - dcMin) * unitsFC) / (dcMax - dcMin);

        if(theta < 0.0)
            theta = 0.0;
        else if(theta > (unitsFC - 1.0))
            theta = unitsFC - 1.0;

        if((theta < q2min) && (thetaPre > q3max))
            turns++;
        else if((thetaPre < q2min) && (theta > q3max))
            turns--;

        if(turns >= 0)
            angle = (turns * unitsFC) + theta;
        else if(turns < 0)
            angle = ((turns + 1) * unitsFC) - (unitsFC - theta);

        thetaPre = theta;
    } else {
        fall = micros();
        tHigh = fall - rise;
    }
}