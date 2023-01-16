#ifndef STREAM_PARSER_HEADER
#define STREAM_PARSER_HEADER

#include <list>
#include <vector>
#include <string>
#include <memory>
#include "functions.h"

template <typename Message>
std::shared_ptr<Message> parseDelimited(const void *data, size_t size, size_t *bytesConsumed = 0)
{
    std::cerr << "parseDelimited" << std::endl;

    std::cerr << size << std::endl;
    if (data == nullptr)
        return nullptr;
    if (size < 1)
        return nullptr;

    size_t length = static_cast<const uint8_t*>(data)[0];
    std::cerr << length << std::endl;
    if ((length + 1 > size) || (length == 0))
    {
        return nullptr;
    }

    std::shared_ptr<Message> message = std::make_shared<Message>();
    if (message->ParseFromArray(data + 1, length))
    {
        std::cerr << "PARSED" << std::endl;
        if (bytesConsumed != nullptr)
        {
            *bytesConsumed += size;
        }
    }else
    {
        std::cerr << "DIDN`T PARSED" << std::endl;
        return nullptr;
    }

    return message;
}

template <typename Message>
class DelimitedMessagesStreamParser
{
public:
    typedef std::shared_ptr<const Message> PointerToConstValue;
    std::list<PointerToConstValue> parse(const std::string &data)
    {
        for (const char byte : data)
        {
            m_buffer.push_back(byte);
        }

        std::list<typename DelimitedMessagesStreamParser<Message>::PointerToConstValue> messages;
        size_t bytesConsumed = 0;
        std::shared_ptr<Message> message = parseDelimited<Message>(m_buffer.data(), m_buffer.size(), &bytesConsumed);
        while (message != nullptr)
        {
            messages.push_back(message);
            std::vector<char>(m_buffer.begin() + bytesConsumed, m_buffer.end()).swap(m_buffer);
            bytesConsumed = 0;
            message = parseDelimited<Message>(m_buffer.data(), m_buffer.size(), &bytesConsumed);
        }
        return messages;
    }

private:
    std::vector<char> m_buffer;
};

#endif // STREAM_PARSER_H