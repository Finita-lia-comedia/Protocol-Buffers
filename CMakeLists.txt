cmake_minimum_required (VERSION 3.10)
cmake_policy(SET CMP0016 NEW)

project(protobuf_parser)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(protocol)  

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INLCUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "Protobuf compiler: ${Protobuf_PROTOC_EXECUTABLE}")


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_SOURCE_DIR}/Messages.proto)
message(STATUS "Generated sources: ${PROTO_SRCS}")
message(STATUS "Generated headers: ${PROTO_HDRS}")



add_executable(
  protobuf_parser
        src/main.cpp
        src/protobuf_parser/DelimitedMessagesStreamParser.h
        src/protobuf_parser/helpers.hpp
        ${PROTO_HDRS}
        ${PROTO_SRCS})
target_link_libraries(protobuf_parser protocol ${Protobuf_LIBRARIES})

enable_testing()
add_subdirectory(test)