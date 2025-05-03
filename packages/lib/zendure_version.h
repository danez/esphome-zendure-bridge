#pragma once
#include <string>

namespace zendure {
    namespace version {
        std::string decode_version(int encoded) {
            int major = encoded >> 12;
            int minor = (encoded >> 6) & 0x3F;
            int patch = encoded & 0x3F;

            char buffer[10];
            snprintf(buffer, sizeof(buffer), "%d.%d.%d", major, minor, patch);

            return std::string(buffer);
        }
    }
}
