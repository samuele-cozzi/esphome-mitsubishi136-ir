#pragma once

#include "esphome/components/climate/climate.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include <IRremoteESP8266.h>
#include <ir_Mitsubishi.h>

namespace esphome {
namespace mitsubishi136_ir {

static const char *const TAG = "mitsubishi136_ir";

class Mitsubishi136IRClimate : public climate::Climate, public Component {
 public:
  Mitsubishi136IRClimate() = default;

  void set_ir_pin(uint16_t pin) { this->ir_pin_ = pin; }

  void setup() override {
    ESP_LOGD(TAG, "Setting up Mitsubishi 136 IR Climate component on GPIO %d", this->ir_pin_);
    
    this->ac_ = new IRMitsubishi136(this->ir_pin_);
    this->ac_->begin();
    
    this->current_temperature = 20.0f;
    this->target_temperature = 24.0f;
    this->mode = climate::CLIMATE_MODE_OFF;
    this->action = climate::CLIMATE_ACTION_OFF;
  }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "Mitsubishi 136 IR Climate:");
    ESP_LOGCONFIG(TAG, "  IR Pin: %d", this->ir_pin_);
  }

  climate::ClimateTraits get_traits() const override {
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

 protected:
  void control(const climate::ClimateCall &call) override {
    if (!this->ac_) {
      ESP_LOGW(TAG, "AC object not initialized");
      return;
    }

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

 private:
  uint16_t ir_pin_{4};
  IRMitsubishi136 *ac_{nullptr};

  void send_ac_command_() {
    if (!this->ac_) return;

    if (this->mode == climate::CLIMATE_MODE_OFF) {
      this->ac_->off();
      this->action = climate::CLIMATE_ACTION_OFF;
    } else {
      this->ac_->on();
      
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

    uint8_t temp = (uint8_t)this->target_temperature;
    this->ac_->setTemp(temp);
    this->ac_->setFan(kMitsubishi136FanAuto);

    if (this->swing_mode == climate::CLIMATE_SWING_VERTICAL) {
      this->ac_->setVane(kMitsubishi136VaneAuto);
    } else {
      this->ac_->setVane(kMitsubishi136VaneOff);
    }

    this->ac_->send();

    ESP_LOGD(TAG, "Sent AC command - Mode: %d, Temp: %.1f°C, Swing: %d",
             (int)this->mode, this->target_temperature, (int)this->swing_mode);
  }
};

}  // namespace mitsubishi136_ir
}  // namespace esphome
