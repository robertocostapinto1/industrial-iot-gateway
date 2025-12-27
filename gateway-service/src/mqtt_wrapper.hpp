#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <mqtt/async_client.h>

namespace industrial {

class MqttWrapper {
public:
    MqttWrapper(const std::string& server_address, const std::string& client_id) {
        // Use std::unique_ptr for zero-raw-pointer constraint
        client_ = std::make_unique<mqtt::async_client>(server_address, client_id);
        std::cout << "MQTT Wrapper initialized for: " << server_address << std::endl;
    }

    // Connect using TLS
    bool connect(const std::string& ca_path) {
        auto ssl_opts = mqtt::ssl_options_builder()
                        .trust_store(ca_path)
                        .finalize();

        auto conn_opts = mqtt::connect_options_builder()
                         .ssl(std::move(ssl_opts))
                         .clean_session(true)
                         .finalize();

        try {
            std::cout << "Connecting to Secure Broker..." << std::endl;
            client_->connect(conn_opts)->wait();
            std::cout << "Connected Successfully." << std::endl;
            return true;
        } catch (const mqtt::exception& exc) {
            std::cerr << "MQTT Connection Error: " << exc.what() << std::endl;
            return false;
        }
    }

    // RAII Destructor: Ensures graceful disconnection
    ~MqttWrapper() {
        if (client_ && client_->is_connected()) {
            std::cout << "RAII: Gracefully disconnecting MQTT..." << std::endl;
            client_->disconnect()->wait();
        }
    }

    // Accessor for the underlying client
    mqtt::async_client& get_client() { return *client_; }

private:
    std::unique_ptr<mqtt::async_client> client_;
};

} // namespace industrial
