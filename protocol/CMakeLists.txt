cmake_minimum_required(VERSION 3.10)
project(protocol)

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS Messages.proto)

add_library(protocol STATIC 
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

target_include_directories(protocol PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Protobuf_INCLUDE_DIRS}
)

target_link_libraries(protocol 
    ${Protobuf_LIBRARIES}
)
