// $Id$

#include "ClientRequest_Interceptor.h"

#include "tao/CORBA_String.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

  CORBA::Boolean
  ClientRequest_Interceptor::success_flag_ = 0;

  ClientRequest_Interceptor::ClientRequest_Interceptor (void)
    : name_ ("ClientRequest_Interceptor")
  {
  }

  ClientRequest_Interceptor::~ClientRequest_Interceptor (void)
  {
  }

  char *
  ClientRequest_Interceptor::name ()
  {
    return CORBA::string_dup (this->name_);
  }

  void
  ClientRequest_Interceptor::destroy ()
  {
    // No-op
  }

  void
  ClientRequest_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr ri
    )
  {
    // Print debug
    CORBA::String_var op =
      ri->operation ();

    ACE_DEBUG ((LM_DEBUG,
               "TAO_FT (%P|%t): ClientRequest_Interceptor::send_poll called for operation: %C\n",
                    op.in ()));
  }

  void
  ClientRequest_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
  {
    // Print debug
    CORBA::String_var op =
      ri->operation ();

    ACE_DEBUG ((LM_DEBUG,
               "ClientRequest_Interceptor::send_request called for operation: %C - things are going well...\n",
                    op.in ()));
  }

  void
  ClientRequest_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
  {
    // No-op
  }

  void
  ClientRequest_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
  {
    // No-op
  }

  void
  ClientRequest_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
  {
    // Print debug
    CORBA::String_var op =
      ri->operation ();

    ACE_DEBUG ((LM_DEBUG,
               "ClientRequest_Interceptor::receive_exception called for operation: %C - test has succeeded...\n",
                op.in ()));
    success_flag_ = 1;
  }
