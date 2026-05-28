#include "states/CalibrationFailureState.h"

#include <Arduino.h>

#include "Config.h"
#include "Controllers.h"
#include "StateMachine.h"

void CalibrationFailureState::enter() {
  sensorController.stopCalibration();
  ledController.blink(Config::LED_ERROR_COLOR, 2);
}

void CalibrationFailureState::update() {
  inputController.update();

  if (inputController.takeCalibrationRequest())
    return stateMachine.changeState(&StateMachine::calibratingState);

  ledController.update();
}

void CalibrationFailureState::exit() {}
