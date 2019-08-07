#include <string>

#include <google/protobuf/message.h>
#include "Library/IPC/Devices.pb.h"

namespace usbguardNotifier
{
    namespace IPC
    {
        using MessageType = google::protobuf::Message;
        using MessagePointer = std::unique_ptr<MessageType>;

        uint32_t payloadTypeNameToNumber(std::string payload_type);

        std::string targetToStr(const uint32_t target);

        template<class C>
        class MessageObserver
        {
        public:
            using methodType = void (C::*)(MessagePointer&, MessagePointer&);

            MessageObserver(C& c, methodType method, const MessageType& factory)
                : _c(c),
                  _method(method),
                  _factory(factory)
            {
            }

            void run(MessagePointer& request, MessagePointer& response)
            {
                (_c.*_method)(request, response);
            }

            MessagePointer stringToMessagePointer(const std::string str)
            {
                MessagePointer message(_factory.New());
                message->ParseFromString(str);
                return message;
            }

            template<class T>
            static MessageObserver create(C& c, methodType method)
            {
                return MessageObserver(c, method, T::default_instance());
            }

        private:
            C& _c;
            methodType _method;
            const MessageType& _factory;
        };
    }
} /* namespace  usbguardNotifier */
