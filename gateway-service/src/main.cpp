#include <iostream>
#include "mqtt_wrapper.hpp"
#include "sensor_data.pb.h"

int main() {
    const std::string BROKER_ADDR = "ssl://localhost:8883";
    const std::string CA_PATH = "/etc/mosquitto/certs/ca.crt";

    std::cout << "=== Industrial Gateway Service Starting ===" << std::endl;

    // 1. Initialize MQTT via RAII
    industrial::MqttWrapper mqtt_handler(BROKER_ADDR, "gateway_main");

    // 2. Attempt Connection (This uses the certs we put in the overlay)
    // Note: This will fail on your Ubuntu host because it lacks the certs at that path,
    // but the binary will be correctly compiled for the RPi4.
    mqtt_handler.connect(CA_PATH);

    std::cout << "Gateway Service logic active. Press Enter to stop." << std::endl;
    std::cin.get();

    // When 'mqtt_handler' goes out of scope here, the destructor 
    // automatically closes the connection. No 'client.stop()' needed.
    return 0;
}
