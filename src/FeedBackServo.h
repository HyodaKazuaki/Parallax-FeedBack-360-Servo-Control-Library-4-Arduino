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

#ifdef ARDUINO_ARCH_NRF52840
#define MAX_INTERRUPT_NUM 22
#endif

// If not listed above, leave it at 2.
// This is just a provisional value; some boards may require a larger value.
#ifndef MAX_INTERRUPT_NUM
#define MAX_INTERRUPT_NUM 2
#warning Unknown board, defining MAX_INTERRUPT_NUM to 2
#endif

#define FEEDBACKSERVO_REPEAT_ISR(M) \
    M(0) M(1) M(2) M(3) M(4) M(5) M(6) M(7) M(8) M(9) M(10) M(11) M(12) M(13) \
    M(14) M(15) M(16) M(17) M(18) M(19) M(20) M(21)

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

#define ISR_PROTOTYPE(n) static void isr ## n ();
    FEEDBACKSERVO_REPEAT_ISR(ISR_PROTOTYPE)
#undef ISR_PROTOTYPE
};

#endif
