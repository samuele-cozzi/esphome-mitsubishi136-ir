#include "esphome.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Mitsubishi.h>

namespace esphome {
namespace mitsubishi136_ir {

class Mitsubishi136IRClimate : public climate::Climate, public Component {
 public:
  void setup() override;
  void dump_config() override;

  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

  void set_ir_pin(uint8_t pin) { ir_pin_ = pin; }

 protected:
  uint8_t ir_pin_ = 4;
  IRMitsubishi136 *ac_ = nullptr;
};

}  // namespace mitsubishi136_ir
}  // namespace esphome