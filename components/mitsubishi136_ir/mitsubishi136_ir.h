#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"
#include "esphome/components/remote_base/remote_base.h"
#include "ir_Mitsubishi.h"

namespace esphome {
namespace mitsubishi136_ir {

class Mitsubishi136IRClimate : public Component, public climate::Climate {
 public:
  void set_ir_pin(int ir_pin) { this->ir_pin_ = ir_pin; }
  void set_ir_inverted(bool inverted) { this->ir_inverted_ = inverted; }
  void set_transmitter(remote_transmitter::RemoteTransmitterComponent *transmitter) { this->transmitter_ = transmitter; }

  void setup() override;
  void dump_config() override;
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

 protected:
  void transmit_state();
  void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
                   const uint16_t onemark, const uint32_t onespace,
                   const uint16_t zeromark, const uint32_t zerospace,
                   const uint16_t footermark, const uint32_t gap,
                   const uint8_t *message, const uint16_t nbytes,
                   const uint16_t frequency);
  void sendData(remote_base::RemoteTransmitData *data,
                const uint16_t onemark, const uint32_t onespace,
                const uint16_t zeromark, const uint32_t zerospace,
                const uint8_t message, const uint8_t nbits);

  int ir_pin_{4};
  bool ir_inverted_{false};
  IRMitsubishi136 *ac_{nullptr};
  remote_transmitter::RemoteTransmitterComponent *transmitter_{nullptr};
};

}  // namespace mitsubishi136_ir
}  // namespace esphome