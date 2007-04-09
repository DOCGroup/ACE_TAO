#include "orbsvcs/SSLIOP/SSLIOP_Invocation_Interceptor.h"
#include "orbsvcs/SSLIOP/SSLIOP_Current.h"

#include "orbsvcs/SecurityLevel2C.h"

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include "tao/PortableServer/POAC.h"
#include "tao/debug.h"

#if defined (SSLIOP_DEBUG_PEER_CERTIFICATE)
#include <openssl/x509.h>   // @@ For debugging code below
#endif /* SSLIOP_DEBUG_PEER_CERTIFICATE */

ACE_RCSID (SSLIOP,
           SSLIOP_Invocation_Interceptor,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Server_Invocation_Interceptor::Server_Invocation_Interceptor
(
  PortableInterceptor::ORBInitInfo_ptr info,
  ::Security::QOP default_qop,
  size_t tss_slot
)
: qop_ (default_qop)
{
  /*
   * Cache references to the "Current" objects that we'll need during
   * during invocations.
   */

  CORBA::Object_var obj =
    info->resolve_initial_references ("SSLIOPCurrent");

  this->ssliop_current_ = ::SSLIOP::Current::_narrow (obj.in ());

  if (!CORBA::is_nil (this->ssliop_current_.in ()))
    {
      TAO::SSLIOP::Current *tao_current =
        dynamic_cast<TAO::SSLIOP::Current *> (this->ssliop_current_.in ());

      if (tao_current != 0)
        {
          if (TAO_debug_level > 3)
            ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) SSLIOP_Invocation_Interceptor::CTOR--setting up SSLIOP Current with slot %d\n", tss_slot));
          tao_current->tss_slot (tss_slot);
        }
      else
        throw CORBA::INTERNAL ();
    }

  obj = info->resolve_initial_references ("SecurityLevel2:SecurityManager");
  this->sec2manager_ = SecurityLevel2::SecurityManager::_narrow (obj.in ());

  if (! CORBA::is_nil (this->sec2manager_.in ()))
    {
      // set the slot id?
    }

  obj = info->resolve_initial_references ("POACurrent");
  this->poa_current_ = PortableServer::Current::_narrow (obj.in ());
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
#if 0
  CORBA::Boolean const no_ssl = this->ssliop_current_->no_context ();

  if (TAO_debug_level >= 3)
    ACE_DEBUG ((LM_DEBUG, "SSLIOP (%P|%t) Interceptor (context), ssl=%d\n", !(no_ssl)));

  if (no_ssl && this->qop_ != ::Security::SecQOPNoProtection)
    throw CORBA::NO_PERMISSION ();
#endif
#if defined(SSLIOP_DEBUG_PEER_CERTIFICATE)
  try
    {
      // If the request was not made through an SSL connection, then
      // this method will throw the SSLIOP::Current::NoContext
      // exception. Otherwise, it will return a DER encoded X509
      // certificate.
      ::SSLIOP::ASN_1_Cert_var cert =
        this->ssliop_current_->get_peer_certificate ();

      // @@ The following debugging code works but I don't think that
      //    we should include it since it dumps alot of information,
      //    i.e. prints two lines of information per request.
      if (TAO_debug_level > 1)
        {
          const CORBA::Octet *der_cert = cert->get_buffer ();

          ::X509 *peer = ::d2i_X509 (0, &der_cert, cert->length ());
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
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) No certificate info\n"));
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
#endif /* SSLIOP_DEBUG_PEER_CERTIFICATE */
}


void
TAO::SSLIOP::Server_Invocation_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri )
{
  SecurityLevel2::AccessDecision_var ad =
    this->sec2manager_->access_decision ();

  
  CORBA::Boolean const no_ssl =
    this->ssliop_current_->no_context ();

  if (TAO_debug_level >= 3)
    ACE_DEBUG ((LM_DEBUG, "SSLIOP (%P|%t) Interceptor (context), ssl=%d\n", !(no_ssl)));

  // if
  // (1) no SSL session state is available (which means that the
  //     invocation is received across a non-SSL transport)
  // AND
  // (2) the required Quality of Protection is something other
  //     than SecQOPNoProtection (set via -SSLNoProtection)
  if (no_ssl && this->qop_ != ::Security::SecQOPNoProtection)
    {
      /*
       * Set up all the arguments needed by the call
       * to AccessDecision::access_allowed()
       */
      
      /* Get the credentials from SSLIOP */
      SecurityLevel2::CredentialsList cred_list; // initial empty?
#if 0
      try {
        SecurityLevel2::ReceivedCredentials_var rcvd_creds =
          this->sec2_current_->received_credentials ();
        // this gets the credentials received from the other side.  We
        // should be able to put this into a CredentialsList with no
        // problem.
        //
        // Do I really need to implement a sec2_current, or can I hack
        // the conversion at this level?  I probably ought to do it as
        // a real sec2_current with the conversion from sec3->sec2
        // happening at a lower level.
        
	cred_list.length(1);
	cred_list[0] = rcvd_creds.in ();
	/*
	  So, in looking for how we can do this, I find that the
	  SL3_SecurityCurrent::client_credentials() delegates to SL3_SecurityCurrent_Impl::client_credentials(), which is pure virtual.
	*/
      }
      catch (...) {
      }
#endif

      /* Get the target object */
      CORBA::Object_var target = CORBA::Object::_nil ();

      target = this->poa_current_->get_reference ();

      CORBA::String_var operation_name = ri->operation ();
      CORBA::String_var target_interface_name = ri->target_most_derived_interface(); // is this the repository ID?

      CORBA::Boolean it_should_happen = false;
      it_should_happen = ad->access_allowed (cred_list,
					     target.in(),
					     operation_name.in(),
					     target_interface_name.in());
      if (TAO_debug_level >= 3)
	ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) SL2::access_allowed returned %s\n",
		    it_should_happen ? "true" : "false"));

      if (! it_should_happen)
	throw CORBA::NO_PERMISSION ();
    }
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
