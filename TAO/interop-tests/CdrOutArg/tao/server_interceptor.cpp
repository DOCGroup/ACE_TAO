#include "server_interceptor.h"
#include "tao/OctetSeqC.h"

#include "ace/Log_Msg.h"
#include <cstring>

constexpr IOP::ServiceId service_id = 0xdeadbeef;
constexpr char request_msg[] = "12345678";

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor ()
  : myname_ ("Echo_Server_Interceptor")
{
}

char *
Echo_Server_Request_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy ()
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%C.receive_request_service_contexts from "
              "\"%C\"\n",
              this->myname_,
              operation.in ()));

  IOP::ServiceId id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  size_t const message_length = sizeof (request_msg);
  if (sc->context_data.length () != message_length
      || std::memcmp (sc->context_data.get_buffer (), request_msg, message_length) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Echo_Server_Request_Interceptor::receive_request_service_contexts: "
                    "Expected request service context to be: %C\n",
                    request_msg));
    }

}

void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr )
{
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr )
{
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr)
{
}
