/**
 * @file Server_Interceptor.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "Server_Interceptor.h"
#include "Shared_Interceptor.h"
#include "tao/OctetSeqC.h"
#include "ace/OS_NS_string.h"

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (void)
{
}

char *
Echo_Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup ("Echo_Server_Interceptor");
}

void
Echo_Server_Request_Interceptor::destroy (void)
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation =
    ri->operation ();

  if (ACE_OS::strcmp ("_is_a", operation.in ()) == 0)
    return;

  IOP::ServiceId id = ::service_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  if (sc->context_data.length() != magic_cookie_len
      || ACE_OS::memcmp(
                magic_cookie, sc->context_data.get_buffer(),
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
