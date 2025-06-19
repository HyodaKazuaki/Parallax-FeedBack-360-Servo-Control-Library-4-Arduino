#ifndef CONSTANTS
#define CONSTANTS

class Constants {
  public:
    enum class Mode {
      POSITION_CONTROL, // Comands the servo to only respond to position-based input
      VELOCITY_CONTROL, // Comands the servo to only respond to velocity-based input
      COAST_CONTROL // Commands the servo to be unresponsive to any input
    };

    enum class Direction {
        FORWARD, // Commands the servo motor to rotate in the counter-clockwise direction from a top-down perspective
        BACKWARD // Commands the servo motor to rotate in the clockwise direction from a top-down perspective
    };
    
    static const int UNITS_FC = 360; // Full Circle
    static constexpr float DC_MIN = 0.029;
    static constexpr float DC_MAX = 0.971;
    static const int DUTY_SCALE = 1;
    static const int Q2_MIN = UNITS_FC / 4;
    static const int Q3_MAX = Q2_MIN * 3;

    static const int LOWER_NEUTRAL_BOUND = 1480;
    static const int UPPER_NEUTRAL_BOUND = 1520;

    static constexpr float POWER_CONVERSION = 2.2;
    static constexpr float RPM_CONVERSION = 1.4667;
};

#endif
