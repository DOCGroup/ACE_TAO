#include "SSLIOP_Connector.h"
#include "SSLIOP_OwnCredentials.h"
#include "SSLIOP_Profile.h"
#include "SSLIOP_Util.h"
#include "SSLIOP_X509.h"

#include "orbsvcs/orbsvcs/SecurityLevel2C.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Stub.h"
#include "tao/Transport_Connector.h"
#include "tao/Blocked_Connect_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Profile_Transport_Resolver.h"
#include "ace/Auto_Ptr.h"
#include "ace/os_include/os_netdb.h"



ACE_RCSID (SSLIOP,
           SSLIOP_Connector,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Connect_Concurrency_Strategy<TAO::SSLIOP::Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO::SSLIOP::Connection_Handler>;
template class ACE_Strategy_Connector<TAO::SSLIOP::Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO::SSLIOP::Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Connector_Base<TAO::SSLIOP::Connection_Handler>;
template class ACE_Connector<TAO::SSLIOP::Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_NonBlocking_Connect_Handler<TAO::SSLIOP::Connection_Handler>;
template class ACE_Auto_Basic_Ptr<TAO::SSLIOP::Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO::SSLIOP::Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO::SSLIOP::Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO::SSLIOP::Connection_Handler, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO::SSLIOP::Connection_Handler, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector_Base<TAO::SSLIOP::Connection_Handler>
#pragma instantiate ACE_Connector<TAO::SSLIOP::Connection_Handler, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate ACE_NonBlocking_Connect_Handler<TAO::SSLIOP::Connection_Handler>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO::SSLIOP::Connection_Handler>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


TAO::SSLIOP::Connector::Connector (::Security::QOP qop)
  : TAO::IIOP_SSL_Connector (),
    qop_ (qop),
    connect_strategy_ (),
    base_connector_ (),
    handler_state_ ()
{
}

int
TAO::SSLIOP::Connector::open (TAO_ORB_Core *orb_core)
{
  // Since the ACE_Strategy_Connector (and ACE_Connector) cannot
  // handle non-blocking connections with protocols that have more
  // than one handshake, such as SSL, force blocking connections for
  // SSLIOP.  This deficiency will be addressed soon.
  ACE_NEW_RETURN (this->active_connect_strategy_,
                  TAO_Blocked_Connect_Strategy (orb_core),
                  -1);

  if (this->ACE_NESTED_CLASS (TAO, IIOP_SSL_Connector)::open (orb_core) == -1)
    return -1;

  if (TAO::SSLIOP::Util::setup_handler_state (orb_core,
                                              &(this->tcp_properties_),
                                              this->handler_state_) != 0)
      return -1;

  // Our connect creation strategy
  CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       &(this->handler_state_),
                       0 /* Forcibly disable TAO's GIOPlite feature.
                            It introduces a security hole. */),
                  -1);

  // Our activation strategy
  CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);

  ACE_Reactor *r = this->orb_core ()->reactor ();

  return this->base_connector_.open (r,
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     concurrency_strategy);
}

int
TAO::SSLIOP::Connector::close (void)
{
  (void) this->ACE_NESTED_CLASS (TAO, IIOP_SSL_Connector)::close ();

  delete this->base_connector_.creation_strategy ();
  delete this->base_connector_.concurrency_strategy ();
  return this->base_connector_.close ();
}

