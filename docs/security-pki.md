# Security Infrastructure: Private PKI
**Status:** 🔒 Established

## The "Chain of Trust" Model
In this project, we act as our own **Certificate Authority (CA)**. This is the industrial standard for internal gateway traffic.

### 1. The Root CA (Master Identity)
- **Files:** `security/ca/ca.key` (Secret) and `security/ca/ca.crt` (Public).
- **Purpose:** All devices trust this identity. The `.crt` is embedded in the ESP32.

### 2. The Gateway Identity (The Pi)
- **Files:** `security/gateway/gateway.key` and `security/gateway/gateway.crt`.
- **Signature:** The `gateway.crt` is signed by the `ca.key`.
- **Validation:** When the ESP32 connects, it verifies that the Pi's cert was signed by the Root CA.

## OpenSSL Implementation Commands
```bash
# Generate CA
openssl genrsa -out ca.key 2048
openssl req -x509 -new -nodes -key ca.key -sha256 -days 3650 -out ca.crt

# Sign Gateway Cert
openssl x509 -req -in gateway.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out gateway.crt
