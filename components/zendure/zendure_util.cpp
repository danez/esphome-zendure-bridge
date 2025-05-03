#include "esphome/components/json/json_util.h"
#include "esphome/core/log.h"
#include "zendure_util.h"
#include <string>
#include <functional>

namespace esphome {
    namespace zendure {

        std::string decode_version(const int encoded) {
            int major = encoded >> 12;
            int minor = (encoded >> 6) & 0x3F;
            int patch = encoded & 0x3F;

            char buffer[10];
            snprintf(buffer, sizeof(buffer), "%d.%d.%d", major, minor, patch);

            return std::string(buffer);
        }

        void find_and_set_device_key(
            JsonObject& root,
            std::string& global_device_id,
            const std::string& device_id
        ) {
            if (global_device_id.empty() && root.containsKey("deviceId")) {
                global_device_id = root["deviceId"].as<std::string>();
                ESP_LOGI(device_id.c_str(), "Found device ID: %s", global_device_id.c_str());
            }
        }

        void handle_sensor_object(
            JsonObject& obj,
            const sensor_handler_map_t& handlers,
            const std::string& device_id
        ) {
            for (JsonObject::iterator it = obj.begin(); it != obj.end(); ++it) {
                std::string key = it->key().c_str();
                // 99% of values in zendures JSON are integers, so far only serial_numbers are not, but we do not report them
                int value = it->value().as<int>();

                auto handler = handlers.find(key);
                if (handler != handlers.end()) {
                    handler->second(value);
                } else {
                    ESP_LOGW(device_id.c_str(), "Unhandled sensor key '%s' with value '%d'", key.c_str(), value);
                }
            }
        }

        void handle_device_message(
            JsonObject& root,
            const sensor_handler_map_t& property_handlers,
            const battery_handler_map_t& pack_handlers,
            std::string& global_device_id,
            const std::string& device_id
        ) {
            find_and_set_device_key(root, global_device_id, device_id);

            if (root["method"] == "report") {
                JsonArray pack_data = root["packData"].as<JsonArray>();
                if (!pack_data.isNull()) {
                    for (JsonObject pack : pack_data) {
                        std::string sn = pack["sn"].as<std::string>();
                        if (!sn.empty()) {
                            auto pack_handler = pack_handlers.find(sn);
                            if (pack_handler != pack_handlers.end()) {
                                pack_handler->second(pack);
                            }
                        }
                    }
                }

                JsonObject properties = root["properties"];
                if (!properties.isNull()) {
                    handle_sensor_object(properties, property_handlers, device_id);
                }
            }

        }
    }
}