TAO_Transport *
TAO::SSLIOP::Connector::connect (TAO::Profile_Transport_Resolver *resolver,
                                 TAO_Transport_Descriptor_Interface *desc,
                                 ACE_Time_Value *timeout
                                 ACE_ENV_ARG_DECL)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Connector::connect, ")
                  ACE_TEXT ("looking for SSLIOP connection.\n")));

  TAO_Endpoint *endpoint = desc->endpoint ();

  if (endpoint->tag () != IOP::TAG_INTERNET_IOP)
    return 0;

  TAO_SSLIOP_Endpoint *ssl_endpoint =
    dynamic_cast<TAO_SSLIOP_Endpoint *> (endpoint);

  if (ssl_endpoint == 0)
    return 0;

  // @@ TODO:  The EstablishTrust policy should be evaluated once per
  //           connection, not once per invocation.  This should
  //           improve performance.
  //
  // Check if the user overrode the default establishment of trust
  // policy for the current object.
  CORBA::Policy_var policy =
    resolver->stub ()->get_policy (::Security::SecEstablishTrustPolicy
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  SecurityLevel2::EstablishTrustPolicy_var trust_policy =
    SecurityLevel2::EstablishTrustPolicy::_narrow (policy.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // We use a pointer and temporary to make it obvious to determine
  // if no establishment of trust policy was set.  Specifically, if
  // the "trust" pointer below is zero, then the SSLIOP pluggable
  // protocol default value will be used.
  ::Security::EstablishTrust trust = { 0 , 0 };
  if (!CORBA::is_nil (trust_policy.in ()))
    {
      trust = trust_policy->trust (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  // Flag that states whether any form of establishment of trust
  // should occur.
  CORBA::Boolean establish_trust =
    trust.trust_in_target || trust.trust_in_client;

  // @@ Should this be in a "policy validator?"
  //
  // If the SSL port is zero, then no SSLIOP tagged component was
  // available in the IOR, meaning that there is no way to establish
  // trust.  Throw an exception.
  if (ssl_endpoint->ssl_component ().port == 0
      && establish_trust)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO_SSLIOP (%P|%t) ERROR: ")
                      ACE_TEXT ("Cannot establish trust since ")
                      ACE_TEXT ("no SSLIOP tagged component was ")
                      ACE_TEXT ("found in the IOR.\n")));
        }

      ACE_THROW_RETURN (CORBA::INV_POLICY (),   // @@ Correct exception?
                        0);
    }

  // Check if the user overrode the default Quality-of-Protection for
  // the current object.
  policy = resolver->stub ()->get_policy (::Security::SecQOPPolicy
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  SecurityLevel2::QOPPolicy_var qop_policy =
    SecurityLevel2::QOPPolicy::_narrow (policy.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Temporary variable used to avoid overwriting the default value
  // set when the ORB was initialized.
  ::Security::QOP qop = this->qop_;

  if (!CORBA::is_nil (qop_policy.in ()))
    {
      qop = qop_policy->qop (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  // If the SSL port is zero, then no SSLIOP tagged component was
  // available in the IOR, meaning that there is no way to make a
  // secure invocation.  Throw an exception.
  if (qop != ::Security::SecQOPNoProtection
      && ssl_endpoint->ssl_component ().port == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO_SSLIOP (%P|%t) ERROR: ")
                      ACE_TEXT ("Cannot make secure invocation since ")
                      ACE_TEXT ("no SSLIOP tagged component was ")
                      ACE_TEXT ("found in the IOR.\n")));
        }

      ACE_THROW_RETURN (CORBA::INV_POLICY (),   // @@ Correct exception?
                        0);
    }

  if ((!establish_trust && qop == ::Security::SecQOPNoProtection)
      || ssl_endpoint->ssl_component ().port == 0)
    {
      return this->iiop_connect (ssl_endpoint,
                                 resolver,
                                 timeout
                                 ACE_ENV_ARG_PARAMETER);
    }

  return this->ssliop_connect (ssl_endpoint,
                               qop,
                               trust,
                               resolver,
                               desc,
                               timeout
                               ACE_ENV_ARG_PARAMETER);
}


TAO_Profile *
TAO::SSLIOP::Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_SSLIOP_Profile (this->orb_core ()),
                  0);

  const int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO::SSLIOP::Connector::make_profile (ACE_ENV_SINGLE_ARG_DECL)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_SSLIOP_Profile (this->orb_core (),
                                          0), // SSL component
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return profile;
}

TAO_Transport*
TAO::SSLIOP::Connector::iiop_connect (
  TAO_SSLIOP_Endpoint *ssl_endpoint,
  TAO::Profile_Transport_Resolver *resolver,
  ACE_Time_Value *timeout
  ACE_ENV_ARG_DECL)
{
  const ::SSLIOP::SSL &ssl_component = ssl_endpoint->ssl_component ();

  // Only allow connection to the insecure IIOP port if the endpoint
  // explicitly allows it, i.e. if the Security::NoProtection security
  // association bit is set in the SSLIOP::SSL::target_supports field.
  // The server performs the same permission check, so this check is
  // an optimization since a connection will not be established
  // needlessly, i.e. rejected due to lack of permission.
  //
  // Note that it is still possible for the standard non-SSLIOP aware
  // IIOP pluggable protocol to attempt to connect to the insecure
  // port.  In that case, the server will have to prevent the
  // connection, and subsequently the request, from completing.
  if (ACE_BIT_DISABLED (ssl_component.target_supports,
                        ::Security::NoProtection))
    ACE_THROW_RETURN (CORBA::NO_PERMISSION (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EPERM),
                        CORBA::COMPLETED_NO),
                      0);

  TAO_IIOP_Endpoint *iiop_endpoint = ssl_endpoint->iiop_endpoint ();

  // An IIOP-only transport descriptor must be used instead of the one
  // passed to this method since the latter is used for SSLIOP
  // connections.  Doing so prevents an IIOP-only cached transport
  // from being associated with an SSLIOP connection.
  TAO_Base_Transport_Property iiop_desc (iiop_endpoint);

  // Note that the IIOP-only transport descriptor is used!
  return
    this->ACE_NESTED_CLASS (TAO, IIOP_SSL_Connector)::connect (
      resolver,
      &iiop_desc,
      timeout
      ACE_ENV_ARG_PARAMETER);
}

