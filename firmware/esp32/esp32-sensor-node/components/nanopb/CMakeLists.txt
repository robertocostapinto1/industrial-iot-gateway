set(NANOPB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../external/nanopb")

idf_component_register(SRCS "${NANOPB_ROOT}/pb_encode.c" 
                            "${NANOPB_ROOT}/pb_decode.c" 
                            "${NANOPB_ROOT}/pb_common.c"
                    INCLUDE_DIRS "${NANOPB_ROOT}")
