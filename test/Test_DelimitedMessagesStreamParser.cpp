#include <gtest/gtest.h>
#include "protobuf_parser/DelimitedMessagesStreamParser.h"
#include <Messages.pb.h> 

TestTask::Messages::WrapperMessage createTestMessage(int timeToSleep) {
    TestTask::Messages::WrapperMessage message;
    auto* request = new TestTask::Messages::RequestForSlowResponse;
    request->set_time_in_seconds_to_sleep(timeToSleep);
    message.set_allocated_request_for_slow_response(request);
    return message;
}

TEST(ParserTests, test_parsing_with_full_msg)
{
    const int testNum = 1;
    std::string data;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    for (int i = 0; i < 5; i++)
    {
        auto message = createTestMessage(testNum * i);
        auto pointerToMsg = serializeDelimited(message);

        data.append(pointerToMsg->begin(), pointerToMsg->end());
    }

    auto list = parser.parse(data);

    int numToCompare = 0;
    for (const auto& msg : list)
    {
        ASSERT_TRUE(msg->has_request_for_slow_response());
        ASSERT_EQ(msg->request_for_slow_response().time_in_seconds_to_sleep(), numToCompare);
        numToCompare += testNum;
    }
}

TEST(ParserTests, test_parsing_with_part_of_msg)
{
    const int testNum = 1;
    std::string data;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    auto message = createTestMessage(testNum);
    auto pointerToMsg = serializeDelimited(message);

    data.append(pointerToMsg->begin(), pointerToMsg->begin() + pointerToMsg->size() / 2);

    auto list = parser.parse(data);
    ASSERT_TRUE(list.empty());

    data.clear();
    data.append(pointerToMsg->begin() + pointerToMsg->size() / 2, pointerToMsg->end());

    list = parser.parse(data);
    ASSERT_TRUE(list.front()->has_request_for_slow_response());
    ASSERT_EQ(list.front()->request_for_slow_response().time_in_seconds_to_sleep(), testNum);
}