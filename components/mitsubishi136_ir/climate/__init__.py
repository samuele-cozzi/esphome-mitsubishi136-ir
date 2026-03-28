# components/mitsubishi136_ir/climate/__init__.py
import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import climate
from esphome.const import CONF_ID

CONF_IR_PIN = "ir_pin"
DEFAULT_IR_PIN = 4

mitsubishi136_ir_ns = cg.esphome_ns.namespace("mitsubishi136_ir")
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