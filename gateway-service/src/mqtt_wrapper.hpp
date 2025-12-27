#pragma once
#include <mqtt/async_client.h>
#include "safe_queue.hpp"

namespace industrial {

// Inherit from mqtt::callback to handle events
class MqttWrapper : public virtual mqtt::callback {
public:
    MqttWrapper(const std::string& addr, const std::string& id, SafeQueue& q) 
        : queue_(q) {
        client_ = std::make_unique<mqtt::async_client>(addr, id);
        client_->set_callback(*this); 
    }

    bool connect(const std::string& ca_path) {
        auto conn_opts = mqtt::connect_options_builder()
            .ssl(mqtt::ssl_options_builder().trust_store(ca_path).finalize())
            .clean_session(true)
            .finalize();
        try {
            client_->connect(conn_opts)->wait();
            client_->subscribe("industrial/telemetry/#", 1);
            return true;
        } catch (...) { return false; }
    }

    ~MqttWrapper() {
        if (client_ && client_->is_connected()) client_->disconnect()->wait();
    }

    // --- MQTT Callbacks ---
    void message_arrived(mqtt::const_message_ptr msg) override {
        // PRODUCER: Push raw bytes into the queue
        queue_.push(msg->get_payload_str());
    }

    void connection_lost(const std::string& cause) override {
        std::cerr << "Connection lost: " << cause << std::endl;
    }

private:
    std::unique_ptr<mqtt::async_client> client_;
    SafeQueue& queue_; // Reference to the shared buffer
};

} // namespace industrial
