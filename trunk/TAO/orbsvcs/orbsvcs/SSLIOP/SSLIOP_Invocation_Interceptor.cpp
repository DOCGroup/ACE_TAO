// -*- C++ -*-
//
// $Id$

#include "SSLIOP_Invocation_Interceptor.h"

#include "tao/debug.h"

// #include <openssl/x509.h>   // @@ For debugging code below

ACE_RCSID (TAO_SSLIOP, SSLIOP_Invocation_Interceptor, "$Id$")

TAO_SSLIOP_Server_Invocation_Interceptor::
TAO_SSLIOP_Server_Invocation_Interceptor (SSLIOP::Current_ptr current,
                                          Security::QOP qop)
  : ssliop_current_ (SSLIOP::Current::_duplicate (current)),
    qop_ (qop)
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
TAO_SSLIOP_Server_Invocation_Interceptor::destroy (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


void
TAO_SSLIOP_Server_Invocation_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
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
  CORBA::Boolean no_ssl =
    this->ssliop_current_->no_context (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (no_ssl && this->qop_ != Security::SecQOPNoProtection)
    ACE_THROW (CORBA::NO_PERMISSION ());

#if 0
  ACE_TRY
    {
      // If the request was not made through an SSL connection, then
      // this method will throw the SSLIOP::Current::NoContext
      // exception.  Otherwise, it will return a DER encoded X509
      // certificate.
      SSLIOP::ASN_1_Cert_var cert =
        this->ssliop_current_->get_peer_certificate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // @@ The following debugging code works but I don't think that
      //    we should include it since it dumps alot of information,
      //    i.e. prints two lines of information per request.
      if (TAO_debug_level > 1)
        {
          CORBA::Octet *der_cert = cert->get_buffer ();

          X509 *peer = ::d2i_X509 (0, &der_cert, cert->length ());
          if (peer != 0)
            {
              char buf[BUFSIZ];

              ::X509_NAME_oneline (::X509_get_subject_name (peer),
                                   buf,
                                   BUFSIZ);

              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) Certificate subject: %s\n",
                          buf));

              ::X509_NAME_oneline (::X509_get_issuer_name (peer),
                                   buf,
                                   BUFSIZ);

              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) Certificate issuer: %s\n",
                          buf));


              ::X509_free (peer);
            }
        }
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
      if (this->qop_ != Security::SecQOPNoProtection)
        ACE_THROW (CORBA::NO_PERMISSION ());
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* 0 */
}

void
TAO_SSLIOP_Server_Invocation_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
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
