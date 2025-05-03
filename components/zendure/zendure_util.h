#pragma once
#include "esphome/components/json/json_util.h"
#include <string>
#include <functional>

namespace esphome {
    namespace zendure {
        using sensor_handler_t = std::function<void(int)>;
        using sensor_handler_map_t = std::unordered_map<std::string, sensor_handler_t>;

        using battery_handler_t = std::function<void(JsonObject)>;
        using battery_handler_map_t = std::unordered_map<std::string, battery_handler_t>;

        constexpr float TEMPERATURE_OFFSET = 273.15f;
        constexpr float TEMPERATURE_SCALE = 10.0f;
        constexpr float VOLTAGE_SCALE = 100.0f;
        constexpr float PERCENT_SCALE = 10.0f;
        constexpr float TIME_HOURS_SCALE = 60.0f;

        std::string decode_version(const int encoded);

        void find_and_set_device_key(
            JsonObject& root,
            std::string& global_device_id,
            const std::string& device_id
        );

        // Function to iterate over a JSON object and apply handlers
        void handle_sensor_object(
            JsonObject& obj,
            const sensor_handler_map_t& handlers,
            const std::string& device_id
        );

        void handle_device_message(
            JsonObject& root,
            const sensor_handler_map_t& property_handlers,
            const battery_handler_map_t& pack_handlers,
            std::string& global_device_id,
            const std::string& device_id
        );
    }
}
