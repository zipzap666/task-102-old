#include <gtest/gtest.h>
#include <memory>
#include "../parser/parserTools.h"
#include "../proto/message.pb.h"

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
    uint8_t *buffer = const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(&*result->begin()));

    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buffer);
    msg.SerializeWithCachedSizesToArray(buffer + headerSize);

    return result;
}

using namespace TestTask::Messages;

TEST(parseDelimited, all_good)
{
    WrapperMessage msg1, msg2, msg3, msg4;

    msg1.mutable_fast_response()->set_current_date_time(std::string("10"));
    msg2.mutable_slow_response()->set_connected_client_count(10);
    *msg3.mutable_request_for_fast_response() = RequestForFastResponse();
    msg4.mutable_request_for_slow_response()->set_time_in_seconds_to_sleep(10);

    auto data = serializeDelimited(msg1);
    auto test = parseDelimited<WrapperMessage>(data->data(), data->size());

    EXPECT_TRUE(test != nullptr);
    EXPECT_EQ(test->has_fast_response(), true);
    EXPECT_EQ(test->has_slow_response(), false);
    EXPECT_EQ(test->has_request_for_fast_response(), false);
    EXPECT_EQ(test->has_request_for_slow_response(), false);
    EXPECT_STREQ(test->fast_response().current_date_time().c_str(), "10");

    data = serializeDelimited(msg2);
    test = parseDelimited<WrapperMessage>(data->data(), data->size());

    EXPECT_TRUE(test != nullptr);
    EXPECT_EQ(test->has_fast_response(), false);
    EXPECT_EQ(test->has_slow_response(), true);
    EXPECT_EQ(test->has_request_for_fast_response(), false);
    EXPECT_EQ(test->has_request_for_slow_response(), false);
    EXPECT_EQ(test->slow_response().connected_client_count(), 10);

    data = serializeDelimited(msg3);
    test = parseDelimited<WrapperMessage>(data->data(), data->size());

    EXPECT_TRUE(test != nullptr);
    EXPECT_EQ(test->has_fast_response(), false);
    EXPECT_EQ(test->has_slow_response(), false);
    EXPECT_EQ(test->has_request_for_fast_response(), true);
    EXPECT_EQ(test->has_request_for_slow_response(), false);

    data = serializeDelimited(msg4);
    test = parseDelimited<WrapperMessage>(data->data(), data->size());

    EXPECT_TRUE(test != nullptr);
    EXPECT_EQ(test->has_fast_response(), false);
    EXPECT_EQ(test->has_slow_response(), false);
    EXPECT_EQ(test->has_request_for_fast_response(), false);
    EXPECT_EQ(test->has_request_for_slow_response(), true);
    EXPECT_EQ(test->request_for_slow_response().time_in_seconds_to_sleep(), 10);
}

TEST(parseDelimited, empty_data)
{
    EXPECT_EQ(parseDelimited<WrapperMessage>(nullptr, 10, 0), nullptr);
    EXPECT_EQ(parseDelimited<WrapperMessage>("", 10, 0), nullptr);
    EXPECT_EQ(parseDelimited<WrapperMessage>(new char *(), 10, 0), nullptr);

    WrapperMessage msg;
    auto data = serializeDelimited(msg);
    EXPECT_EQ(parseDelimited<WrapperMessage>(data->data(), data->size()), nullptr);
}

TEST(parseDelimited, wrong_size)
{
    WrapperMessage msg;
    msg.mutable_fast_response()->set_current_date_time(std::string("10"));
    auto data = serializeDelimited(msg);

    EXPECT_TRUE(parseDelimited<WrapperMessage>(data->data(), data->size() + 2) != nullptr);
    EXPECT_EQ(parseDelimited<WrapperMessage>(data->data(), data->size() - 2), nullptr);
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
    parseDelimited<WrapperMessage>(data->data(), data->size() - 2, &bytesConsumed);
    EXPECT_EQ(bytesConsumed, data->size());
    parseDelimited<WrapperMessage>(data->data(), data->size(), &bytesConsumed);
    EXPECT_EQ(bytesConsumed, data->size() * 2);
}

TEST(DelimitedMessagesStreamParser, all_good)
{
    WrapperMessage msg1, msg2, msg3, msg4;

    msg1.mutable_fast_response()->set_current_date_time(std::string("10"));
    msg2.mutable_slow_response()->set_connected_client_count(10);
    *msg3.mutable_request_for_fast_response() = RequestForFastResponse();
    msg4.mutable_request_for_slow_response()->set_time_in_seconds_to_sleep(10);

    std::vector<char> messages;

    auto data = serializeDelimited(msg1);
    messages.insert(messages.end(), data->begin(), data->end());

    data = serializeDelimited(msg2);
    messages.insert(messages.end(), data->begin(), data->end());

    data = serializeDelimited(msg3);
    messages.insert(messages.end(), data->begin(), data->end());

    data = serializeDelimited(msg4);
    messages.insert(messages.end(), data->begin(), data->end());

    std::list<std::shared_ptr<const WrapperMessage>> list_msg;
    typedef DelimitedMessagesStreamParser<WrapperMessage> Parser;
    Parser parser;

    for (const char byte : messages)
    {
        const auto &parsedMessages = parser.parse(std::string(1, byte));
        for (const auto &value : parsedMessages)
        {
            list_msg.push_back(value);
        }
    }
    EXPECT_EQ(list_msg.size(), 4);

    auto iter = list_msg.begin();
    EXPECT_TRUE((*iter)->has_fast_response());
    ++iter;
    EXPECT_TRUE((*iter)->has_slow_response());
    ++iter;
    EXPECT_TRUE((*iter)->has_request_for_fast_response());
    ++iter;
    EXPECT_TRUE((*iter)->has_request_for_slow_response());

    list_msg = parser.parse(std::string(messages.data(),messages.size()));
    EXPECT_EQ(list_msg.size(), 4);
}

TEST(DelimitedMessagesStreamParser, empty_data)
{
    DelimitedMessagesStreamParser<WrapperMessage> parser;
    EXPECT_EQ(parser.parse("").size(), 0);
    EXPECT_EQ(parser.parse(std::string()).size(), 0);
}

TEST(DelimitedMessagesStreamParser, any_data)
{
    DelimitedMessagesStreamParser<WrapperMessage> parser;
    EXPECT_EQ(parser.parse("sadasgasdgasd").size(), 0);
    EXPECT_EQ(parser.parse(std::string("asdfgasdga")).size(), 0);
}

TEST(DelimitedMessagesStreamParser, corrupted_data)
{
    DelimitedMessagesStreamParser<WrapperMessage> parser;
    WrapperMessage msg;
    msg.mutable_fast_response()->set_current_date_time(std::string("10"));
    auto data = serializeDelimited(msg);

    std::string corrupt_data(data->data());

    corrupt_data[3] = 'k';
    EXPECT_EQ(parser.parse(corrupt_data).size(), 0);
    corrupt_data[4] = 'e';
    EXPECT_EQ(parser.parse(corrupt_data).size(), 0);
    corrupt_data[5] = 'e';
    EXPECT_EQ(parser.parse(corrupt_data).size(), 0);
}