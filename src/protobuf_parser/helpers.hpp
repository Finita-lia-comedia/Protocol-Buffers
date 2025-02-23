#pragma once

#include <vector>
#include <memory>
#include <google/protobuf/io/coded_stream.h>

#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif


typedef std::vector<char> Data;
typedef std::shared_ptr<const Data> PointerToConstData;

template <typename Message>
PointerToConstData serializeDelimited(const Message& msg)
{
    uint32_t messageSize = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
    size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);
    Data data(variantByteSize + messageSize);

    uint8_t* buf = reinterpret_cast<uint8_t*>(&*(data.begin()));
    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buf);
    msg.SerializeWithCachedSizesToArray(buf + variantByteSize);

    return std::make_shared<const Data>(data);
}

template <typename Message>
std::shared_ptr<Message> parseDelimited(const void* data, size_t size,
    size_t* bytesConsumed = nullptr)
{
    if (!data || size == 0) {
        if (bytesConsumed) *bytesConsumed = 0;
        return nullptr;
    }
    const uint8_t* buf = static_cast<const uint8_t*>(data);
    google::protobuf::io::CodedInputStream stream(buf, size);

    uint32_t messageSize = 0;
    if (!stream.ReadVarint32(&messageSize)) {
        if (bytesConsumed) *bytesConsumed = 0;
        return nullptr;
    }

    size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);
    if (variantByteSize + messageSize > size) {
        if (bytesConsumed) *bytesConsumed = 0;
        return nullptr;
    }

    std::shared_ptr<Message> result = std::make_shared<Message>();
    std::string bytes;

    if (stream.ReadString(&bytes, messageSize) && result->ParseFromString(bytes)) {
        if (bytesConsumed) {
            *bytesConsumed = variantByteSize + messageSize;
        }
        return result;
    }

    if (bytesConsumed) *bytesConsumed = 0;
    return nullptr;
}
