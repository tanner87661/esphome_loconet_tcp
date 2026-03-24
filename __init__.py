import esphome.config_validation as cv
import esphome.codegen as cg

CODEOWNERS = ["@vinsce"]
DEPENDENCIES = [ ]
AUTO_LOAD = [ ]
MULTI_CONF = True

# C++ namespace
ns = cg.esphome_ns.namespace("loconet_tcp")
MyExternalComponent = ns.class_("LoconetTCP", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(LoconetTCP),
    # Schema definition, containing the options available for the component
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    # Code generation
    pass