#include <iostream>
#include <thread>
#include <vector>
#include <csignal>
#include <atomic>
#include "mqtt_wrapper.hpp"
#include "safe_queue.hpp"
#include "sensor_data.pb.h"

// Use an atomic boolean to handle clean shutdown via Linux signals
std::atomic<bool> running{true};

void signal_handler(int signal) {
    running = false;
}

int main() {
    // Setup signal handling (for systemctl stop)
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::cout << "=== Industrial Gateway: Daemon Ingestion Engine ===" << std::endl;

    industrial::SafeQueue telemetry_queue;

    // 1. Start Worker Thread
    std::thread worker_thread([&telemetry_queue]() {
        while (running) {
            auto raw_data_opt = telemetry_queue.pop();
            if (!raw_data_opt.has_value()) break;

            industrial::SensorData msg;
            if (msg.ParseFromString(raw_data_opt.value())) {
                std::cout << "[WORKER] Ingested -> Node: " << msg.device_id() 
                          << " | Temp: " << msg.temperature() << "C" << std::endl;
            }
        }
        std::cout << "[WORKER] Thread exiting..." << std::endl;
    });

    // 2. Start MQTT (RAII)
    const std::string BROKER_ADDR = "ssl://localhost:8883";
    industrial::MqttWrapper mqtt_handler(BROKER_ADDR, "gateway_daemon", telemetry_queue);
    
    if (mqtt_handler.connect("/etc/mosquitto/certs/ca.crt")) {
        std::cout << "[MAIN] Connected to broker. Service ready." << std::endl;
    }

    // 3. Keep Main Alive without std::cin
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 4. Graceful Shutdown
    std::cout << "[MAIN] Initiating shutdown..." << std::endl;
    telemetry_queue.abort();
    if (worker_thread.joinable()) worker_thread.join();

    return 0;
}
