import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import wifi
from esphome.const import CONF_HOST, CONF_ID, CONF_PORT

DEPENDENCIES = ["wifi"]
CODEOWNERS = ["@yourusername"]

loconet_tcp_ns = cg.esphome_ns.namespace("loconet_tcp")
LoconetTCP = loconet_tcp_ns.class_("LoconetTCP", cg.Component)

CONF_ON_RECEIVE = "on_receive"
CONF_ON_SENT_OK = "on_sent_ok"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(LoconetTCP),
        cv.Required(CONF_HOST): cv.string,
        cv.Required(CONF_PORT): cv.port,
        cv.Optional(CONF_ON_RECEIVE): automation.validate_automation(
            {
                cv.GenerateID("trigger_id"): cv.declare_id(automation.Trigger.template(cg.std_string)),
            }
        ),
        cv.Optional(CONF_ON_SENT_OK): automation.validate_automation(
            {
                cv.GenerateID("trigger_id"): cv.declare_id(automation.Trigger.template(cg.std_string)),
            }
        ),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_host(config[CONF_HOST]))
    cg.add(var.set_port(config[CONF_PORT]))

    for conf in config.get(CONF_ON_RECEIVE, []):
        trigger = cg.new_Pvariable(conf["trigger_id"])
        await automation.build_automation(trigger, [(cg.std_string, "x")], conf)
        cg.add(var.set_receive_trigger(trigger))

    for conf in config.get(CONF_ON_SENT_OK, []):
        trigger = cg.new_Pvariable(conf["trigger_id"])
        await automation.build_automation(trigger, [(cg.std_string, "x")], conf)
        cg.add(var.set_sent_ok_trigger(trigger))