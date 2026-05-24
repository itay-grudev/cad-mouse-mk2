#pragma once

#include "State.h"

class CalibrationFailureState : public State {
 public:
  void enter() override;
  void update() override;
  void exit() override;
};
