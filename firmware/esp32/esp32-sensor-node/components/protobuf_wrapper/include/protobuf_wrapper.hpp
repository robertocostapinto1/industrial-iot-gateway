#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "sensor_data.pb.h"

namespace industrial {

class ProtobufSerializer {
public:
    static std::vector<uint8_t> serialize_sensor_data(const std::string& id, 
                                                     float temp, 
                                                     float hum, 
                                                     uint32_t uptime);
};

} // namespace industrial
