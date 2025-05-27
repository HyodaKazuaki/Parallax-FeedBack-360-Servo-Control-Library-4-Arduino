#ifndef FEEDBACK360_CONTROL_LIBRARY
#define FEEDBACK360_CONTROL_LIBRARY

#include <Arduino.h>
#include <Servo.h>

#ifdef ARDUINO_AVR_UNO
#define MAX_INTERRUPT_NUM 2
#endif

#ifdef ARDUINO_AVR_LEONARDO
#define MAX_INTERRUPT_NUM 5
#endif

#ifdef ARDUINO_AVR_MEGA2560
#define MAX_INTERRUPT_NUM 6
#endif

// If not listed above, leave it at 2.
// This is just a provisional value; some boards may require a larger value.
#ifndef MAX_INTERRUPT_NUM
#define MAX_INTERRUPT_NUM 2
#endif

class FeedBackServo
{
public:
    FeedBackServo(byte feedbackPinNumber);
    void setServoControl(byte servoPinNumber = 3);
    void setKp(float Kp = 1.0);
    void setActive(bool isActive);
    void setTarget(int target);
    void update(int threshold = 4);
    int getAngle();

    // These functions are left for compatibility.
    int Angle();

private:
    void checkPin(byte pinNumber);
    void handleFeedback();
    Servo Parallax;
    byte feedbackPinNumber;
    byte interruptNumber;

    float Kp_ = 1.0;
    bool isActive_ = true;
    int targetAngle_;

    volatile int angle_ = 0;
    float thetaPre = 0;
    unsigned int tHigh = 0, tLow = 0;
    unsigned long rise = 0, fall = 0;
    int turns = 0;

    static const int unitsFC = 360;
    static const float dcMin;
    static const float dcMax;
    static const int dutyScale = 1;
    static const int q2min;
    static const int q3max;

    static FeedBackServo* instances[MAX_INTERRUPT_NUM];
    static void isr0();
    static void isr1();
    static void isr2();
    static void isr3();
    static void isr4();
    static void isr5();
};

#endif