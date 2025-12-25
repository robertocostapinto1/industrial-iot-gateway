```markdown
# Phase 3: Gateway Service Integration
**Status:** 🚧 In Progress

## Objectives
Bridge the gap between the Edge Tier ([[phase-2-edge|ESP32]]) and the Data Sinks using a high-performance C++ service.

## Infrastructure Tasks
- [x] **Certificate Signing:** Sign `gateway.crt` with `ca.key`.
- [ ] **Secure Broker:** Configure Mosquitto on RPi4 to use TLS.
- [ ] **Verify Connection:** Flash ESP32 with real Gateway IP and observe SSL handshake.

## Software Tasks
- [ ] **Ingestion Service:** Develop a C++ service in Buildroot using the [[cross-compilation|AArch64 Toolchain]].
- [ ] **Protobuf Deserialization:** Link shared `sensor_data.proto` to the Gateway logic.
