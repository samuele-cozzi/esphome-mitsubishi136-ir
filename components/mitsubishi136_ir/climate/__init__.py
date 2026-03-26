"""
Mitsubishi136 IR climate.
"""
CONFIG_SCHEMA = esphome.configschema.CLIMATE_SCHEMA.extend({
    esphome.configschema.Optional("ir_pin"): esphome.configschema.gpio_pin_schema,
}).extend(esphome.configschema.UI_COLORSCHEME_CLIMATE)

async def to_code(config):
    var = esphome.new_variable("Mitsubishi136IRClimate", "climate")
    await esphome.register_parented(var)
    if CONF_IR_PIN in config:
        pin = await esphome.gpio_pin_expression(config[CONF_IR_PIN])
        esphome.register_parented(var.set_ir_pin(pin))