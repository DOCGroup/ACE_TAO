// $Id$

#include "server_interceptor.h"
#include "tao/OctetSeqC.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_string.h"
#include "ace/Thread.h"

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (void)
  : myname_ ("Echo_Server_Interceptor")
{
}

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (CORBA::Object_ptr forward_location)
  : myname_ ("Echo_Server_Interceptor"),
    forward_location_ (CORBA::Object::_duplicate (forward_location)),
    forward_location_done_ (false)
{
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor (void)
{
}

void
Echo_Server_Request_Interceptor::forward_reference (CORBA::Object_ptr forward_location)
{
  if (CORBA::is_nil (forward_location))
    throw CORBA::INV_OBJREF (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);
  this->forward_location_ = CORBA::Object::_duplicate (forward_location);
}

char *
Echo_Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy (void)
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var operation = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%C.receive_request_service_contexts from "
              "\"%C\"\n",
              this->myname_,
              operation.in ()));

  // Ignore the "_is_a" operation since it may have been invoked
  // locally on the server side as a side effect of another call,
  // meaning that the client hasn't added the service context yet.
  // Same goes for the shutdown call
  if (ACE_OS_String::strcmp ("_is_a", operation.in ()) == 0 ||
      ACE_OS_String::strcmp ("shutdown", operation.in ()) == 0)
    return;

  forward_location_done_ = true;

  ACE_DEBUG ((LM_DEBUG, "Sending LOCATION_FORWARD, current thread %i\n", ACE_Thread::self ()));

  throw PortableInterceptor::ForwardRequest (this->forward_location_.in ());
}


void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
    // No op
     ACE_UNUSED_ARG (ri);
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
    // No Op
     ACE_UNUSED_ARG (ri);
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr ri)
{

  // This will throw an exception if a location forward has not
  // occured.  If an exception is thrown then something is wrong with
  // the PortableInterceptor::ForwardRequest support.
  CORBA::Object_var forward = ri->forward_reference ();

  if (CORBA::is_nil (forward.in ()))
    throw CORBA::INTERNAL ();
}

bool
Echo_Server_Request_Interceptor::forward_location_done() const
{
  return forward_location_done_;
}
