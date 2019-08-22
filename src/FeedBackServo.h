#ifndef FEEDBACK360_CONTROL_LIBRARY
#define FEEDBACK360_CONTROL_LIBRARY
#include <Arduino.h>
#include <Servo.h>

class FeedBackServo {
    public:
        FeedBackServo(byte feedbackPinNumber = 2);
        void setServoControl(byte servoPinNumber = 3);
        void setKp(float _Kp = 1.0);
        void rotate(int degree, int threshold = 4);
        int Angle();
    
    private:
        void pinCheck(byte pinNumber);
        byte convertFeedbackPin();
        void static feedback();
        static Servo Parallax;
        static byte feedbackPinNumber;
        static volatile int angle;
        static float thetaPre;
        static unsigned int tHigh, tLow;
        static unsigned long rise, fall;
        static int turns;
        static float Kp;
        static const int unitsFC = 360;
        static const float dcMin = 0.029;
        static const float dcMax = 0.971;
        static const int dutyScale = 1;
        static const int q2min = unitsFC / 4;
        static const int q3max = q2min * 3;
};

#endif