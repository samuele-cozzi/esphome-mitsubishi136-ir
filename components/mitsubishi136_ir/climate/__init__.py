"""
ESPHome climate platform for Mitsubishi 136-bit IR climate control.
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID

from .. import mitsubishi136_ir_ns

Mitsubishi136IRClimate = mitsubishi136_ir_ns.class_(
    "Mitsubishi136IRClimate", climate.Climate, cg.Component
)

CONF_IR_PIN = "ir_pin"

CONFIG_SCHEMA = cv.All(
    climate._CLIMATE_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(Mitsubishi136IRClimate),
            cv.Optional(CONF_IR_PIN, default=4): cv.int_range(min=0, max=39),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    """Generate C++ code for the climate platform."""
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    
    # Set IR pin
    ir_pin = config[CONF_IR_PIN]
    cg.add(var.set_ir_pin(ir_pin))
