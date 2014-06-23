// $Id$

#include <stdio.h>
#include "server_interceptor.h"

const IOP::ServiceId service_id = 0xdeadbeef;
const char *request_msg = "12345678";


Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (void)
  : myname_ ("Echo_Server_Interceptor")
{
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor (void)
{
}

char *
Echo_Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy (void)
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var operation = ri->operation ();

  printf("%s.receive_request_service_contexts from "
              "\"%s\"\n",
              this->myname_,
              operation.in ());

  IOP::ServiceId id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  const char *buf =
    reinterpret_cast<const char *> (sc->context_data.get_buffer ());
#if 0
  ACE_DEBUG ((LM_DEBUG,
              "  Received service context: %C\n",
              buf));
#endif /*if 0*/

  if (strcmp (buf, request_msg) != 0)
    {
        printf("ERROR: Echo_Server_Request_Interceptor::receive_request_service_contexts: "
                    "Expected request service context to be: %s\n",
                    request_msg);
        _exit(1);
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
