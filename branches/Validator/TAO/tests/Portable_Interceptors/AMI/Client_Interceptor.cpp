/**
 * @file Client_Interceptor.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "Client_Interceptor.h"
#include "Shared_Interceptor.h"
#include "tao/OctetSeqC.h"


ACE_RCSID (AMI,
           Client_Interceptor,
           "$Id$")


unsigned long Echo_Client_Request_Interceptor::request_count = 0;
unsigned long Echo_Client_Request_Interceptor::reply_count = 0;
unsigned long Echo_Client_Request_Interceptor::other_count = 0;
unsigned long Echo_Client_Request_Interceptor::exception_count = 0;

Echo_Client_Request_Interceptor::
Echo_Client_Request_Interceptor (void)
{
}

char *
Echo_Client_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Echo_Client_Interceptor");
}

void
Echo_Client_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ERROR((LM_ERROR,
             "ERROR, unexpected interception point called send_poll()\n"));
  ACE_THROW(CORBA::BAD_PARAM ());
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  IOP::ServiceContext sc;
  sc.context_id = ::service_id;

  CORBA::Octet *buf = CORBA::OctetSeq::allocbuf(magic_cookie_len);
  ACE_OS::memcpy(buf, magic_cookie, magic_cookie_len);
  sc.context_data.replace (magic_cookie_len, magic_cookie_len, buf, 1);

  // Add this context to the service context list.
  ri->add_request_service_context (sc,
                                   0
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Check that the request service context can be retrieved.
  IOP::ServiceContext_var rc =
    ri->get_request_service_context (::service_id
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (rc->context_data.length() != magic_cookie_len
      || ACE_OS::memcmp(
             magic_cookie, rc->context_data.get_buffer(),
             magic_cookie_len) != 0
      )
    {
      ACE_THROW(CORBA::BAD_PARAM());
    }

  Echo_Client_Request_Interceptor::request_count++;
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Check that the request service context can be retrieved.
  IOP::ServiceContext_var rc =
    ri->get_request_service_context (::service_id
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (rc->context_data.length() != magic_cookie_len
      || ACE_OS::memcmp(
             magic_cookie, rc->context_data.get_buffer(),
             magic_cookie_len) != 0
      )
    {
      ACE_THROW(CORBA::BAD_PARAM());
    }

  Echo_Client_Request_Interceptor::reply_count++;
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Check that the request service context can be retrieved.
  IOP::ServiceContext_var rc =
    ri->get_request_service_context (::service_id
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (rc->context_data.length() != magic_cookie_len
      || ACE_OS::memcmp(
             magic_cookie, rc->context_data.get_buffer(),
             magic_cookie_len) != 0
      )
    {
      ACE_THROW (CORBA::BAD_PARAM ());
    }

  Echo_Client_Request_Interceptor::other_count++;
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  Echo_Client_Request_Interceptor::exception_count++;
}
