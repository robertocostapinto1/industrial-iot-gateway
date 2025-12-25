sequenceDiagram
    participant D as Developer Host
    participant E as ESP32 (Edge)
    participant G as RPi4 (Gateway)

    Note over D: openssl genrsa (ca.key)
    D->>E: Embed ca.crt (Public Key)
    D->>G: Sign & Deploy Gateway Cert

    E->>G: TCP Connection (Port 8883)
    G->>E: Server Hello + Gateway Certificate
    Note over E: Verify Gateway Cert against embedded ca.crt
    
    alt Verification Success
        E->>G: Encrypted MQTTS Handshake
        E->>G: Send Protobuf Telemetry
    else Verification Fail (MITM Attack)
        E->>E: Terminate Connection
    end
