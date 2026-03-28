import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import climate
from esphome.const import CONF_ID

from . import mitsubishi136_ir_ns

AUTO_LOAD = ["climate"]

CONF_IR_PIN = "ir_pin"
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
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    cg.add(var.set_ir_pin(config[CONF_IR_PIN]))
    cg.add_library(
        "IRremoteESP8266",
        None,
        "https://github.com/crankyoldgit/IRremoteESP8266",
    )