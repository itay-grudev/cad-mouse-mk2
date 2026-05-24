#include "controllers/LEDController.h"
#include "Config.h"
#include <math.h>

LEDController::LEDController()
    : ring_(Config::LED_COUNT, Config::PIN_LED_DATA,
            NEO_GRB + NEO_KHZ800) {}

void LEDController::fillAll(unsigned long color) {
  for (int i = 0; i < ring_.numPixels(); i++) {
    ring_.setPixelColor(i, color);
  }
}

unsigned long LEDController::toNeoColor(unsigned long color) {
  int r = (color >> 16) & 0xFF;
  int g = (color >> 8) & 0xFF;
  int b = color & 0xFF;
  return ring_.Color(r, g, b);
}

void LEDController::begin() {
  pinMode(Config::PIN_LED_LS, OUTPUT);
  digitalWrite(Config::PIN_LED_LS, Config::SWITCH_LOW);

  ring_.begin();
  ring_.setBrightness(Config::LED_BRIGHTNESS);
  ring_.show();
}

void LEDController::setPower(bool enabled) {
  if (enabled == isPowered_) {
    return;
  }

  isPowered_ = enabled;
  digitalWrite(Config::PIN_LED_LS, enabled ? Config::SWITCH_HIGH : Config::SWITCH_LOW);
  delay(10);
  
}

void LEDController::setSolid(unsigned long color) {
  mode_ = Mode::Solid;
  color_ = toNeoColor(color);
  setPower(true);
  fillAll(color_);
  ring_.show();
}

void LEDController::startSpinner(unsigned long color) {
  mode_ = Mode::Spinner;
  color_ = toNeoColor(color);
  spinnerIndex_ = 0;
  lastSpinnerStepMs_ = 0;
  setPower(true);
}

void LEDController::updateSpinner() {
  if (mode_ != Mode::Spinner) {
    return;
  }

  const unsigned long now = millis();

  fillAll(0);
  const int pixelCount = ring_.numPixels();
  if (pixelCount <= 0) {
    return;
  }

  // Preserve roughly the old rotation speed: 8 LEDs * 2 half-steps * 50ms.
  const float rotationPeriodMs = 500;
  const float phase = fmod(static_cast<float>(now), rotationPeriodMs) / rotationPeriodMs;
  const float idealPosition = phase * static_cast<float>(pixelCount);

  const uint8_t baseR = (color_ >> 16) & 0xFF;
  const uint8_t baseG = (color_ >> 8) & 0xFF;
  const uint8_t baseB = color_ & 0xFF;

  // Smooth distance-based illumination around the moving phase.
  // In LED units: 1 lights nearest two strongly, >1 adds a soft tail.
  const float falloffWidth = 1.0f;

  for (int i = 0; i < pixelCount; i++) {
    float distance = fabsf(static_cast<float>(i) - idealPosition);
    distance = fminf(distance, static_cast<float>(pixelCount) - distance);

    float weight = 0.0f;
    if (distance <= falloffWidth) {
      const float t = distance / falloffWidth;
      weight = 0.5f * (cosf(t * PI) + 1.0f);
    }

    const uint8_t r = static_cast<uint8_t>(baseR * weight + 0.5f);
    const uint8_t g = static_cast<uint8_t>(baseG * weight + 0.5f);
    const uint8_t b = static_cast<uint8_t>(baseB * weight + 0.5f);
    ring_.setPixelColor(i, ring_.Color(r, g, b));
  }

  ring_.show();
}

void LEDController::off() {
  mode_ = Mode::Off;
  fillAll(0);
  ring_.show();
  setPower(false);
}
