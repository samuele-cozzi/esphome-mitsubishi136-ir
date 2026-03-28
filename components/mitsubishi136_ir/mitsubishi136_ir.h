#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"

// Forward declaration — full definition included only in the .cpp
class IRMitsubishi136;

namespace esphome {
namespace mitsubishi136_ir {

class Mitsubishi136IRClimate : public Component, public climate::Climate {
 public:
  void set_ir_pin(int ir_pin) { this->ir_pin_ = ir_pin; }

  void setup() override;
  void dump_config() override;
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

 protected:
  int ir_pin_{4};
  IRMitsubishi136 *ac_{nullptr};
};

}  // namespace mitsubishi136_ir
}  // namespace esphome