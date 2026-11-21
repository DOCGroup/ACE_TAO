// -*- C++ -*-
#include "Client_Request_Interceptor.h"
#include "testC.h"
#include "ace/Log_Msg.h"

Client_Request_Interceptor::Client_Request_Interceptor (
  char const *orb_id,
  char const *forward_str)
  : orb_id_ (CORBA::string_dup (orb_id)),
    forward_str_ (CORBA::string_dup (forward_str))
{
}

char *
Client_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Client_Request_Interceptor");
}

void
Client_Request_Interceptor::destroy ()
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
      int argc {};
      ACE_TCHAR **argv {};
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    this->orb_id_.in ());
    }

  CORBA::Object_var const forward =
    this->orb_->string_to_object (this->forward_str_.in ());

  // Notice that this is not a permanent forward.
  throw PortableInterceptor::ForwardRequest (forward.in ());
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
