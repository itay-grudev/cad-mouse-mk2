#include "states/CalibratingState.h"

#include <Arduino.h>

#include "Config.h"
#include "Controllers.h"
#include "StateMachine.h"

void CalibratingState::enter() {
  sensorController.beginCalibration();
  motionController.reset();
  ledController.spinner(Config::LED_CALIBRATING_COLOR);
  startTimeMS = millis();
}

void CalibratingState::update() {
  inputController.update();
  ledController.update();
  sensorController.updateCalibration();

  if (sensorController.calibrationDone()) {
    stateMachine.changeState(&StateMachine::idleState);
    return;
  }


  if(millis() - startTimeMS >= Config::CALIBRATION_TIMEOUT_MS){
    stateMachine.changeState(&StateMachine::calibrationFailureState);
  }
}

void CalibratingState::exit() {}
