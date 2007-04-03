#include "orbsvcs/SSLIOP/SSLIOP_Invocation_Interceptor.h"
#include "orbsvcs/SSLIOP/SSLIOP_Current.h"

#include "orbsvcs/SecurityLevel2C.h"

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include "tao/PortableServer/POAC.h"
#include "tao/debug.h"

#if defined(SSLIOP_DEBUG_PEER_CERTIFICATE)
#include <openssl/x509.h>   // @@ For debugging code below
#endif /* DEBUG_PEER_CERTIFICATES */

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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO::SSLIOP::Server_Invocation_Interceptor");
}

void
TAO::SSLIOP::Server_Invocation_Interceptor::destroy ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


void
TAO::SSLIOP::Server_Invocation_Interceptor::receive_request_service_contexts (
	PortableInterceptor::ServerRequestInfo_ptr /* ri */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
#if 1
  // This operation is where the old security implementation used to
  // makes its simplistic decision whether to permit or deny, but with
  // the introduction of more detailed checking, there's a need to
  // have access to information that's not available until later in
  // the PI flow.  So, it's now all moved into receive_request().


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
	throw CORBA::NO_PERMISSION ();
#endif
}


void
TAO::SSLIOP::Server_Invocation_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
#if 0
  // The current upcall is not being performed through an SSL
  // connection.  If server is configured to disallow insecure
  // invocations then throw a CORBA::NO_PERMISSION exception.
  // @@ TODO: Once the SecurityManager is implemented, query it
  //          for the current object's
  //          SecureInvocationPolicy of type
  //          SecTargetSecureInvocationPolicy so that we can
  //          accept or reject requests on a per-object basis
  //          instead on a per-endpoint basis.

#if 1
  /*
  // Of course, since we're implementing a SecurityManager, we can do
  // this now!  But we want to keep the default behavior, too...

  Q: It's also not clear how we're supposed to query the securitymanager
  "for the current object's SecureInvocationPolicy".  Maybe we need to
  query the policies on the object itself?  Maybe we can ask the
  ServerRequestInfo via get_server_policy()?

  A: The spec says that "This operation returns the policy in effect
  for this operation for the given policy type.  The returned
  CORBA::Policy object shall only be a policy whose type was
  registered via register_policy_factory()", so that should give us
  what we want.
  */
  
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
#if 0
	throw CORBA::NO_PERMISSION ();
  if (   ! CORBA:is_nil (ad.in ())
      && ri->get_server_policy(SecureInvocationPolicy) == SecTargetSecureInvocationPolicy)
#endif
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

#if 0 // we want to use the POACurrent::get_reference()!
      // I suppose that we can derive this using server_id, adapter_id
      // and object_id using id_to_reference.  One problem is that,
      // according to the spec, id_to_reference() can only be used
      // when the POA in question has the RETAIN policy so this would
      // be unavailable when the POA has NO_RETAIN.
      try {
	int no_args = 0;
	CORBA::ORB_var orb = CORBA::ORB_init (no_args, 0, ri->orb_id());
	CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
	PortableServer::POA_var rootpoa = 
	  PortableServer::POA::_narrow (obj.in ());
	if (CORBA::is_nil (rootpoa.in ()))
	  // Probably not a good choice for exception, but we need to
	  // throw something.
	  throw PortableServer::POA::AdapterNonExistent();
	
	// @@CJC Grrr...of course nothing is ever easy.  What we'd really
	// like to be able to do is something like...
	//    rootpoa->find_POA (ri->adapter_id (), false); // don't activate
	// to get a reference to the POA indicated here.  Sigh.  CORBA 3.0
	// provides adapter_name(), so maybe I'll get lucky and that'll be
	// available.  Otherwise, I'm going to have to resort to ugliness
	// and proprietary hacks to get this info, b/c adapter_id()
	// returns an opaque octet sequence.
	//
	// The good news is that adapter_name() exists, but it doesn't
	// return a name suitable for submission to find_POA().  Rather,
	// it returns a string sequence.  Exactly what's in that sequence
	// we don't know, but I'll guess that it's supposed to be an in-order
	// list of the poa path down to the poa that's hosting the target
	// CORBA Object.  So, if we look at the last element in the sequence
	// we should get the name of the hosting POA.  <fingers crossed>
	PortableInterceptor::AdapterName_var poa_path = ri->adapter_name ();
	size_t last_component_index = poa_path->length() - 1;
	
	const char* last_component = poa_path[last_component_index];
	PortableServer::POA_var poa =
	  rootpoa->find_POA (last_component,
			     false); // don't activate
     
	// Yow.  Sometimes the C++ mapping mystifies me in its oddities.
	// One would expect that, since the requestinfo has an ObjectId
	// attribute, and the id_to_reference() operation takes an ObjectId
	// as an "in" argument, that one could simply do
	//   target = poa->id_to_reference (ri->object_id())
	// Alas, 'tis not that simple.  The mapping for a return value for
	// an unbounded sequence is T*, and the mapping for an "in" is
	// const T&, which means that we have to dereference the pointer.
	//
	// Damn, that's really ugly.  But I'm glad that's off my chest.
	target = poa->id_to_reference (*(ri->object_id ()));
      }
#define quote(x) #x
      catch (const PortableServer::POA::ObjectNotActive& e) {
	// thrown by id_to_reference()
	e._tao_print_exception ("ssliop_invocation_interceptor::receive_request @ " quote(__LINE__));
      }
      catch (const PortableServer::POA::WrongPolicy& e) {
	// thrown by id_to_reference()
	e._tao_print_exception ("ssliop_invocation_interceptor::receive_request @ " quote(__LINE__));
      }
      catch (const PortableServer::POA::AdapterNonExistent& e) {
	// thrown by find_POA()
	e._tao_print_exception ("yoyoyo ssliop_invocation_interceptor::receive_request @ " quote(__LINE__));
      }
      catch (const CORBA::ORB::InvalidName& e) {
	// thrown by resolve_initial_references()
	e._tao_print_exception ("ssliop_invocation_interceptor::receive_request @ " quote(__LINE__));
      }
#else
      target = this->poa_current_->get_reference ();
#endif

      char* operation_name = ri->operation ();
      char* target_interface_name = ri->target_most_derived_interface(); // is this the repository ID?

      CORBA::Boolean it_should_happen = false;
      if (ad)
	it_should_happen = ad->access_allowed (cred_list,
							    target.in(),
							    operation_name,
							    target_interface_name);
      else
	it_should_happen = true;  // if no access_decision, then let it all through

      if (! it_should_happen)
	{
	  throw CORBA::NO_PERMISSION ();
	}
    }
  else
#endif
    {
      // PREVIOUS DEFAULT BEHAVIOR

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
	throw CORBA::NO_PERMISSION ();
    }

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
#endif
}

void
TAO::SSLIOP::Server_Invocation_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::SSLIOP::Server_Invocation_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO::SSLIOP::Server_Invocation_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr /* ri */)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