TAO_Transport *
TAO::SSLIOP::Connector::ssliop_connect (
  TAO_SSLIOP_Endpoint *ssl_endpoint,
  ::Security::QOP qop,
  const ::Security::EstablishTrust &trust,
  TAO::Profile_Transport_Resolver *resolver,
  TAO_Transport_Descriptor_Interface *desc,
  ACE_Time_Value *max_wait_time
  ACE_ENV_ARG_DECL)
{
  const ::SSLIOP::SSL &ssl_component = ssl_endpoint->ssl_component ();

  // @@ The following check for "required insecurity" seems odd, but
  //    I haven't seen anything in the Security spec that says this
  //    policy isn't possible.
  //      -Ossama

  // If the endpoint requires an insecure connection, i.e. the
  // Security::NoProtection security association bit in the
  // SSLIOP::SSL::target_requires field is enabled, then prevent an
  // SSL connection from occuring.
  if (ACE_BIT_ENABLED (ssl_component.target_requires,
                       ::Security::NoProtection))
    ACE_THROW_RETURN (CORBA::NO_PERMISSION (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EPERM),
                        CORBA::COMPLETED_NO),
                      0);

  // If the invocation wants integrity without confidentiality but the
  // server does not support "no protection," then it won't be
  // possible provide integrity.  In order to support integrity
  // without confidentiality, encryption must be disabled but secure
  // hashes must remain enabled.  This is achieved using the "eNULL"
  // cipher.  However, the "eNULL" cipher is only enabled on the
  // server side if "no protection" is enabled.
  if (ACE_BIT_DISABLED (ssl_component.target_supports,
                        ::Security::NoProtection)
      && qop == ::Security::SecQOPIntegrity)
    ACE_THROW_RETURN (CORBA::INV_POLICY (), 0);

  const ACE_INET_Addr &remote_address =
    ssl_endpoint->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized
  // properly.  Failure can occur if hostname lookup failed when
  // initializing the remote ACE_INET_Addr.
  if (remote_address.get_type () != AF_INET)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) SSLIOP connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) This is most likely ")
                      ACE_TEXT ("due to a hostname lookup ")
                      ACE_TEXT ("failure.\n")));
        }

      return 0;
    }

  int result = 0;
  TAO::SSLIOP::Connection_Handler *svc_handler = 0;
  TAO_Transport *transport = 0;

  // Check the Cache first for connections
  if (this->orb_core ()->lane_resources ().transport_cache ().find_transport (
        desc,
        transport) == 0)
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - SSLIOP_Connector::ssliop_connect, ")
                    ACE_TEXT ("got existing transport[%d]\n"),
                    transport->id ()));

      // When the transport is not connected wait for completion
      if (!transport->is_connected())
        {
          if (!this->wait_for_connection_completion (resolver,
                                                     transport,
                                                     max_wait_time))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - SSLIOP_Connector::ssliop_connect,")
                          ACE_TEXT ("wait for completion failed\n")));

            }
        }
    }
  else
    {
      if (TAO_debug_level > 4)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - SSLIOP_Connector::ssliop_connect, ")
                    ACE_TEXT ("making a new connection \n")));

      // Purge connections (if necessary)
      this->orb_core ()->lane_resources ().transport_cache ().purge ();

      // The svc_handler is created beforehand so that we can get
      // access to the underlying ACE_SSL_SOCK_Stream (the peer) and
      // its SSL pointer member prior to descending into the
      // ACE_Strategy_Connector (the "base_connector_").  This is
      // thread-safe and reentrant, hence no synchronization is
      // necessary.
      //
      // The make_svc_handler() method creates the service handler and
      // bumps the #REFCOUNT# up one extra.  The extra reference count
      // in TAO_Connect_Creation_Strategy::make_svc_handler() is
      // needed in the case when connection completion is pending and
      // we are going to wait on a variable in the handler to changes,
      // signifying success or failure.  Note, that this increment
      // cannot be done once the connect() returns since this might be
      // too late if another thread pick up the completion and
      // potentially deletes the handler before we get a chance to
      // increment the reference count.
      if (this->base_connector_.creation_strategy ()->make_svc_handler (
               svc_handler) != 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) Unable to create SSLIOP ")
                        ACE_TEXT ("service handler.\n")));

          return 0;
        }

      ACE_Auto_Basic_Ptr<TAO::SSLIOP::Connection_Handler>
        safe_handler (svc_handler);

      // Setup the establishment of trust connection properties, if
      // any.
      int verify_mode = 0;

      // On the server side, "trust_in_client" requires that a peer
      // (client) certificate exist.  Fail if one doesn't exist.
      //
      // In SSLIOP's case, trust_in_client also implies
      // trust_in_target.
      if (trust.trust_in_client)
        verify_mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;

      // Require verification of the target's certificate.
      else if (trust.trust_in_target)
        verify_mode = SSL_VERIFY_PEER;

      // Trust in neither the client nor the target is explicitly
      // specified.  Use the default setting.
      else
        verify_mode =
          ACE_SSL_Context::instance ()->default_verify_mode ();

      ::SSL_set_verify (svc_handler->peer ().ssl (),
                        verify_mode,
                        0);

      // The "eNULL" cipher disables encryption but still uses a
      // secure hash (e.g. SHA1 or MD5) to ensure integrity.  (Try the
      // command "openssl ciphers -v eNULL".)
      //
      // Note that it is not possible to completely disable protection
      // here.
      if ((qop == ::Security::SecQOPNoProtection
          || qop == ::Security::SecQOPIntegrity)
          && ::SSL_set_cipher_list (svc_handler->peer ().ssl (),
                                    "eNULL") == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Unable to set eNULL ")
                        ACE_TEXT ("SSL cipher.\n")));

          ACE_THROW_RETURN (CORBA::INV_POLICY (), 0);
        }

      TAO::SSLIOP::Credentials_var credentials =
        this->retrieve_credentials (resolver->stub (),
                                    svc_handler->peer ().ssl ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      svc_handler = safe_handler.release ();

      // Get the right synch options
      ACE_Synch_Options synch_options;

      this->active_connect_strategy_->synch_options (max_wait_time,
                                                     synch_options);

      // If we don't need to block for a transport just set the timeout to
      // be zero.
      ACE_Time_Value tmp_zero (ACE_Time_Value::zero);
      if (!resolver->blocked ())
        {
          synch_options.timeout (ACE_Time_Value::zero);
          max_wait_time = &tmp_zero;
        }

      // We obtain the transport in the <svc_handler> variable.  As we
      // know now that the connection is not available in Cache we can
      // make a new connection
      result = this->base_connector_.connect (svc_handler,
                                              remote_address,
                                              synch_options);

      // There are three possibilities from calling connect(): (a)
      // connection succeeds immediately - in this case, the
      // #REFCOUNT# on the handler is two; (b) connection completion
      // is pending - in this case, the #REFCOUNT# on the handler is
      // also two; (c) connection fails immediately - in this case,
      // the #REFCOUNT# on the handler is one since close() gets
      // called on the handler.

      // Make sure that we always do a remove_reference
      ACE_Event_Handler_var svc_handler_auto_ptr (svc_handler);

      transport =
        svc_handler->transport ();

      if (result == -1)
        {
          // No immediate result, wait for completion
          if (errno == EWOULDBLOCK)
            {
              // Try to wait until connection completion. Incase we block, then we
              // get a connected transport or not. In case of non block we get
              // a connected or not connected transport
              if (!this->wait_for_connection_completion (resolver,
                                                         transport,
                                                         max_wait_time))
                {
                  if (TAO_debug_level > 2)
                    ACE_ERROR ((LM_ERROR, "TAO (%P|%t) - SSLIOP_Connector::"
                                          "ssliop_connect, "
                                          "wait for completion failed\n"));
                }
            }
          else
            {
              // Transport is not usable
              transport = 0;
            }
        }

      // In case of errors transport is zero
      if (transport == 0)
        {
          // Give users a clue to the problem.
          if (TAO_debug_level)
            {
              char buffer [MAXHOSTNAMELEN + 6 + 1];
              ssl_endpoint->addr_to_string (buffer,
                                            sizeof (buffer) - 1);
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - SSL connection to ")
                          ACE_TEXT ("<%s:%d> failed (%p)\n"),
                          buffer,
                          remote_address.get_port_number (),
                          ACE_TEXT ("errno")));
            }

          return 0;
        }

      // At this point, the connection has be successfully connected.
      // #REFCOUNT# is one.
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - SSLIOP_Connector::ssliop_connect, "
                    "new SSL connection to port %d on transport[%d]\n",
                    remote_address.get_port_number (),
                    svc_handler->peer ().get_handle ()));

      ssl_endpoint->qop (qop);
      ssl_endpoint->trust (trust);
      ssl_endpoint->credentials (credentials.in ());

      // Add the handler to Cache
      int retval =
        this->orb_core ()->
          lane_resources ().transport_cache ().cache_transport (desc,
                                                                transport);

      // Failure in adding to cache.
      if (retval != 0)
        {
          // Close the handler.
          svc_handler->close ();

          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "TAO (%P|%t) - SLIIOP_Connector::ssliop_connect, "
                          "could not add the new connection to cache\n"));
            }

          return 0;
        }

      if (transport->is_connected () &&
          transport->wait_strategy ()->register_handler () != 0)
        {
          // Registration failures.

          // Purge from the connection cache, if we are not in the cache, this
          // just does nothing.
          (void) transport->purge_entry ();

          // Close the handler.
          (void) transport->close_connection ();

          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "TAO (%P|%t) - SSLIOP_Connector [%d]::ssliop_connect, "
                        "could not register the transport "
                        "in the reactor.\n",
                        transport->id ()));

          return 0;
        }
    }

  return transport;
}

