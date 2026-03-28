#include "mitsubishi136_ir.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mitsubishi136_ir {

static const char *const TAG = "mitsubishi136_ir";

void Mitsubishi136IRClimate::setup() {
  ESP_LOGD(TAG, "Setting up Mitsubishi 136 IR Climate component on GPIO %d", this->ir_pin_);
  
  // Initialize the IRMitsubishi136 object
  this->ac_ = new IRMitsubishi136(this->ir_pin_);
  this->ac_->begin();
  
  // Set default state
  this->current_temperature = 20.0f;
  this->target_temperature = 24.0f;
  this->mode = climate::CLIMATE_MODE_OFF;
  this->action = climate::CLIMATE_ACTION_OFF;
}

void Mitsubishi136IRClimate::dump_config() {
  ESP_LOGCONFIG(TAG, "Mitsubishi 136 IR Climate:");
  ESP_LOGCONFIG(TAG, "  IR Pin: %d", this->ir_pin_);
}

climate::ClimateTraits Mitsubishi136IRClimate::get_traits() const {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
  traits.set_supports_target_temperature(true);
  traits.set_supports_action(true);
  traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_HEAT_COOL,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_FAN_ONLY,
      climate::CLIMATE_MODE_DRY,
  });
  traits.set_supported_swing_modes({
      climate::CLIMATE_SWING_OFF,
      climate::CLIMATE_SWING_VERTICAL,
  });
  traits.set_visual_min_temperature(16.0f);
  traits.set_visual_max_temperature(32.0f);
  traits.set_visual_target_temperature_step(0.5f);
  return traits;
}

void Mitsubishi136IRClimate::control(const climate::ClimateCall &call) {
  if (!this->ac_) {
    ESP_LOGW(TAG, "AC object not initialized");
    return;
  }

  // Update internal state
  if (call.get_mode().has_value()) {
    this->mode = call.get_mode().value();
  }
  if (call.get_target_temperature().has_value()) {
    this->target_temperature = call.get_target_temperature().value();
  }
  if (call.get_swing_mode().has_value()) {
    this->swing_mode = call.get_swing_mode().value();
  }

  this->send_ac_command_();
  this->publish_state();
}

void Mitsubishi136IRClimate::send_ac_command_() {
  if (!this->ac_) return;

  // Handle power and mode
  if (this->mode == climate::CLIMATE_MODE_OFF) {
    this->ac_->off();
    this->action = climate::CLIMATE_ACTION_OFF;
  } else {
    this->ac_->on();
    
    // Set mode based on climate mode
    switch (this->mode) {
      case climate::CLIMATE_MODE_HEAT:
        this->ac_->setMode(kMitsubishi136Heat);
        this->action = climate::CLIMATE_ACTION_HEATING;
        break;
      case climate::CLIMATE_MODE_COOL:
        this->ac_->setMode(kMitsubishi136Cool);
        this->action = climate::CLIMATE_ACTION_COOLING;
        break;
      case climate::CLIMATE_MODE_DRY:
        this->ac_->setMode(kMitsubishi136Dry);
        this->action = climate::CLIMATE_ACTION_IDLE;
        break;
      case climate::CLIMATE_MODE_FAN_ONLY:
        this->ac_->setMode(kMitsubishi136Fan);
        this->action = climate::CLIMATE_ACTION_FAN;
        break;
      case climate::CLIMATE_MODE_HEAT_COOL:
        this->ac_->setMode(kMitsubishi136Auto);
        this->action = climate::CLIMATE_ACTION_IDLE;
        break;
      default:
        break;
    }
  }

  // Set temperature
  uint8_t temp = (uint8_t)this->target_temperature;
  this->ac_->setTemp(temp);

  // Set fan speed
  this->ac_->setFan(kMitsubishi136FanAuto);

  // Set swing/vane mode
  if (this->swing_mode == climate::CLIMATE_SWING_VERTICAL) {
    this->ac_->setVane(kMitsubishi136VaneAuto);
  } else {
    this->ac_->setVane(kMitsubishi136VaneOff);
  }

  // Send the command
  this->ac_->send();

  ESP_LOGD(TAG, "Sent AC command - Mode: %d, Temp: %.1f°C, Swing: %d",
           (int)this->mode, this->target_temperature, (int)this->swing_mode);
}

}  // namespace mitsubishi136_ir
}  // namespace esphome
