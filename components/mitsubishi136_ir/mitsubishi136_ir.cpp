#include "mitsubishi136_ir.h"
#include "esphome/core/log.h"
#include "esphome/components/climate/climate_mode.h"
#include <Arduino.h>

namespace esphome {
namespace mitsubishi136_ir {

static const char *const TAG = "mitsubishi136_ir.climate";

void Mitsubishi136IRClimate::setup() {
  ESP_LOGI(TAG, "Setting up Mitsubishi136 IR Climate on GPIO %d", this->ir_pin_);

  // ── Hardware pin test ──────────────────────────────────────────────────────
  // Pulse the IR LED 3× at boot (visible on a phone camera) to confirm the
  // LED is actually connected to the configured GPIO before the RMT takes over.
  pinMode(static_cast<uint8_t>(this->ir_pin_), OUTPUT);
  for (int i = 0; i < 3; i++) {
    digitalWrite(static_cast<uint8_t>(this->ir_pin_), HIGH);
    delay(200);
    digitalWrite(static_cast<uint8_t>(this->ir_pin_), LOW);
    delay(200);
  }
  ESP_LOGI(TAG, "GPIO %d blink test done — check camera for 3 IR flashes", this->ir_pin_);

  this->ac_ = new IRMitsubishi136(static_cast<uint16_t>(this->ir_pin_), this->ir_inverted_);
  this->ac_->begin();
  ESP_LOGI(TAG, "IR transmitter initialised (inverted=%s)", this->ir_inverted_ ? "yes" : "no");

  this->mode = climate::CLIMATE_MODE_OFF;
  this->target_temperature = 24.0f;
  this->current_temperature = 24.0f;
  this->fan_mode = climate::CLIMATE_FAN_AUTO;
  this->publish_state();
}

void Mitsubishi136IRClimate::dump_config() {
  ESP_LOGCONFIG(TAG, "Mitsubishi136 IR Climate");
  ESP_LOGCONFIG(TAG, "  IR Pin: %d", this->ir_pin_);
  ESP_LOGCONFIG(TAG, "  Inverted: %s", this->ir_inverted_ ? "yes" : "no");
}

climate::ClimateTraits Mitsubishi136IRClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.add_feature_flags(climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);
  traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_DRY,
      climate::CLIMATE_MODE_FAN_ONLY,
      climate::CLIMATE_MODE_AUTO,
  });
  traits.set_supported_fan_modes({
      climate::CLIMATE_FAN_AUTO,
      climate::CLIMATE_FAN_LOW,
      climate::CLIMATE_FAN_MEDIUM,
      climate::CLIMATE_FAN_HIGH,
  });
  traits.set_visual_min_temperature(16.0f);
  traits.set_visual_max_temperature(30.0f);
  traits.set_visual_temperature_step(1.0f);
  return traits;
}

void Mitsubishi136IRClimate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value())
    this->mode = *call.get_mode();
  if (call.get_target_temperature().has_value())
    this->target_temperature = *call.get_target_temperature();
  if (call.get_fan_mode().has_value())
    this->fan_mode = *call.get_fan_mode();

  ESP_LOGI(TAG, "control() called: mode=%d temp=%.1f fan=%d",
           static_cast<int>(this->mode), this->target_temperature,
           static_cast<int>(this->fan_mode.value_or(climate::CLIMATE_FAN_AUTO)));

  if (ac_ == nullptr) {
    ESP_LOGE(TAG, "IR transmitter not initialised! Did setup() run?");
    return;
  }

  // ── Power ────────────────────────────────────────────────────────────────
  if (this->mode == climate::CLIMATE_MODE_OFF) {
    ESP_LOGI(TAG, "Sending OFF");
    this->ac_->off();
  } else {
    this->ac_->on();

    // ── Operating mode ──────────────────────────────────────────────────
    switch (this->mode) {
      case climate::CLIMATE_MODE_COOL:
        this->ac_->setMode(kMitsubishi136Cool);
        break;
      case climate::CLIMATE_MODE_HEAT:
        this->ac_->setMode(kMitsubishi136Heat);
        break;
      case climate::CLIMATE_MODE_DRY:
        this->ac_->setMode(kMitsubishi136Dry);
        break;
      case climate::CLIMATE_MODE_FAN_ONLY:
        this->ac_->setMode(kMitsubishi136Fan);
        break;
      case climate::CLIMATE_MODE_AUTO:
      default:
        this->ac_->setMode(kMitsubishi136Auto);
        break;
    }

    // ── Fan speed ────────────────────────────────────────────────────────
    switch (this->fan_mode.value_or(climate::CLIMATE_FAN_AUTO)) {
      case climate::CLIMATE_FAN_LOW:
        this->ac_->setFan(kMitsubishi136FanMin);
        break;
      case climate::CLIMATE_FAN_MEDIUM:
        this->ac_->setFan(kMitsubishi136FanMed);
        break;
      case climate::CLIMATE_FAN_HIGH:
        this->ac_->setFan(kMitsubishi136FanMax);
        break;
      case climate::CLIMATE_FAN_AUTO:
      default:
        this->ac_->setFan(kMitsubishi136FanMed);
        break;
    }

    // ── Temperature ──────────────────────────────────────────────────────
    this->ac_->setTemp(static_cast<uint8_t>(this->target_temperature));

    ESP_LOGI(TAG, "Sending IR frame: mode=%d temp=%.1f fan=%d",
             static_cast<int>(this->mode), this->target_temperature,
             static_cast<int>(this->fan_mode.value_or(climate::CLIMATE_FAN_AUTO)));
  }

  ESP_LOGI(TAG, "Calling ac_->send() on GPIO %d", this->ir_pin_);
  this->ac_->send();
  ESP_LOGI(TAG, "ac_->send() completed");

  this->publish_state();
}

}  // namespace mitsubishi136_ir
}  // namespace esphome