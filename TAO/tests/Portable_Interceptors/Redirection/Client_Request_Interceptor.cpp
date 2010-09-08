// -*- C++ -*-
// $Id$

#include "Client_Request_Interceptor.h"
#include "testC.h"
#include "ace/Log_Msg.h"

Client_Request_Interceptor::Client_Request_Interceptor (
  const char *orb_id,
  const char *forward_str)
  : orb_id_ (CORBA::string_dup (orb_id)),
    orb_ (),
    request_count_ (0),
    forward_str_ (CORBA::string_dup (forward_str))
{
}

char *
Client_Request_Interceptor::name (void)
{
  return CORBA::string_dup ("Client_Request_Interceptor");
}

void
Client_Request_Interceptor::destroy (void)
{
}

void
Client_Request_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr)
{
  ++this->request_count_;
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  if (CORBA::is_nil (this->orb_.in ()))
  {
    int argc = 0;
    ACE_TCHAR **argv = 0;
    this->orb_ = CORBA::ORB_init (argc,
                                  argv,
                                  this->orb_id_.in ());
  }

  CORBA::Object_var forward =
  this->orb_->string_to_object (this->forward_str_.in ());

  // Notice that this is not a permanent forward.
  throw PortableInterceptor::ForwardRequest (forward.in ());
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
