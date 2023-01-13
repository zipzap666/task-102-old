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
    google::protobuf::uint8 *buffer = reinterpret_cast<const google::protobuf::uint8 *>(&*result->begin());

    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buffer);
    msg.SerializeWithCachedSizesToArray(buffer + headerSize);

    return result;
}
/*
// empty data
// wrong size
// 
*/

using namespace TestTask::Messages;

TEST(parseDelimited, empty_data)
{
    EXPECT_EQ(parseDelimited<WrapperMessage>(nullptr, 10, 0), nullptr);
    EXPECT_EQ(parseDelimited<WrapperMessage>("", 10, 0), nullptr);
}

TEST(parseDelimited, wrong_size)
{
    WrapperMessage msg;
    //msg.mutable_fast_response()->set_current_date_time(std::string("10"));
    auto data = serializeDelimited(msg);
    std::string str = convert_int32_to_str(data->size()) + std::string(data->data());
    
    EXPECT_EQ( parseDelimited<WrapperMessage>(str.c_str(), 0, 0), nullptr);   

    EXPECT_EQ(parseDelimited<WrapperMessage>(str.c_str(), 6, 0), nullptr);
}