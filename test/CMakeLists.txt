cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0016 NEW)

project(test_parser)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
add_test(test ${EXECUTABLE_OUTPUT_PATH}/test_parser)

include_directories(${CMAKE_SOURCE_DIR}/src)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(
  test_parser
        run.cpp
		Test_Helpers.cpp
        Test_DelimitedMessagesStreamParser.cpp
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ${CMAKE_SOURCE_DIR}/src/protobuf_parser/DelimitedMessagesStreamParser.h)
target_link_libraries(test_parser protocol ${Protobuf_LIBRARIES} gtest gmock pthreadVC3)