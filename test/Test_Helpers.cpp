#include <gtest/gtest.h>
#include "protobuf_parser/helpers.hpp"
#include <Messages.pb.h> 

TestTask::Messages::WrapperMessage createTestMessage(int timeToSleep) {
    TestTask::Messages::WrapperMessage message;
    auto* request = new TestTask::Messages::RequestForSlowResponse;
    request->set_time_in_seconds_to_sleep(timeToSleep);
    message.set_allocated_request_for_slow_response(request);
    return message;
}

TEST(HelpersTests, test_serialize_and_parsing_full_msg)
{
    const int testNum = 2;
    auto message = createTestMessage(testNum);

    uint32_t msgSize = PROTOBUF_MESSAGE_BYTE_SIZE(message);
    size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(msgSize);

    auto data = serializeDelimited(message);

    size_t bytesConsumed;
    auto res = parseDelimited<TestTask::Messages::WrapperMessage>(
        static_cast<const void*>(&*data), data->size(), &bytesConsumed);

    ASSERT_TRUE(res != nullptr);
    ASSERT_EQ(bytesConsumed, msgSize + variantByteSize);

    auto response = *res;
    ASSERT_TRUE(response.has_request_for_slow_response());
    ASSERT_EQ(response.request_for_slow_response().time_in_seconds_to_sleep(), testNum);
}

TEST(HelpersTests, test_serialize_and_parsing_msg_by_parts)
{
    const int testNum = 2;
    auto message = createTestMessage(testNum);

    uint32_t msgSize = PROTOBUF_MESSAGE_BYTE_SIZE(message);
    size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(msgSize);

    auto data = serializeDelimited(message);

    size_t bytesConsumed;
    auto res = parseDelimited<TestTask::Messages::WrapperMessage>(
        static_cast<const void*>(&*data), data->size() / 2, &bytesConsumed);

    ASSERT_TRUE(res == nullptr);
    ASSERT_EQ(bytesConsumed, 0);

    res = parseDelimited<TestTask::Messages::WrapperMessage>(
        static_cast<const void*>(&*data), data->size(), &bytesConsumed);

    ASSERT_TRUE(res != nullptr);
    ASSERT_EQ(bytesConsumed, msgSize + variantByteSize);

    auto response = *res;
    ASSERT_TRUE(response.has_request_for_slow_response());
    ASSERT_EQ(response.request_for_slow_response().time_in_seconds_to_sleep(), testNum);
}

TEST(HelpersTests, test_parsing_wrong_format_msg)
{
    const int testNum = 2;
    auto message = createTestMessage(testNum);

    uint32_t msgSize = PROTOBUF_MESSAGE_BYTE_SIZE(message);
    size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(msgSize);

    auto data = serializeDelimited(message);

    size_t bytesConsumed;
    std::shared_ptr<Data> wrongData = std::make_shared<Data>(Data(data->cbegin(), --data->cend()));
    wrongData->push_back('\b');

    auto res = parseDelimited<TestTask::Messages::WrapperMessage>(
        static_cast<const void*>(&*wrongData), wrongData->size() / 2, &bytesConsumed);

    ASSERT_TRUE(res == nullptr);
    ASSERT_EQ(bytesConsumed, 0);

    res = parseDelimited<TestTask::Messages::WrapperMessage>(
        static_cast<const void*>(&*data), data->size(), &bytesConsumed);

    ASSERT_TRUE(res != nullptr);
    ASSERT_EQ(bytesConsumed, msgSize + variantByteSize);

    auto response = *res;
    ASSERT_TRUE(response.has_request_for_slow_response());
    ASSERT_EQ(response.request_for_slow_response().time_in_seconds_to_sleep(), testNum);
}