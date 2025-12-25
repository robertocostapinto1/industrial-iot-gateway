# Path: main -> sensor-node -> esp32 -> firmware -> root (4 levels)
idf_component_register(SRCS "main.cpp" 
                    INCLUDE_DIRS "." 
                    REQUIRES wifi_manager protobuf_wrapper mqtt_manager mqtt
                    EMBED_TXTFILES ../../../../security/ca/ca_cert.pem)
