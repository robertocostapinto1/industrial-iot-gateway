# Phase 1: Gateway OS Infrastructure
**Status:**  Validated | **Core Tool:** cross-compilation|Buildroot

## Objective
Build a minimalist, secure, and deterministic Linux distribution for the Raspberry Pi 4 to act as an Industrial Gateway. 

## Engineering Decisions
| Feature | Choice | Rationale |
| :--- | :--- | :--- |
| **Toolchain** | Buildroot 2024.02 LTS | Provides a reproducible, "frozen" environment for production stability. |
| **C-Library** | `glibc` | Selected over `musl` to ensure full C++17/20 STL support and robust multi-threading. |
| **Architecture** | AArch64 | Maximizes RPi4 hardware performance for high-throughput data ingestion. |

## Middleware Stack
- **Mosquitto:** Configured as the secure MQTT broker. See: phase-3-gateway-service#Secure Broker|[Secure Configuration](phase-3-gateway-service.md).
- **BlueZ 5.x:** Integrated for future BLE edge-node expansion.
- **Protobuf-C++:** Ingests binary packets from the Edge Tier.

## Hardware Integration
The system relies on the linux-boot-flow|Raspberry Pi 4 Boot Sequence to load the custom kernel and mount the RootFS.

## Host Optimization
- **CCache:** 50GB global cache implemented to reduce iterative build times by 80%.
- **Global DL:** Redirected `BR2_DL_DIR` to ensure source code persistence.