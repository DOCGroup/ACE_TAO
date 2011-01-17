// -*- C++ -*-
// $Id$

#include "Server_Request_Interceptor.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "testS.h"

Server_Request_Interceptor::Server_Request_Interceptor (CORBA::ULong request_pass_count)
  : request_pass_count_ (request_pass_count)
  , request_count_ (0)
  , to_ (CORBA::Object::_nil ())
{
}

void
Server_Request_Interceptor::forward (
  CORBA::Object_ptr to)
{
  if (CORBA::is_nil (to))
    throw CORBA::INV_OBJREF (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);

  ACE_TCHAR *argv[] = {NULL};
  int   argc = 0;

  // Fetch the ORB having been initialized in main()
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv, "Server ORB");

  CORBA::String_var s_to = orb->object_to_string (to);

  this->to_ = orb->string_to_object (s_to.in ());
}

char *
Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy (void)
{
  CORBA::release (this->to_);
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  this->request_count_++;
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  if (this->request_count_ == this->request_pass_count_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "SERVER: Request %d will be forwarded "
                  "to object 'to'\nSERVER: via "
                  "receive_request_service_contexts().\n",
                  this->request_count_));

      // Throw forward exception
      throw PortableInterceptor::ForwardRequest (this->to_);
    }

  return;
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}
