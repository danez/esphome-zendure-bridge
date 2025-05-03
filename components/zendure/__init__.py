import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.core import coroutine_with_priority

json_ns = cg.esphome_ns.namespace("zendure")

CONFIG_SCHEMA = cv.All(
    cv.Schema({}),
)

async def to_code(config):
    cg.add_global(json_ns.using)
