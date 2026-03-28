#pragma once

#include "esphome/components/climate/climate.h"
#include "esphome/core/component.h"
#include <IRremoteESP8266.h>
#include <ir_Mitsubishi.h>

namespace esphome {
namespace mitsubishi136_ir {

class Mitsubishi136IRClimate : public climate::Climate, public Component {
 public:
  Mitsubishi136IRClimate() = default;

  void set_ir_pin(uint16_t pin) { this->ir_pin_ = pin; }

  void setup() override;
  void dump_config() override;

  climate::ClimateTraits get_traits() const override;

 protected:
  void control(const climate::ClimateCall &call) override;

 private:
  uint16_t ir_pin_{4};
  IRMitsubishi136 *ac_{nullptr};
  
  void send_ac_command_();
};

}  // namespace mitsubishi136_ir
}  // namespace esphome
