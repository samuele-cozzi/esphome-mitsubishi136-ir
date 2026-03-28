import esphome.codegen as cg

CODEOWNERS = []
AUTO_LOAD = []

mitsubishi136_ir_ns = cg.esphome_ns.namespace("mitsubishi136_ir")
Mitsubishi136IRClimate = mitsubishi136_ir_ns.class_(
    "Mitsubishi136IRClimate"
)