#include "ClientRequest_Interceptor.h"

#include "tao/CORBA_String.h"
#include "ace/Log_Msg.h"

CORBA::Boolean ClientRequest_Interceptor::success_flag_ {};

char *
ClientRequest_Interceptor::name ()
{
  return CORBA::string_dup ("ClientRequest_Interceptor");
}

void
ClientRequest_Interceptor::destroy ()
{
}

void
ClientRequest_Interceptor::send_poll (
  PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "TAO_FT (%P|%t): ClientRequest_Interceptor::send_poll called for operation: %C\n",
              op.in ()));
}

void
ClientRequest_Interceptor::send_request (
  PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "ClientRequest_Interceptor::send_request called for operation: %C - things are going well...\n",
              op.in ()));
}

void
ClientRequest_Interceptor::receive_reply (
  PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
ClientRequest_Interceptor::receive_other (
  PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
ClientRequest_Interceptor::receive_exception (
  PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "ClientRequest_Interceptor::receive_exception called for operation: %C - test has succeeded...\n",
              op.in ()));
  success_flag_ = true;
}
