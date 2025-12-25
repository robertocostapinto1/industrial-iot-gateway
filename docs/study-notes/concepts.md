# Interview Knowledge: Glossary of Concepts

### RAII (Resource Acquisition Is Initialization)
- **What:** Binding resource lifetime to object lifetime.
- **Why:** Prevents resource leaks (memory, sockets, hardware handles).
- **Implementation:** Constructor starts it, Destructor stops it.

### Name Mangling & extern "C"
- **What:** C++ encodes function names to support overloading; C does not.
- **Why:** Necessary for C++ code to be called by a C-kernel (like FreeRTOS/ESP-IDF).
- **Implementation:** `extern "C" void app_main()`.

### Static vs Dynamic Allocation (Nanopb)
- **Static:** Memory size is decided at compile-time (Fixed).
- **Dynamic:** Memory is requested at runtime (Heap/Malloc).
- **Why:** In industrial systems, static is safer as it prevents "Out of Memory" crashes at runtime.

### PKI (Public Key Infrastructure)
- **Root CA:** The "Master" certificate used to sign other certificates.
- **Chain of Trust:** The Edge Node trusts the Gateway because both "know" the same Root CA.
- **Implementation:** Created via OpenSSL; Public Key embedded in firmware.

### MQTTS (MQTT over TLS)
- **What:** Standard MQTT wrapped in an SSL/TLS encrypted tunnel.
- **Security:** Provides **Confidentiality** (encryption) and **Authentication** (certificates).
- **Standard Port:** 8883.