graph TB
    subgraph "1. EDGE TIER (ESP32 / RTOS)"
        S1[Sensors] -->|Raw Data| P_WRAP[Protobuf Wrapper]
        
        subgraph "RAII Resource Management"
            WIFI[WiFi Manager]
            MQTT_C[MQTTS Client]
        end
        
        P_WRAP -->|Binary Payload| MQTT_C
    end

    subgraph "2. TRANSPORT LAYER (Encrypted)"
        MQTT_C == "Protobuf over TLS 1.3 (Port 8883)" ==> BROKER
    end

    subgraph "3. GATEWAY TIER (RPi 4 / Buildroot)"
        subgraph "Middleware Layer"
            BROKER[Mosquitto Broker]
        end

        subgraph "Application Layer (C++17 Service)"
            INGEST[Ingestion Engine]
            DECODE[Protobuf Decoder]
            SYNC[Thread-Safe Queue]
        end

        BROKER -->|Encrypted Stream| INGEST
        INGEST --> DECODE
        DECODE --> SYNC
    end

    subgraph "4. DATA SINKS"
        SYNC --> SQL[(Local SQL DB)]
        SYNC --> LOGS[System Logs]
    end

    subgraph "INFRASTRUCTURE (Shared Logic)"
        SCHEMA{sensor_data.proto} -.->|Generates Code| P_WRAP
        SCHEMA{sensor_data.proto} -.->|Generates Code| DECODE
        PKI[Private Root CA] -.->|Certificate Trust| MQTT_C
        PKI -.->|Server Signing| BROKER
    end

    %% Styling
    classDef edge fill:#fff2cc,stroke:#d6b656,stroke-width:2px;
    classDef gateway fill:#dae8fc,stroke:#6c8ebf,stroke-width:2px;
    classDef transport fill:#f8cecc,stroke:#b85450,stroke-width:2px,stroke-dasharray: 5 5;
    classDef infra fill:#e1d5e7,stroke:#9673a6,stroke-style:italic;

    class S1,P_WRAP,WIFI,MQTT_C edge;
    class BROKER,INGEST,DECODE,SYNC gateway;
    class SCHEMA,PKI infra;
