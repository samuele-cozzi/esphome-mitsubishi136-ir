from esphome.components.mitsubishi136_ir.climate.mitsubishi136_ir_climate import Mitsubishi136IRClimate

PLATFORMS = ["climate"]
CONFIG_SCHEMA = PLATFORM_SCHEMA
register_component("mitsubishi136_ir", Mitsubishi136IRClimate)