#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"

namespace mitsubishi136_ir {

class Mitsubishi136IRClimate : public esphome::Component, public esphome::climate::Climate {
 public:
  void set_ir_pin(int ir_pin) { this->ir_pin_ = ir_pin; }

  void setup() override;
  void dump_config() override;
  esphome::climate::ClimateTraits traits() override;
  void control(const esphome::climate::ClimateCall &call) override;

 protected:
  int ir_pin_{4};
};

}  // namespace mitsubishi136_ir