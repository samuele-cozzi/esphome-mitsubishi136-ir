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

CONFIG_SCHEMA = cv.All(
    climate._CLIMATE_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(Mitsubishi136IRClimate),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    """Generate C++ code for the climate platform."""
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
