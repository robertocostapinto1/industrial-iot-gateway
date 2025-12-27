# Industrial Gateway CMake Toolchain File
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(BR_ROOT $ENV{HOME}/projects/iot-gateway/gateway-os/buildroot/output)

set(CMAKE_C_COMPILER   ${BR_ROOT}/host/bin/aarch64-linux-gcc)
set(CMAKE_CXX_COMPILER ${BR_ROOT}/host/bin/aarch64-linux-g++)

set(CMAKE_SYSROOT ${BR_ROOT}/host/aarch64-buildroot-linux-gnu/sysroot)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

# --- PROTOBUF CROSS-COMPILER FIX ---
# Point to the HOST protoc (x86_64) instead of the TARGET one (ARM64)
set(Protobuf_PROTOC_EXECUTABLE ${BR_ROOT}/host/bin/protoc CACHE FILEPATH "Host Protobuf Compiler")
# -----------------------------------

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Toolchain Loaded: AArch64 Cross-Compiler via Buildroot")
message(STATUS "Using Host Protoc: ${Protobuf_PROTOC_EXECUTABLE}")
