// -*- C++ -*-
//
// $Id$

#include "SSLIOP_Invocation_Interceptor.h"

ACE_RCSID (TAO_SSLIOP, SSLIOP_Invocation_Interceptor, "$Id$")

TAO_SSLIOP_Server_Invocation_Interceptor::
TAO_SSLIOP_Server_Invocation_Interceptor (SSLIOP::Current_ptr current,
                                          int no_protection)
  : ssliop_current_ (SSLIOP::Current::_duplicate (current)),
    no_protection_ (no_protection)
{
}

TAO_SSLIOP_Server_Invocation_Interceptor::
~TAO_SSLIOP_Server_Invocation_Interceptor (void)
{
}

char *
TAO_SSLIOP_Server_Invocation_Interceptor::name (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO_SSLIOP_Server_Invocation_Interceptor");
}

void
TAO_SSLIOP_Server_Invocation_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_SSLIOP_Server_Invocation_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // @@ TODO: Once the receive_request_service_contexts() interception
  //          point is implemented in TAO, move this secure invocation
  //          verification code to the
  //          receive_request_service_contexts() method in this class,
  //          since it is better to reject a request as early as
  //          possible.

  TAO_ENV_ARG_DEFN;

  ACE_TRY
    {
      SSLIOP::SSL_Cert_var cert_chain =
        this->ssliop_current_->get_peer_certificate_chain (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (SSLIOP::Current::NoContext, exc)
    {
      // The current upcall is not being performed through an SSL
      // connection.  If server is configured to disallow insecure
      // invocations then throw a CORBA::NO_PERMISSION exception.
      // @@ TODO: Once the SecurityManager is implemented, query it
      //          for the current object's
      //          SecureInvocationPolicy of type
      //          SecTargetSecureInvocationPolicy so that we can
      //          accept or reject requests on a per-object basis
      //          instead on a per-endpoint basis.
      if (this->no_protection_ == 0)
        ACE_THROW (CORBA::NO_PERMISSION ());
    }
  ACE_ENDTRY;
}

void
TAO_SSLIOP_Server_Invocation_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_SSLIOP_Server_Invocation_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_SSLIOP_Server_Invocation_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
