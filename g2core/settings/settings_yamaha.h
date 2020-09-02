#define SOFT_LIMIT_ENABLE           1

#define X_AXIS_MODE                 AXIS_STANDARD
#define X_VELOCITY_MAX              (800.0*60)
#define X_FEEDRATE_MAX              X_VELOCITY_MAX
#define X_TRAVEL_MIN                0.0
#define X_TRAVEL_MAX                685.0
#define X_JERK_MAX                  1000.0
#define X_JERK_HIGH_SPEED           X_JERK_MAX
#define X_HOMING_INPUT              1
#define X_HOMING_DIRECTION          1
#define X_SEARCH_VELOCITY           100.0
#define X_LATCH_VELOCITY            2.0
#define X_LATCH_BACKOFF             0.0
#define X_ZERO_BACKOFF              0.0

#define Y_AXIS_MODE                 AXIS_STANDARD
#define Y_VELOCITY_MAX              (800.0*60)
#define Y_FEEDRATE_MAX              Y_VELOCITY_MAX
#define Y_TRAVEL_MIN                -10.0
#define Y_TRAVEL_MAX                590.0
#define Y_JERK_MAX                  1000.0
#define Y_JERK_HIGH_SPEED           Y_JERK_MAX
#define Y_HOMING_INPUT              2
#define Y_HOMING_DIRECTION          1
#define Y_SEARCH_VELOCITY           100.0
#define Y_LATCH_VELOCITY            2.0
#define Y_LATCH_BACKOFF             0.0
#define Y_ZERO_BACKOFF              0.0

#define U_AXIS_MODE                 AXIS_STANDARD
#define U_VELOCITY_MAX              (400.0*60)
#define U_FEEDRATE_MAX              U_VELOCITY_MAX
#define U_TRAVEL_MIN                -76.0
#define U_TRAVEL_MAX                0.0
#define U_JERK_MAX                  1000.0
#define U_JERK_HIGH_SPEED           U_JERK_MAX
#define U_HOMING_INPUT              3
#define U_HOMING_DIRECTION          1
#define U_SEARCH_VELOCITY           50.0
#define U_LATCH_VELOCITY            2.0
#define U_LATCH_BACKOFF             0.0
#define U_ZERO_BACKOFF              0.0

#define V_AXIS_MODE                 AXIS_STANDARD
#define V_VELOCITY_MAX              (400.0*60)
#define V_FEEDRATE_MAX              V_VELOCITY_MAX
#define V_TRAVEL_MIN                -76.0
#define V_TRAVEL_MAX                0.0
#define V_JERK_MAX                  1000.0
#define V_JERK_HIGH_SPEED           V_JERK_MAX
#define V_HOMING_INPUT              4
#define V_HOMING_DIRECTION          1
#define V_SEARCH_VELOCITY           50.0
#define V_LATCH_VELOCITY            2.0
#define V_LATCH_BACKOFF             0.0
#define V_ZERO_BACKOFF              0.0

#define A_AXIS_MODE                 AXIS_STANDARD
#define A_VELOCITY_MAX              (400.0*60)
#define A_FEEDRATE_MAX              A_VELOCITY_MAX
#define A_TRAVEL_MIN                0.0
#define A_TRAVEL_MAX                0.0
#define A_JERK_MAX                  1000.0
#define A_JERK_HIGH_SPEED           A_JERK_MAX
#define A_HOMING_INPUT              5
#define A_HOMING_DIRECTION          1
#define A_SEARCH_VELOCITY           50.0
#define A_LATCH_VELOCITY            2.0
#define A_LATCH_BACKOFF             0.0
#define A_ZERO_BACKOFF              0.0

#define B_AXIS_MODE                 AXIS_STANDARD
#define B_VELOCITY_MAX              (400.0*60)
#define B_FEEDRATE_MAX              B_VELOCITY_MAX
#define B_TRAVEL_MIN                0.0
#define B_TRAVEL_MAX                0.0
#define B_JERK_MAX                  1000.0
#define B_JERK_HIGH_SPEED           B_JERK_MAX
#define B_HOMING_INPUT              6
#define B_HOMING_DIRECTION          1
#define B_SEARCH_VELOCITY           50.0
#define B_LATCH_VELOCITY            2.0
#define B_LATCH_BACKOFF             0.0
#define B_ZERO_BACKOFF              0.0

#define M1_MOTOR_MAP                AXIS_X_EXTERNAL
#define M1_STEPS_PER_UNIT           100
#define M1_MICROSTEPS               -1

#define M2_MOTOR_MAP                AXIS_Y_EXTERNAL
#define M2_STEPS_PER_UNIT           100
#define M2_MICROSTEPS               -1

#define M3_MOTOR_MAP                AXIS_U_EXTERNAL
#define M3_STEPS_PER_UNIT           106
#define M3_MICROSTEPS               -1

#define M4_MOTOR_MAP                AXIS_V_EXTERNAL
#define M4_STEPS_PER_UNIT           106
#define M4_MICROSTEPS               -1

#define M5_MOTOR_MAP                AXIS_A_EXTERNAL
#define M5_STEPS_PER_UNIT           50
#define M5_MICROSTEPS               -1

#define M6_MOTOR_MAP                AXIS_B_EXTERNAL
#define M6_STEPS_PER_UNIT           50
#define M6_MICROSTEPS               -1

#define DI12_MODE                   IO_ACTIVE_HIGH          /* USB VBUS Detect */
#define DI5_FUNCTION                INPUT_FUNCTION_NONE
