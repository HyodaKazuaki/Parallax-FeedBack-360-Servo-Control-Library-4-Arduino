#ifndef FEEDBACK360_CONTROL_LIBRARY
#define FEEDBACK360_CONTROL_LIBRARY
#include <Arduino.h>
#include <Servo.h>

class FeedBackServo {
    public:
        FeedBackServo(byte feedbackPinNumber);
        void setServoControl(byte servoPinNumber);
        void rotate(int degree, int threshold);
        int Angle();
    
    private:
        void pinCheck(byte pinNumber);
        byte convertFeedbackPin();
        void static feedback();
        static Servo Parallax;
        static byte feedbackPinNumber;
        static volatile int angle;
        static float thetaP;
        static unsigned long tHigh, tLow;
        static unsigned long rise, fall;
        static int turns;
        static const int Kp;
        static const int unitsFC;
        static const float dcMin;
        static const float dcMax;
        static const int dutyScale;
        static const int q2min;
        static const int q3max;
};

#endif