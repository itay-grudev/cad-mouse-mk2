#include "states/SleepState.h"

#include <Arduino.h>

#include "Config.h"
#include "Controllers.h"
#include "StateMachine.h"

void SleepState::enter() {
  ledController.off();
}

void SleepState::update() {
  inputController.update();

  if (inputController.takeActivity())
    return stateMachine.changeState(&StateMachine::idleState);

  // Wake up if we see significant movement on any axis
  float raw[9] = {};
  if (!sensorController.readRaw(raw)) {
    return;
  }
  const float* baseline = sensorController.baseline();
  for (int i = 0; i < 9; i++) {
    if (fabsf(raw[i] - baseline[i]) > Config::SLEEP_KNOB_DEVIATION_THRESHOLD)
      return stateMachine.changeState(&StateMachine::idleState);
  }

}

void SleepState::exit() {}
