#include "pr.pb.h"
#include "protobuf_parser/DelimitedMessagesStreamParser.h"
#include <iostream>

int main() {
    TestTask::Messages::WrapperMessage msg;
    TestTask::Messages::RequestForSlowResponse* req;
    std::string data;
    std::shared_ptr<const Data> pointerToMsg;
    for (int i = 0; i < 5; i++)
    {
        req = new TestTask::Messages::RequestForSlowResponse;
        req->set_time_in_seconds_to_sleep(1 * i);
        msg.set_allocated_request_for_slow_response(req);
        pointerToMsg = serializeDelimited<TestTask::Messages::WrapperMessage>(msg);
        for (int j = 0; j < pointerToMsg->size(); j++)
        {
            data.push_back((*pointerToMsg)[j]);
        }
    }

    std::list<DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::PointerToConstValue> list;

    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;
    list = parser.parse(data);

    for (const auto& pointer : list)
    {
        std::cout << (*pointer).DebugString() << "\n";
    }

    return 0;
}