TAO::SSLIOP::OwnCredentials *
TAO::SSLIOP::Connector::retrieve_credentials (TAO_Stub *stub,
                                              SSL *ssl
                                              ACE_ENV_ARG_DECL)
{
  // Check if the user overrode the default invocation credentials.
  CORBA::Policy_var policy =
    stub->get_policy (::SecurityLevel3::ContextEstablishmentPolicyType
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO::SSLIOP::OwnCredentials::_nil ());

  SecurityLevel3::ContextEstablishmentPolicy_var creds_policy =
    SecurityLevel3::ContextEstablishmentPolicy::_narrow (
      policy.in ()
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO::SSLIOP::OwnCredentials::_nil ());

  TAO::SSLIOP::OwnCredentials_var ssliop_credentials;

  // Set the Credentials (X.509 certificates and corresponding private
  // keys) to be used for this invocation.
  if (!CORBA::is_nil (creds_policy.in ()))
    {
      SecurityLevel3::OwnCredentialsList_var creds_list =
        creds_policy->creds_list (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO::SSLIOP::OwnCredentials::_nil ());

      if (creds_list->length () > 0)
        {
          // Assume that we've got an SSLIOP credential.
          SecurityLevel3::Credentials_ptr credentials =
            creds_list[0u];

          ssliop_credentials =
            TAO::SSLIOP::OwnCredentials::_narrow (credentials
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO::SSLIOP::OwnCredentials::_nil ());

          if (!CORBA::is_nil (ssliop_credentials.in ()))
            {
              TAO::SSLIOP::X509_var x509 = ssliop_credentials->x509 ();
              if (::SSL_use_certificate (ssl, x509.in ()) != 1)
                return TAO::SSLIOP::OwnCredentials::_nil ();

              TAO::SSLIOP::EVP_PKEY_var evp = ssliop_credentials->evp ();
              if (evp.in () != 0
                  && ::SSL_use_PrivateKey (ssl, evp.in ()) != 1)
                {
                  // Invalidate the certificate we just set.
                  (void) ::SSL_use_certificate (ssl, 0);
                  return TAO::SSLIOP::OwnCredentials::_nil ();
                }
            }
        }
    }
  else
    {
      // Use the default certificate and private key, i.e. the one set
      // in the SSL_CTX that was used when creating the SSL data
      // structure.
      TAO::SSLIOP::OwnCredentials_ptr & c = ssliop_credentials.out ();
      ACE_NEW_THROW_EX (c,
                        TAO::SSLIOP::OwnCredentials (
                          ::SSL_get_certificate (ssl),
                          ::SSL_get_privatekey (ssl)),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (TAO::SSLIOP::OwnCredentials::_nil ());
    }

  return ssliop_credentials._retn ();
}

int
TAO::SSLIOP::Connector::cancel_svc_handler (
  TAO_Connection_Handler * svc_handler)
{
  TAO::SSLIOP::Connection_Handler* handler=
    dynamic_cast<TAO::SSLIOP::Connection_Handler*>(svc_handler);

  if (handler)
    {
      // Cancel from the connector
      this->base_connector_.cancel (handler);

      return 0;
    }
  else
    {
      return -1;
    }
}
