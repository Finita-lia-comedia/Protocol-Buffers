#pragma once

#include <list>
#include <string>
#include <memory>
#include <vector>
#include "helpers.hpp"

template <typename MessageType>
class DelimitedMessagesStreamParser
{
public:
    DelimitedMessagesStreamParser() = default;
    typedef std::shared_ptr<const MessageType> PointerToConstValue;
    typedef std::list<PointerToConstValue> ParsedMsgsList;
    ParsedMsgsList parse(const std::string& data);

private:
    std::vector<char> m_buffer;

};

template<typename MessageType>
typename DelimitedMessagesStreamParser<MessageType>::ParsedMsgsList
DelimitedMessagesStreamParser<MessageType>::parse(const std::string& data) {
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());
    ParsedMsgsList msgsList;
    size_t consumedBytes = 0;

    do {
        auto parsedMsg = parseDelimited<MessageType>(
            static_cast<const void*>(m_buffer.data()),
            m_buffer.size(),
            &consumedBytes);

        if (parsedMsg != nullptr) {
            msgsList.push_back(parsedMsg);
            m_buffer.erase(m_buffer.begin(), m_buffer.begin() + consumedBytes);
        }
        else {
            break; 
        }
    } while (consumedBytes > 0);

    return msgsList;
}
