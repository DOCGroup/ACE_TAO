// -*- C++ -*-
#include "Client_Request_Interceptor.h"
#include "testC.h"
#include "ace/Log_Msg.h"
#include <cstring>

extern bool interceptor_invoked;

Client_Request_Interceptor::Client_Request_Interceptor (
    char const *orb_id)
  : orb_id_ (CORBA::string_dup (orb_id))
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
      int argc {};
      ACE_TCHAR **argv {};
      this->orb_ = CORBA::ORB_init (argc, argv, this->orb_id_.in ());
    }

  CORBA::String_var const exception_id = ri->received_exception_id ();
  ACE_DEBUG ((LM_DEBUG,
              "Client_Request_Interceptor::receive_exception (%s)\n",
              exception_id.in ()));

  if (std::strcmp (exception_id.in (), "IDL:omg.org/CORBA/TRANSIENT:1.0") == 0)
    {
      interceptor_invoked = true;
    }
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
