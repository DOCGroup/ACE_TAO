/**
 * @file Server_Interceptor.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "Server_Interceptor.h"
#include "Shared_Interceptor.h"
#include "tao/OctetSeqC.h"
#include <cstring>

char *
Echo_Server_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Echo_Server_Interceptor");
}

void
Echo_Server_Request_Interceptor::destroy ()
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation =
    ri->operation ();

  if (std::strcmp ("_is_a", operation.in ()) == 0)
    return;

  IOP::ServiceId const id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  if (sc->context_data.length () != magic_cookie_len
      || std::memcmp (magic_cookie,
                      sc->context_data.get_buffer (),
                      magic_cookie_len) != 0)
    {
      throw CORBA::BAD_PARAM();
    }
}

void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Echo_Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}
