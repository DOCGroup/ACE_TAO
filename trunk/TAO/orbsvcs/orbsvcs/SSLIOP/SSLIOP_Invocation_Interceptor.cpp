#include "orbsvcs/SSLIOP/SSLIOP_Invocation_Interceptor.h"

#include "orbsvcs/SecurityLevel2C.h"

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include "tao/debug.h"

#if defined(SSLIOP_DEBUG_PEER_CERTIFICATE)
#include <openssl/x509.h>   // @@ For debugging code below
#endif /* DEBUG_PEER_CERTIFICATES */

ACE_RCSID (SSLIOP,
           SSLIOP_Invocation_Interceptor,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Server_Invocation_Interceptor::Server_Invocation_Interceptor (
   ::SSLIOP::Current_ptr current,
   ::Security::QOP qop)
  : ssliop_current_ (::SSLIOP::Current::_duplicate (current)),
    qop_ (qop)
{
}

TAO::SSLIOP::Server_Invocation_Interceptor::~Server_Invocation_Interceptor (
  void)
{
}

char *
TAO::SSLIOP::Server_Invocation_Interceptor::name ()
{
  return CORBA::string_dup ("TAO::SSLIOP::Server_Invocation_Interceptor");
}

void
TAO::SSLIOP::Server_Invocation_Interceptor::destroy ()
{
}


void
TAO::SSLIOP::Server_Invocation_Interceptor::receive_request_service_contexts (
                                              PortableInterceptor::ServerRequestInfo_ptr /*ri*/)
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
  CORBA::Boolean const no_ssl =
    this->ssliop_current_->no_context ();

  if (TAO_debug_level >= 3)
    ACE_DEBUG ((LM_DEBUG, "SSLIOP (%P|%t) Interceptor (context), ssl=%d\n", !(no_ssl)));

  if (no_ssl && this->qop_ != ::Security::SecQOPNoProtection)
    throw CORBA::NO_PERMISSION ();

#if defined(DEBUG_PEER_CERTIFICATES)
  try
    {
      // If the request was not made through an SSL connection, then
      // this method will throw the SSLIOP::Current::NoContext
      // exception.  Otherwise, it will return a DER encoded X509
      // certificate.
      ::SSLIOP::ASN_1_Cert_var cert =
        this->ssliop_current_->get_peer_certificate ();

      // @@ The following debugging code works but I don't think that
      //    we should include it since it dumps alot of information,
      //    i.e. prints two lines of information per request.
      if (TAO_debug_level > 1)
        {
          CORBA::Octet *der_cert = cert->get_buffer ();

          X509 *peer = ::d2i_X509 (0, &der_cert, cert->length ());
          if (peer != 0)
            {
              char buf[BUFSIZ] = { 0 };

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
  catch (const ::SSLIOP::Current::NoContext& )
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
      if (this->qop_ != ::Security::SecQOPNoProtection)
        throw CORBA::NO_PERMISSION ();
    }
#endif /* DEBUG_PEER_CERTIFICATES */
}


void
TAO::SSLIOP::Server_Invocation_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
{
}

void
TAO::SSLIOP::Server_Invocation_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
{
}

void
TAO::SSLIOP::Server_Invocation_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
{
}

void
TAO::SSLIOP::Server_Invocation_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
