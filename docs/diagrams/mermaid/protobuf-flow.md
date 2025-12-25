graph TD  
subgraph "The Contract (Shared)"  
P[sensor_data.proto] --> O[sensor_data.options]  
end  
  
subgraph "ESP32 (Edge Tier)"  
P --> N_GEN[Nanopb Generator]  
N_GEN --> C_WRAP[C++ Protobuf Wrapper]  
C_WRAP --> BIN[Serialized Binary Payload]  
end  
  
subgraph "RPi4 (Gateway Tier)"  
P --> P_GEN[Google Protobuf C++ Gen]  
BIN --> INGEST[Gateway Ingestion Service]  
INGEST --> DB[(Postgres/TimescaleDB)]  
end  
  
style P fill:#f9f,stroke:#333  
style BIN fill:#bbf,stroke:#333