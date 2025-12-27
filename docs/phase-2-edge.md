# Phase 2: Edge Node Firmware (C++17)
**Status:**  Validated | **Framework:** ESP-IDF v5.x+

## Design Philosophy: concepts#RAII|RAII
Peripherals are treated as scoped resources. This eliminates memory leaks and ensures hardware states (WiFi/MQTT) are deterministic.

### 1. Network Layer (WiFi Manager)
- **Class:** `industrial::WifiManager`
- **Pattern:** Non-copyable Singleton (`= delete`).
- **Safety:** Destructor guarantees `esp_wifi_deinit()`.

### 2. Data Layer: concepts#Protobuf|Protobuf
- **Serialization:** Used binary format for minimal wireless air-time.
- **Optimization:** Utilized `.options` files to enforce **Static Memory Allocation**, preventing concepts#Static vs Dynamic Allocation|heap fragmentation.

### 3. Transport Layer: security-pki|Secure MQTTS
- **Encryption:** TLS 1.2/1.3 on Port 8883.
- **Trust Model:** Embedded Root CA Certificate as a binary blob using CMake `EMBED_TXTFILES`.
- **Dependency:** Integrated `espressif/mqtt` via the Managed Component Manager.

## Build System Mastery
Implemented a **Two-Pass CMake** logic to automate Protobuf generation while satisfying the ESP-IDF component validation rules. See: concepts#Multi-Pass Build|Multi-Pass Explanation.

##  Build Engineering & Debugging
### Hurdles Resolved
1. **CMake Pass 1 vs Pass 2:** 
   - *Problem:* ESP-IDF failed to find generated Protobuf headers.
   - *Fix:* Implemented `if(NOT CMAKE_BUILD_EARLY_EXPANSION)` guard. This ensures the component registers its name in Pass 1, but only triggers the Python generator in Pass 2 when the full environment is ready.
2. **String Truncation Warnings:**
   - *Problem:* `-Werror=stringop-truncation` on `strncpy`.
   - *Fix:* Moved to a manual `std::copy_n` + explicit null-termination strategy. This prevents buffer overflows in the networking stack.