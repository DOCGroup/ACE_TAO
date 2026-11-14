/**
 * @file Client_Interceptor.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "Client_Interceptor.h"
#include "Shared_Interceptor.h"
#include "tao/OctetSeqC.h"

#include "ace/Log_Msg.h"

#include <cstring>

unsigned long Echo_Client_Request_Interceptor::request_count {};
unsigned long Echo_Client_Request_Interceptor::reply_count {};
unsigned long Echo_Client_Request_Interceptor::other_count {};
unsigned long Echo_Client_Request_Interceptor::exception_count {};

char *
Echo_Client_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Echo_Client_Interceptor");
}

void
Echo_Client_Request_Interceptor::destroy ()
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  ACE_ERROR((LM_ERROR,
             "ERROR, unexpected interception point called send_poll()\n"));
  throw CORBA::BAD_PARAM ();
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  IOP::ServiceContext sc {};
  sc.context_id = ::service_id;

  CORBA::Octet *const buf = CORBA::OctetSeq::allocbuf (magic_cookie_len);
  std::memcpy (buf, magic_cookie, magic_cookie_len);
  sc.context_data.replace (magic_cookie_len, magic_cookie_len, buf, true);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, false);

  // Check that the request service context can be retrieved.
  IOP::ServiceContext_var rc =
    ri->get_request_service_context (::service_id);

  if (rc->context_data.length () != magic_cookie_len
      || std::memcmp (magic_cookie,
                      rc->context_data.get_buffer (),
                      magic_cookie_len) != 0)
    {
      throw CORBA::BAD_PARAM();
    }

  ++Echo_Client_Request_Interceptor::request_count;
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  // Check that the request service context can be retrieved.
  IOP::ServiceContext_var rc =
    ri->get_request_service_context (::service_id);

  if (rc->context_data.length () != magic_cookie_len
      || std::memcmp (magic_cookie,
                      rc->context_data.get_buffer (),
                      magic_cookie_len) != 0)
    {
      throw CORBA::BAD_PARAM();
    }

  ++Echo_Client_Request_Interceptor::reply_count;
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  // Check that the request service context can be retrieved.
  IOP::ServiceContext_var rc =
    ri->get_request_service_context (::service_id);

  if (rc->context_data.length () != magic_cookie_len
      || std::memcmp (magic_cookie,
                      rc->context_data.get_buffer (),
                      magic_cookie_len) != 0)
    {
      throw CORBA::BAD_PARAM ();
    }

  ++Echo_Client_Request_Interceptor::other_count;
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  Echo_Client_Request_Interceptor::exception_count++;
}
