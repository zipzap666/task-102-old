#include <gtest/gtest.h>
#include <memory>
#include "../parser/parserTools.h"

#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif
typedef std::vector<char> Data;
typedef std::shared_ptr<const Data> PointerToConstData;
template <typename Message>
PointerToConstData serializeDelimited(const Message &msg)
{
    const size_t messageSize = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
    const size_t headerSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);

    const PointerToConstData &result = std::make_shared<Data>(headerSize + messageSize);
    uint8_t *buffer = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&*result->begin()));

    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buffer);
    msg.SerializeWithCachedSizesToArray(buffer + headerSize);

    return result;
}

using namespace TestTask::Messages;

TEST(parseDelimited, empty_data)
{
    EXPECT_EQ(parseDelimited<WrapperMessage>(nullptr, 10, 0), nullptr);
    EXPECT_EQ(parseDelimited<WrapperMessage>("", 10, 0), nullptr);
    EXPECT_EQ(parseDelimited<WrapperMessage>(new char*(), 10, 0), nullptr);
}

TEST(parseDelimited, wrong_size)
{
    WrapperMessage msg;
    msg.mutable_fast_response()->set_current_date_time(std::string("10"));
    auto data = serializeDelimited(msg);

    EXPECT_TRUE(parseDelimited<WrapperMessage>(data->data(), data->size()+2) !=  nullptr);
    EXPECT_EQ(parseDelimited<WrapperMessage>(data->data(), data->size()-2), nullptr);
}

TEST(parseDelimited, any_data)
{


    EXPECT_EQ(parseDelimited<WrapperMessage>("123451212324123", 10, 0), nullptr);
    EXPECT_EQ(parseDelimited<WrapperMessage>("asdasdasdasd", 10, 0), nullptr);
}

TEST(parseDelimited, corrupted_data)
{
    WrapperMessage msg;
    msg.mutable_fast_response()->set_current_date_time(std::string("10"));
    auto data = serializeDelimited(msg);

    std::string corrupt_data(data->data());

    corrupt_data[3] = 'k';
    EXPECT_EQ(parseDelimited<WrapperMessage>(corrupt_data.data(), corrupt_data.size()), nullptr);
    corrupt_data[4] = 'e';
    EXPECT_EQ(parseDelimited<WrapperMessage>(corrupt_data.data(), corrupt_data.size()), nullptr);
    corrupt_data[5] = 'e';
    EXPECT_EQ(parseDelimited<WrapperMessage>(corrupt_data.data(), corrupt_data.size()), nullptr);
}

TEST(parseDelimited, bytes_consumed)
{
    WrapperMessage msg;
    msg.mutable_fast_response()->set_current_date_time(std::string("10"));
    auto data = serializeDelimited(msg);

    size_t bytesConsumed = 0;
    parseDelimited<WrapperMessage>(data->data(), data->size(), &bytesConsumed);
    EXPECT_EQ(bytesConsumed, data->size());
    parseDelimited<WrapperMessage>(data->data(), data->size()-2, &bytesConsumed);
    EXPECT_EQ(bytesConsumed, data->size());
    parseDelimited<WrapperMessage>(data->data(), data->size(), &bytesConsumed);
    EXPECT_EQ(bytesConsumed, data->size() * 2);    
}