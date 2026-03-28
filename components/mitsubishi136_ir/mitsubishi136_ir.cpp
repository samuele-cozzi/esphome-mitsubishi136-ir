#include "mitsubishi136_ir.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mitsubishi136_ir {

static const char *const TAG = "mitsubishi136_ir.climate";

void Mitsubishi136IRClimate::setup() {
  this->mode = climate::CLIMATE_MODE_OFF;
  this->target_temperature = 24.0f;
  this->current_temperature = 24.0f;
  this->fan_mode = climate::CLIMATE_FAN_AUTO;
  this->publish_state();
}

void Mitsubishi136IRClimate::dump_config() {
  ESP_LOGCONFIG(TAG, "Mitsubishi136 IR Climate");
  ESP_LOGCONFIG(TAG, "  IR Pin: %d", this->ir_pin_);
}

climate::ClimateTraits Mitsubishi136IRClimate::traits() {
  auto traits = climate::ClimateTraits();

  traits.set_supports_current_temperature(true);
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
  if (call.get_mode().has_value()) {
    this->mode = *call.get_mode();
  }
  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();
  }
  if (call.get_fan_mode().has_value()) {
    this->fan_mode = *call.get_fan_mode();
  }
  this->publish_state();
}

}  // namespace mitsubishi136_ir
}  // namespace esphome