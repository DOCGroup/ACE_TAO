// $Id$

#include "interceptors.h"
#include "tao/OctetSeqC.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_string.h"
#include "ace/Thread.h"

ACE_RCSID (Bug_1495_Regression_Test,
           interceptors,
           "$Id$")


Echo_Client_Request_Interceptor::
Echo_Client_Request_Interceptor (const char *id)
  : myname_ ("Echo_Client_Interceptor"),
    orb_id_ (CORBA::string_dup (id))
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor (void)
{
}

char *
Echo_Client_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
    ACE_UNUSED_ARG (ri);
// No Op
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    ACE_UNUSED_ARG (ri);
    // No Op
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
 ACE_UNUSED_ARG (ri);
// no op
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
    // No op
     ACE_UNUSED_ARG (ri);
}

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
Echo_Server_Request_Interceptor::forward_reference (CORBA::Object_ptr forward_location
                                                    ACE_ENV_ARG_DECL)
                                                    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (forward_location))
    ACE_THROW (CORBA::INV_OBJREF (
                 CORBA::SystemException::_tao_minor_code (
                   TAO::VMCID,
                   EINVAL),
                 CORBA::COMPLETED_NO));
  this->forward_location_ = CORBA::Object::_duplicate (forward_location);
}

char *
Echo_Server_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.receive_request_service_contexts from "
              "\"%s\"\n",
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

  ACE_THROW (PortableInterceptor::ForwardRequest (this->forward_location_, 0));

}


void
Echo_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    // No op
     ACE_UNUSED_ARG (ri);
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
    // No Op
     ACE_UNUSED_ARG (ri);
}

void
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr ri
             ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{

  // This will throw an exception if a location forward has not
  // occured.  If an exception is thrown then something is wrong with
  // the PortableInterceptor::ForwardRequest support.
  CORBA::Object_var forward = ri->forward_reference (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (forward.in ()))
    ACE_THROW (CORBA::INTERNAL ());
}

bool
Echo_Server_Request_Interceptor::forward_location_done() const
{
  return forward_location_done_;
}
