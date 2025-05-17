#ifndef FEEDBACK360_CONTROL_LIBRARY
#define FEEDBACK360_CONTROL_LIBRARY

#include <Arduino.h>
#include <Servo.h>

class FeedBackServo
{
public:
    FeedBackServo(byte feedbackPinNumber);
    void setServoControl(byte servoPinNumber = 3);
    void setKp(float _Kp = 1.0);
    void SetActive(bool isActive);
    void SetTarget(int target);
    void Update(int threshold = 4);
    int Angle();

private:
    void pinCheck(byte pinNumber);
    void HandleFeedback();
    Servo Parallax;
    byte feedbackPinNumber;
    byte interruptNumber;

    int targetAngle_;
    bool isActive_ = true;

    volatile int angle = 0;
    float thetaPre = 0;
    unsigned int tHigh = 0, tLow = 0;
    unsigned long rise = 0, fall = 0;
    int turns = 0;
    float Kp = 1.0;

    static const int unitsFC = 360;
    static const float dcMin;
    static const float dcMax;
    static const int dutyScale = 1;
    static const int q2min;
    static const int q3max;

    static FeedBackServo* instances[6];
    static void isr0();
    static void isr1();
    static void isr2();
    static void isr3();
    static void isr4();
    static void isr5();
};

#endif