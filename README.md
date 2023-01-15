h4. Краткое описание задания
Разработать функцию и класс (и модульные тесты к ним) по требованиям ниже, которые будут делать разбор потока length-prefixed Protobuf сообщений

h4. Протокол
{code:java}
syntax = "proto2";

package TestTask.Messages;

message WrapperMessage {
	optional FastResponse fast_response = 1;
	optional SlowResponse slow_response = 2;
	optional RequestForFastResponse request_for_fast_response = 3;
	optional RequestForSlowResponse request_for_slow_response = 4;
}

message FastResponse {
	required string current_date_time = 1;
}

message SlowResponse {
	required uint32 connected_client_count = 1;
}

message RequestForFastResponse {

}

message RequestForSlowResponse {
	required uint32 time_in_seconds_to_sleep = 1;
}
{code}
Особенности разбора сообщений Protobuf
* каждое сообщение будет предварять его размер (length-prefixed). Для размера должен быть использован тип Varint32. https://developers.google.com/protocol-buffers/docs/techniques#streaming
* Использоваться всегда будет  WrapperMessage(заполнено только одно из опциональных полей – только так можно понять при разборе какое именно у нас сообщение)
* Время должно быть в ISO формате: YYYYMMDDThhmmss.fff например: 19851019T050107.333 - 19 октября 1985 года 5 часов 1 минута 7секунд 333 миллисекунды. Кодировка строк – UTF8.
* Следует обратить внимание, что при разборе сообщений необходимо учесть, что в общем случае может поступить на вход любая часть сообщения, несколько сообщений сразу – в общем любой кусок потока (нельзя даже рассчитывать, что предваряющий размер сообщения поступит полностью). Поэтому необходимо при разборе обязательно использовать буфер

h4. Описание требований к API
h5. Интерфейс функции разбора одного сообщения из потока
{code:cpp}
      /*!
       * \brief Расшифровывает сообщение, предваренное длиной из массива байтов.
       *
       * \tparam Message Тип сообщения, для работы с которым предназначена данная функция.
       *
       * \param data Указатель на буфер данных.
       * \param size Размер буфера данных.
       * \param bytesConsumed Количество байт, которое потребовалось для расшифровки сообщения в случае успеха.
       *
       * \return Умный указатель на сообщение. Если удалось расшифровать сообщение, то он не пустой.
       */
      template<typename Message>
      std::shared_ptr<Message> parseDelimited(const void* data, size_t size, size_t* bytesConsumed = 0);
{code}
Эта функция должна возвращать первое сообщение из буфера data, если оно там есть.

h5. Интерфейс класса разбора потока сообщений
{code:cpp}
      template<typename MessageType>
      class DelimitedMessagesStreamParser
      {
      public:
        typedef std::shared_ptr<const MessageType> PointerToConstValue;

        std::list<PointerToConstValue> parse(const std::string& data);
    

      private:
        std::vector<char> m_buffer;
      };
{code}
Этот класс должен возвращать список сообщений из строки data.
Он должен использовать в реализации функции parse() функцию  parseDelimited().

h5. Вспомогательная функция для сериализации сообщений в поток байт (нужна для тестов):
{code:cpp}
#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif
typedef std::vector<char> Data;
typedef std::shared_ptr<const Data> PointerToConstData;
    template <typename Message> PointerToConstData serializeDelimited(const Message& msg)
      {
        const size_t messageSize = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
        const size_t headerSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);

        const PointerToData& result = boost::make_shared<Data>(headerSize + messageSize);
        google::protobuf::uint8* buffer = reinterpret_cast<google::protobuf::uint8*>(&*result->begin());

        google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buffer);
        msg.SerializeWithCachedSizesToArray(buffer + headerSize);

        return result;
      }
{code}

h5. Пример использования объекта класса DelimitedMessagesStreamParser:
{code:cpp}
      std::vector<char> messages; 
      // тут код заполнения messages с помощью  serializeDelimited
      typedef DelimitedMessagesStreamParser<WrapperMessage> Parser;
      Parser parser;
   
      // идем по одному байту по входному потоку сообщений
      for(const char byte, messages)
      {
        const std::list<PointerToConstValue>& parsedMessages = parser.parse(std::string(1, byte));
        for(const PointerToConstValue& value, parsedMessages)
        {
          // добавляем куда-то все сообщения
        }
            
 
      }

      // тут код проверки, что все сообщения расшифровались верно
{code}

h4. Общие требования
* На функцию  parseDelimited() и класс  DelimitedMessagesStreamParser должны быть написаны модульные тесты с помощью Google Test
* Проект должен собираться с помощью CMake
