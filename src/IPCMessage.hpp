#include <string>

#include <google/protobuf/message.h>
#include "Library/IPC/Devices.pb.h"

namespace usbguardNotifier
{
    namespace IPC
    {
        using MessagePointer = std::unique_ptr<google::protobuf::Message>;

        uint32_t payloadTypeNameToNumber(std::string payload_type);

        template<class C>
        class MessageObserver
        {
        public:
            using methodType = void (C::*)(MessagePointer&, MessagePointer&);

            MessageObserver(C& c, methodType method)
                : _c(c),
                  _method(method)
            {
            }

            void run(MessagePointer& request, MessagePointer& response)
            {
                (_c.*_method)(request, response);
            }

            MessagePointer stringToMessagePointer(const std::string str)
            {
                // TODO
//              MessagePointer message();
//              message->ParseFromString(str);
                return nullptr;
            }
        private:
            C& _c;
            methodType _method;
        };
    }
} /* namespace  usbguardNotifier */
