#include "protobuf_wrapper.hpp"
#include "pb_encode.h"
#include "esp_log.h"
#include <cstdio>

namespace industrial {

std::vector<uint8_t> ProtobufSerializer::serialize_sensor_data(const std::string& id, 
                                                              float temp, 
                                                              float hum, 
                                                              uint32_t uptime) {
    // 1. Initialize struct (Exactly matching the Message Name)
    SensorData message = SensorData_init_default;
    
    // 2. Fill data (Names must match the .proto file fields exactly)
    std::snprintf(message.device_id, sizeof(message.device_id), "%s", id.c_str());
    message.temperature = temp;
    message.humidity = hum;  // Changed from .hum to .humidity
    message.uptime_ms = uptime;

    // 3. Encode to binary
    uint8_t buffer[128];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&stream, SensorData_fields, &message)) {
        ESP_LOGE("PROTO", "Encoding failed!");
        return {};
    }

    return std::vector<uint8_t>(buffer, buffer + stream.bytes_written);
}

} // namespace industrial
