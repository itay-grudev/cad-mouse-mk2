#pragma once

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class LEDController {
 public:
  LEDController();
  void begin();
  void setSolid(unsigned long color);
  void startSpinner(unsigned long color);
  void startBlink(unsigned long color, uint8_t blinkCount);
  void updateSpinner();
  void updateBlink();
  void off();

 private:
  enum class Mode {
    Off,
    Solid,
    Blink,
    Spinner,
  };

  void setPower(bool enabled);
  void fillAll(unsigned long color);
  unsigned long toNeoColor(unsigned long color);

  bool isPowered_ = false;
  Mode mode_ = Mode::Off;
  unsigned long color_ = 0;
  uint8_t remainingBlinks_ = 0;
  unsigned long lastBlinkStartedAt_ = 0;
  Adafruit_NeoPixel ring_;
};
