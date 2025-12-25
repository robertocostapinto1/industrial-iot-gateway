# 1. Define paths (5 levels up to reach project root)
set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../")
set(PROTO_DIR "${ROOT_DIR}/proto")
set(PROTO_FILE "${PROTO_DIR}/sensor_data.proto")
set(NANOPB_GENERATOR "${ROOT_DIR}/external/nanopb/generator/nanopb_generator.py")

# 2. Create the directory
file(MAKE_DIRECTORY ${GEN_DIR})

# 3. Register the component
idf_component_register(SRCS "protobuf_wrapper.cpp"
                    INCLUDE_DIRS "include" ${GEN_DIR}
                    REQUIRES nanopb)

# 4. Generator Logic
if(NOT CMAKE_BUILD_EARLY_EXPANSION)
    add_custom_command(
        OUTPUT ${GEN_DIR}/sensor_data.pb.c ${GEN_DIR}/sensor_data.pb.h
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GEN_DIR}
        COMMAND python3 ${NANOPB_GENERATOR} -I ${PROTO_DIR} --output-dir=${GEN_DIR} ${PROTO_FILE}
        DEPENDS ${PROTO_FILE} ${NANOPB_GENERATOR}
        COMMENT "Generating Nanopb files from project root"
        VERBATIM
    )
    target_sources(${COMPONENT_LIB} PRIVATE ${GEN_DIR}/sensor_data.pb.c)
endif()
