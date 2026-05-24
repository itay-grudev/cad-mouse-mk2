#include "states/CalibrationFailureState.h"

#include <Arduino.h>

#include "Config.h"
#include "Controllers.h"
#include "StateMachine.h"

void CalibrationFailureState::enter() {
  sensorController.stopCalibration();
  ledController.startBlink(Config::LED_ERROR_COLOR, 2);
}

void CalibrationFailureState::update() {
  inputController.update();

  if (inputController.takeCalibrationRequest())
    return stateMachine.changeState(&StateMachine::calibratingState);

  ledController.updateBlink();
}

void CalibrationFailureState::exit() {}
