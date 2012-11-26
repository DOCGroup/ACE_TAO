// -*- C++ -*-
// $Id$

#include "Client_Request_Interceptor.h"
#include "testC.h"
#include "ace/Log_Msg.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "ace/OS_NS_string.h"

Client_Request_Interceptor::Client_Request_Interceptor (
  const char *orb_id,
  const char *first_forward_str, const char *second_forward_str)
  : orb_id_ (CORBA::string_dup (orb_id)),
    orb_ (),
    request_count_ (0),
    exception_count_ (0),
    first_forward_str_ (CORBA::string_dup (first_forward_str)),
    second_forward_str_ (CORBA::string_dup (second_forward_str))
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
  ++this->exception_count_;
  ACE_DEBUG ((LM_DEBUG, "received exception %d\n",
              this->exception_count_));
  if (CORBA::is_nil (this->orb_.in ()))
  {
    int argc = 0;
    ACE_TCHAR **argv = 0;
    this->orb_ = CORBA::ORB_init (argc, argv, this->orb_id_.in ());
  }

  if (this->exception_count_ == 1)
    {
      ACE_DEBUG ((LM_DEBUG, "forwarding client to the second server\n"));
      CORBA::Object_var first_forward =
        this->orb_->string_to_object (this->first_forward_str_.in ());

      // Notice that this is not a permanent forward.
      throw PortableInterceptor::ForwardRequest (first_forward.in ());
    }
  else if (this->exception_count_ == 2)
    {
      ACE_DEBUG ((LM_DEBUG, "forwarding client to the third server\n"));
      CORBA::Object_var second_forward =
        this->orb_->string_to_object (this->second_forward_str_.in ());

      // Notice that this is not a permanent forward.
      throw PortableInterceptor::ForwardRequest (second_forward.in ());
    }
  else if (this->exception_count_ == 3)
    {
      CORBA::Any_var ex = ri->received_exception ();

      CORBA::TypeCode_var tc;
      const char * id = 0;
      tc = ex->type ();
      id = tc->id ();

      if (ACE_OS_String::strcmp (id,
                                 "IDL:omg.org/CORBA/TRANSIENT:1.0") == 0)
          throw ::CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
