#ifndef PARSERTOOLS_H
#define PARSERTOOLS_H

#include <list>
#include <vector>
#include <string>
#include <memory>

template <typename Message>
std::shared_ptr<Message> parseDelimited(const void *data, size_t size, size_t *bytesConsumed = 0)
{
    if (data == nullptr)
        return nullptr;
    if (size < 1)
        return nullptr;

    size_t length = static_cast<const uint8_t *>(data)[0];
    if ((length + 1 > size) || (length == 0))
    {
        return nullptr;
    }

    std::shared_ptr<Message> message = std::make_shared<Message>();
    if (message->ParseFromArray(data + 1, length))
    {
        if (bytesConsumed != nullptr)
        {
            *bytesConsumed += length + 1;
        }
    }
    else
    {
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
        m_buffer.insert(m_buffer.end(), data.begin(), data.end());

        std::list<typename DelimitedMessagesStreamParser<Message>::PointerToConstValue> messages;
        size_t bytesConsumed = 0;

        while (bytesConsumed < m_buffer.size())
        {
            std::shared_ptr<Message> message = parseDelimited<Message>(m_buffer.data() + bytesConsumed, m_buffer.size(), &bytesConsumed);
            if (message != nullptr)
                messages.push_back(message);
            else
                break;
        }
        if (bytesConsumed == 0)
            return messages;

        m_buffer.erase(m_buffer.begin(), m_buffer.begin() + bytesConsumed);
        return messages;
    }

private:
    std::vector<char> m_buffer;
};

#endif // PARSERTOOLS_H