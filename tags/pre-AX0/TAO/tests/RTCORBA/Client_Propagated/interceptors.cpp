// $Id$

#include "interceptors.h"
#include "tao/OctetSeqC.h"

ACE_RCSID (Client_Propagated,
           interceptors,
           "$Id$")

Client_Request_Interceptor::
Client_Request_Interceptor (const char *id)
  : myname_ ("Client_Request_Interceptor"),
    orb_id_ (CORBA::string_dup (id))
{
}

Client_Request_Interceptor::~Client_Request_Interceptor (void)
{
}

void
Client_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Client_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Client_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Client_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do Nothing
}

void
Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_TRY
    {
      // Try to get the RTCorbaPriority context.  If its not there,
      // report the fact via an exception.
      IOP::ServiceContext_var sc =
        ri->get_reply_service_context (IOP::RTCorbaPriority ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");

      ACE_DEBUG ((LM_DEBUG, "EXCEPTION was most likely caused by the lack "
                            "of an IOP::RTCorbaPriority context in the reply.\n"));
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      this->orb_ = CORBA::ORB_init (argc, 0,
                                    this->orb_id_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  CORBA::String_var operation = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var target = ri->target (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "%s.received_exception "
              "from \"%s\" on object: %s\n",
              this->myname_,
              operation.in (),
              ior.in ()));
  ACE_CHECK;

  // Try to get the RTCorbaPriority context.  If its not there,
  // report the fact via an exception.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (IOP::RTCorbaPriority ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
