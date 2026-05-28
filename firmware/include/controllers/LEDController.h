#pragma once

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class LEDController {
 public:
  enum class Mode {
    Off,
    Solid,
    Blink,
    FadeIn,
    FadeOut,
    Spinner,
  };

  LEDController();
  void begin();
  void solid(uint32_t color);
  void spinner(uint32_t color);
  void blink(uint32_t color, uint8_t blinkCount, uint16_t animationPeriod = 1500);
  void fadeIn(uint32_t color, uint16_t durationMs = 750);
  void fadeOut(uint16_t durationMs = 750);
  void update();
  void off();
  Mode mode() const { return mode_; }

 private:

  inline void updateSpinner();
  inline void updateBlink();
  inline void updateFade();

  Mode mode_ = Mode::Off;
  uint32_t color_ = 0;
  uint8_t remainingBlinks_ = 0;
  uint32_t animationStartedAt_ = 0;
  uint16_t animationPeriod_ = 1500;
  Adafruit_NeoPixel ring_;
};
