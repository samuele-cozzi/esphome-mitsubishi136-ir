import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import climate, remote_transmitter
from esphome.const import CONF_ID

from . import mitsubishi136_ir_ns

AUTO_LOAD = ["climate"]

CONF_IR_PIN = "ir_pin"
CONF_IR_INVERTED = "ir_inverted"
CONF_TRANSMITTER_ID = "transmitter_id"
DEFAULT_IR_PIN = 4

Mitsubishi136IRClimate = mitsubishi136_ir_ns.class_(
    "Mitsubishi136IRClimate",
    cg.Component,
    climate.Climate,
)

CONFIG_SCHEMA = (
    climate.climate_schema(Mitsubishi136IRClimate)
    .extend(
        {
            cv.GenerateID(): cv.declare_id(Mitsubishi136IRClimate),
            cv.Optional(CONF_IR_PIN, default=DEFAULT_IR_PIN): cv.int_,
            cv.Optional(CONF_IR_INVERTED, default=False): cv.boolean,
            cv.Optional(CONF_TRANSMITTER_ID): cv.use_id(remote_transmitter.RemoteTransmitterComponent),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    cg.add(var.set_ir_pin(config[CONF_IR_PIN]))
    cg.add(var.set_ir_inverted(config[CONF_IR_INVERTED]))
    if CONF_TRANSMITTER_ID in config:
        transmitter = await cg.get_variable(config[CONF_TRANSMITTER_ID])
        cg.add(var.set_transmitter(transmitter))
    cg.add_library("crankyoldgit/IRremoteESP8266", "2.9.0")