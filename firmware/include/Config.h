#pragma once

#include <Arduino.h>

namespace Config {

const bool ENABLE_TELEMETRY = true;

// Hardware pins (XIAO RP2040)
const int PIN_RIGHT_BTN = D0;
const int PIN_LEFT_BTN = D2;
const int PIN_LED_DATA = D3;
const int PIN_LED_LS = D1;
const int PIN_MAG1_LS = D10;
const int PIN_MAG2_LS = D9;
const int PIN_MAG3_LS = D8;


// If using AP22816BKEWT-7 instead of AP22816AKEWT-7, as specified in the BOM
// the output to the switches needs to be inverted.
const bool SWITCH_HIGH = LOW;
const bool SWITCH_LOW = HIGH;

// Samples for calibration offset
const int ZERO_SAMPLES = 200;

// Gains and sign fixes
const float GAIN_T[3] = {28.0, 28.0, 24.0};
const float GAIN_R[3] = {18.0, 18.0, 20.0};
const int SIGN_AXIS[6] = {-1, +1, -1, +1, +1, +1};

// Dead zones
const float DEAD_T = 16.0;
const float DEAD_R = 20.0;

// Kalman filter tuning
// Process noise: how much we expect the true value to change per step.
// Higher = more responsive but noisier.
const float KALMAN_Q = 0.5;
// Measurement noise: how noisy the sensor readings are.
// Higher = smoother but more latency.
const float KALMAN_R = 4.0;

// Sensitivity curve exponent.
// 1.0 = linear, 3.0 = cubic (fine control at small deflections, fast at large).
const float SENSITIVITY_EXP = 2;

// Final axis output range
const float AXIS_LIMIT = 350.0;

// RGB LEDs
const int LED_COUNT = 8;
const unsigned long LED_IDLE_COLOR = 0x00FF00;
const unsigned long LED_CALIBRATING_COLOR = 0x0000FF;
const unsigned long LED_ERROR_COLOR = 0xFF0000;

// Calibration timeout period.  Display LED_ERROR_COLOR on LED ring if
// the calibration fails.
const unsigned long CALIBRATION_TIMEOUT_MS = 5000;

// FSM timing
const long IDLE_SLEEP_TIMEOUT_MS = 2 * 60 * 1000;

}  // namespace Config
