#include "Stepper_lib.h"

StepperMotor::StepperMotor(EncoderIT &enc, TimPWM &pwmTimer, DigitalOut &dir)
    : encoder(enc),
      pwm(pwmTimer),
      direction(dir),
      targetPosition(0),
      speed(0)
{
    encoder.start();
    pwm.start();
}

StepperMotor::~StepperMotor() {

}

void StepperMotor::setTargetPosition(int32_t position) {
    targetPosition = position;
}

void StepperMotor::setSpeed(uint16_t stepsPerSecond) {
    speed = stepsPerSecond;
    // Adjust the PWM frequency to match the desired step rate.
    pwm.setFrequency(stepsPerSecond);
}

int32_t StepperMotor::getCurrentPosition() {
    return encoder.read();
}

void StepperMotor::update() {
    int32_t currentPos = getCurrentPosition();

    if (currentPos < targetPosition) {
        // Set direction for forward movement
        direction.write(GPIO_PIN_SET);
        pwm.setFrequency(speed);
        pwm.start();
    }
    else if (currentPos > targetPosition) {
        // Set direction for reverse movement
        direction.write(GPIO_PIN_RESET);
        pwm.setFrequency(speed);
        pwm.start();
    }
    else {
        // Target reached, stop the motor
        pwm.stop();
    }
}

void StepperMotor::stop() {
    pwm.stop();
}
