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

#ifdef ARDUINO_SAMD_NANO_33_IOT
#define MAX_INTERRUPT_NUM 9
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
    void setServoControl(byte servoPinNumber);
    void setKp(float Kp = 1.0);
    void setActive(bool isActive);
    void setTarget(int target);
    void update(int threshold = 4);
    int getAngle();

    // These functions are left for compatibility.
    void rotate(int degree, int threshold = 4);
    int Angle();

private:
    void checkPin(byte pinNumber);
    void handleFeedback();
    void updateAngleFromPWM();

    Servo parallax_;
    byte feedbackPinNumber_;
    byte interruptNumber_;

    float Kp_ = 1.0;
    bool isActive_ = true;
    int targetAngle_;

    volatile int angle_ = 0;
    float thetaPre_ = 0;
    unsigned int tHigh_ = 0, tLow_ = 0;
    unsigned long rise_ = 0, fall_ = 0;
    int turns_ = 0;

    volatile bool feedbackUpdated_ = false;

    static const int UNITS_FC = 360; // Full Circle
    static const float DC_MIN;
    static const float DC_MAX;
    static const int DUTY_SCALE = 1;
    static const int Q2_MIN;
    static const int Q3_MAX;

    static FeedBackServo* instances[MAX_INTERRUPT_NUM];
    static void isr0();
    static void isr1();
    static void isr2();
    static void isr3();
    static void isr4();
    static void isr5();
};

#endif
