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
    void rotate(int degree, int threshold = 4);
    int Angle();

private:
    void pinCheck(byte pinNumber);
    void HandleFeedback();
    Servo Parallax;
    byte feedbackPinNumber;
    byte interruptNumber;
    static volatile int angle;
    static float thetaPre;
    static unsigned int tHigh, tLow;
    static unsigned long rise, fall;
    static int turns;
    static float Kp;
    static const int unitsFC;
    static const float dcMin;
    static const float dcMax;
    static const int dutyScale;
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