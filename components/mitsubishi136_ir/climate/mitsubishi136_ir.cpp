#include "mitsubishi136_ir_climate.h"

namespace esphome {
namespace mitsubishi136_ir {

using namespace climate;

void Mitsubishi136IRClimate::setup() {
  ac_ = new IRMitsubishi136(ir_pin_);
  ac_->begin();
  this->mode = ClimateMode::CLIMATE_MODE_OFF;
  this->fan_mode = ClimateFanMode::CLIMATE_FAN_AUTO;
  this->target_temperature = 24;
  this->swing_mode = ClimateSwingMode::CLIMATE_SWING_OFF;

  ac_->off();
  ac_->setTemp((uint8_t) this->target_temperature);
  ac_->setFan(kMitsubishi136FanAuto);
}

void Mitsubishi136IRClimate::dump_config() {
  LOG_CLIMATE("", "Mitsubishi136 IR Climate", this);
  ESP_LOGCONFIG(TAG, "IR pin: %u", this->ir_pin_);
}

ClimateTraits Mitsubishi136IRClimate::traits() {
  ClimateTraits traits;
  traits.set_supports_current_temperature(false);
  traits.set_supports_two_point_target_temperature(false);
  traits.set_supports_action(false);
  traits.set_supports_away(false);

  traits.set_supported_modes({
    ClimateMode::CLIMATE_MODE_OFF,
    ClimateMode::CLIMATE_MODE_HEAT,
    ClimateMode::CLIMATE_MODE_COOL,
    ClimateMode::CLIMATE_MODE_DRY,
    ClimateMode::CLIMATE_MODE_FAN_ONLY,
  });

  traits.set_supported_fan_modes({
    ClimateFanMode::CLIMATE_FAN_AUTO,
    ClimateFanMode::CLIMATE_FAN_LOW,
    ClimateFanMode::CLIMATE_FAN_MEDIUM,
    ClimateFanMode::CLIMATE_FAN_HIGH,
  });

  traits.set_supported_swing_modes({
    ClimateSwingMode::CLIMATE_SWING_OFF,
    ClimateSwingMode::CLIMATE_SWING_VERTICAL,
  });

  traits.set_visual_min_temperature(16);
  traits.set_visual_max_temperature(30);
  traits.set_visual_temperature_step(1);

  return traits;
}

void Mitsubishi136IRClimate::control(const ClimateCall &call) {
  if (call.get_mode().has_value()) {
    auto mode = *call.get_mode();
    this->mode = mode;

    if (mode == ClimateMode::CLIMATE_MODE_OFF) {
      ac_->off();
    } else {
      ac_->on();
      if (mode == ClimateMode::CLIMATE_MODE_HEAT) {
        ac_->setMode(kMitsubishi136Heat);
      } else if (mode == ClimateMode::CLIMATE_MODE_COOL) {
        ac_->setMode(kMitsubishi136Cool);
      } else if (mode == ClimateMode::CLIMATE_MODE_DRY) {
        ac_->setMode(kMitsubishi136Dry);
      } else if (mode == ClimateMode::CLIMATE_MODE_FAN_ONLY) {
        ac_->setMode(kMitsubishi136Fan);
      }
    }
  }

  if (call.get_fan_mode().has_value()) {
    auto fan_mode = *call.get_fan_mode();
    this->fan_mode = fan_mode;

    if (fan_mode == ClimateFanMode::CLIMATE_FAN_AUTO) {
      ac_->setFan(kMitsubishi136FanAuto);
    } else if (fan_mode == ClimateFanMode::CLIMATE_FAN_LOW) {
      ac_->setFan(kMitsubishi136FanLow);
    } else if (fan_mode == ClimateFanMode::CLIMATE_FAN_MEDIUM) {
      ac_->setFan(kMitsubishi136FanMed);
    } else if (fan_mode == ClimateFanMode::CLIMATE_FAN_HIGH) {
      ac_->setFan(kMitsubishi136FanHigh);
    }
  }

  if (call.get_swing_mode().has_value()) {
    auto swing_mode = *call.get_swing_mode();
    this->swing_mode = swing_mode;

    if (swing_mode == ClimateSwingMode::CLIMATE_SWING_OFF) {
      ac_->setSwing(false);
    } else if (swing_mode == ClimateSwingMode::CLIMATE_SWING_VERTICAL) {
      ac_->setSwing(true);
    }
  }

  if (call.get_target_temperature().has_value()) {
    float temp = *call.get_target_temperature();
    temp = std::max(temp, 16.0f);
    temp = std::min(temp, 30.0f);
    this->target_temperature = temp;
    ac_->setTemp((uint8_t) temp);
  }

  ac_->send();
  this->publish_state();
}

}  // namespace mitsubishi136_ir
}  // namespace esphome