#ifndef INC_STEPPER_LIB_H_
#define INC_STEPPER_LIB_H_

#include "Encoder_lib.h"
#include "TIM_lib.h"
#include "GPIO_lib.h"

// - EncoderIT for position feedback,
// - TimPWM for generating step pulses,
// - DigitalOut for controlling direction.
class StepperMotor {
public:
    StepperMotor(EncoderIT &encoder, TimPWM &pwm, DigitalOut &direction);
    ~StepperMotor();

    void setTargetPosition(int32_t position);

    void setSpeed(uint16_t stepsPerSecond);

    void update();

    int32_t getCurrentPosition();

    void stop();

private:
    EncoderIT &encoder;
    TimPWM &pwm;
    DigitalOut &direction;

    int32_t targetPosition; // Target position in steps
    uint16_t speed;         // Stepping speed (steps per second)
};

#endif // /* INC_STEPPER_LIB_H_ */