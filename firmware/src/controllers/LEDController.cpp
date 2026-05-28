#include "controllers/LEDController.h"
#include "Config.h"
#include <math.h>

inline void fillAll(Adafruit_NeoPixel& ring, uint32_t color) {
  for (int i = 0; i < ring.numPixels(); ++i) {
    ring.setPixelColor(i, color);
  }
}

LEDController::LEDController()
    : ring_(Config::LED_COUNT, Config::PIN_LED_DATA,
            NEO_GRB + NEO_KHZ800) {}

void LEDController::begin() {
  pinMode(Config::PIN_LED_LS, OUTPUT);
  digitalWrite(Config::PIN_LED_LS, HIGH != Config::USE_INVERTED_SWITCHES);
  ring_.begin();
}

void LEDController::off() {
  mode_ = Mode::Off;
  ring_.clear();
  ring_.show();
}

void LEDController::solid(uint32_t color) {
  mode_ = Mode::Solid;
  color_ = color;
  fillAll(ring_, color_);
  ring_.show();
}

void LEDController::spinner(uint32_t color) {
  mode_ = Mode::Spinner;
  color_ = color;
  ring_.setBrightness(255);
}

void LEDController::update() {
  switch (mode_) {
  case Mode::Solid:
    break;
  case Mode::Blink:
    updateBlink();
    break;
  case Mode::Spinner:
    updateSpinner();
    break;
  case Mode::FadeIn:
  case Mode::FadeOut:
    updateFade();
    break;
  case Mode::Off:
  default:
    break;
  } 
}

inline void LEDController::updateSpinner() {
  const uint32_t now = millis();

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

  for (int i = 0; i < pixelCount; ++i) {
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

void LEDController::blink(uint32_t color, uint8_t blinkCount, uint16_t animationPeriod){
  mode_ = Mode::Blink;
  color_ = color;
  remainingBlinks_ = blinkCount;
  ring_.setBrightness(255);
  animationStartedAt_ = 0;
  animationPeriod_ = animationPeriod;
}

inline void LEDController::updateBlink() {
  if( animationStartedAt_ == 0 || millis() - animationStartedAt_ >= animationPeriod_ ){
    --remainingBlinks_;
   
    if (remainingBlinks_ == 255)
      return off(); 

    animationStartedAt_ = millis();
  }

  float brightness = sinf((static_cast<float>(max(millis() - animationStartedAt_, 1)) / animationPeriod_) * PI) * 100;
  fillAll(ring_, color_);
  ring_.setBrightness(brightness);
  ring_.show();
}

void LEDController::fadeIn(uint32_t color, uint16_t durationMs) {
  mode_ = Mode::FadeIn;
  color_ = color;
  animationStartedAt_ = millis();
  animationPeriod_ = durationMs;
}

void LEDController::fadeOut(uint16_t durationMs) {
  mode_ = Mode::FadeOut;
  animationStartedAt_ = millis();
  animationPeriod_ = durationMs;
}

inline void LEDController::updateFade() {
  const float progress = static_cast<float>(millis() - animationStartedAt_) / animationPeriod_;
  if (progress >= 1.0f) {
    if (mode_ == Mode::FadeIn)
      return solid(color_);
    else
      return off();
  }

  float brightness = (mode_ == Mode::FadeIn) ? (progress * 100) : ((1.0f - progress) * 100);
  fillAll(ring_, color_);
  ring_.setBrightness(brightness);
  ring_.show();
}