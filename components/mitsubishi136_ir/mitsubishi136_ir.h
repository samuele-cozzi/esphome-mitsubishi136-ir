#pragma once

#include "esphome/components/climate/climate.h"
#include "esphome/core/component.h"

namespace esphome {
namespace mitsubishi136_ir {

class Mitsubishi136IRClimate : public climate::Climate, public Component {
 public:
  Mitsubishi136IRClimate() = default;

  void setup() override;
  void dump_config() override;

  climate::ClimateTraits get_traits() const override;

 protected:
  void control(const climate::ClimateCall &call) override;
};

}  // namespace mitsubishi136_ir
}  // namespace esphome
