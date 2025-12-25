# Heterogeneous Industrial IoT Gateway
**An End-to-End Secure Telemetry System (Edge-to-Cloud)**
![System Architecture](docs/diagrams/master-architecture.png)

## Executive Summary
This project implements a secure, high-performance industrial IoT pipeline. It utilizes a **Heterogeneous Architecture** consisting of an **ESP32 Edge Node** (C++17/RTOS) communicating over an encrypted **MQTTS** tunnel to a custom **Linux Gateway** (Buildroot/AArch64).

### Key Architectural Pillars
1. **Resource Safety:** Strict RAII implementation in Modern C++.
2. **Security:** Private PKI (Certificate Authority) with TLS 1.2/1.3 encryption.
3. **Efficiency:** Nanopb-based binary serialization (75% bandwidth reduction vs. JSON).
4. **Reliability:** Custom-built Linux OS via Buildroot for deterministic gateway performance.

---

## Project Structure
- `/firmware`: ESP32-sensor-node (Phase 2)
- `/gateway`: Buildroot configuration & Gateway Service (Phase 1 & 3)
- `/proto`: Shared Protobuf schemas (The Data Contract)
- `/security`: Root CA and Infrastructure keys
- `/docs`: Detailed implementation guides and study notes

## Engineering Milestones
- **Dec 2025:** Established Private PKI and signed Gateway certificates.
- **Dec 2025:** Validated RAII-based hardware drivers on ESP32 silicon.
- **Dec 2025:** Achieved 75% telemetry compression using Nanopb serialization.
