#include <iostream>
#include <thread>
#include <vector>
#include "mqtt_wrapper.hpp"
#include "safe_queue.hpp"
#include "sensor_data.pb.h"

int main() {
    std::cout << "=== Industrial Gateway: Multi-threaded Ingestion Engine ===" << std::endl;

    // 1. Shared Thread-Safe Queue
    industrial::SafeQueue telemetry_queue;

    // 2. The Consumer (Worker) Thread
    std::thread worker_thread([&telemetry_queue]() {
        while (true) {
            // pop() now returns a std::optional
            auto raw_data_opt = telemetry_queue.pop();
            
            // If the optional is empty, it means the queue was aborted
            if (!raw_data_opt.has_value()) {
                std::cout << "[WORKER] Shutdown signal received. Exiting..." << std::endl;
                break; 
            }

            // Extract the string from the optional
            std::string raw_data = std::move(raw_data_opt.value());
            
            industrial::SensorData msg;
            if (msg.ParseFromString(raw_data)) {
                std::cout << "[WORKER] Ingested -> Node: " << msg.device_id() 
                          << " | Temp: " << msg.temperature() << "C" 
                          << " | Uptime: " << msg.uptime_ms() << "ms" << std::endl;
            } else {
                std::cerr << "[WORKER] Deserialization failed!" << std::endl;
            }
        }
    });

    // 3. Initialize MQTT via RAII
    // Note: We pass the queue by reference so the MQTT thread can push to it
    const std::string BROKER_ADDR = "ssl://localhost:8883";
    industrial::MqttWrapper mqtt_handler(BROKER_ADDR, "gateway_main", telemetry_queue);

    // In production, this would use the baked-in RootFS path
    mqtt_handler.connect("/etc/mosquitto/certs/ca.crt");

    std::cout << "[MAIN] Gateway logic active. Monitoring industrial/telemetry/#" << std::endl;
    std::cout << "Press Enter to shut down the service..." << std::endl;
    std::cin.get();

    // 4. Graceful Shutdown Sequence
    std::cout << "[MAIN] Initiating shutdown..." << std::endl;
    telemetry_queue.abort(); // Signal the worker thread to stop
    worker_thread.join();    // Wait for worker to finish current task

    return 0;
}
