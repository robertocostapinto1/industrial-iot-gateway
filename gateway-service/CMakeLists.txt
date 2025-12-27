cmake_minimum_required(VERSION 3.10)
project(gateway_service CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find Dependencies
find_package(Protobuf REQUIRED)
find_package(PahoMqttCpp REQUIRED)

# 2. Shared Schema Path
set(PROTO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../proto")
set(PROTO_FILE "${PROTO_SRC_DIR}/sensor_data.proto")

# 3. Generate Protobuf Code
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})

# 4. Include Paths
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${PROTO_SRC_DIR})

# 5. Executable
add_executable(gateway_service 
    src/main.cpp 
    ${PROTO_SRCS} 
    ${PROTO_HDRS}
)

# 6. Link Libraries
# Note: PahoMqttCpp::paho-mqttpp3 handles the underlying C dependency automatically
target_link_libraries(gateway_service PRIVATE 
    ${Protobuf_LIBRARIES} 
    PahoMqttCpp::paho-mqttpp3
)
