// -*- C++ -*-
// $Id$

#include "Client_Request_Interceptor.h"
#include "testC.h"
#include "ace/Log_Msg.h"

extern bool interceptor_invoked;

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
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (CORBA::is_nil (this->orb_.in ()))
  {
    int argc = 0;
    ACE_TCHAR **argv = 0;
    this->orb_ = CORBA::ORB_init (argc,
                                  argv,
                                  this->orb_id_.in ());
  }

  ACE_DEBUG ((LM_DEBUG,
              "Client_Request_Interceptor::receive_exception (%s)\n",
              ri->received_exception_id ()));

  if (ACE_OS::strcmp (ri->received_exception_id (),
                      "IDL:omg.org/CORBA/TRANSIENT:1.0") == 0)
    {
      interceptor_invoked = true;
    }
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
