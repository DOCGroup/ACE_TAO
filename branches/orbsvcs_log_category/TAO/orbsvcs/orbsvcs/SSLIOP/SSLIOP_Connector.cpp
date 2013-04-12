// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/SSLIOP/SSLIOP_Connector.h"
#include "orbsvcs/SSLIOP/SSLIOP_OwnCredentials.h"
#include "orbsvcs/SSLIOP/SSLIOP_Profile.h"
#include "orbsvcs/SSLIOP/SSLIOP_X509.h"

#include "orbsvcs/SecurityLevel2C.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Connector::Connector (::Security::QOP qop)
  : TAO::IIOP_SSL_Connector (),
    qop_ (qop),
    connect_strategy_ (),
    base_connector_ (0)
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

  if (this->TAO::IIOP_SSL_Connector::open (orb_core) == -1)
    return -1;

  // Our connect creation strategy
  CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core),
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
  (void) this->TAO::IIOP_SSL_Connector::close ();

  delete this->base_connector_.creation_strategy ();
  delete this->base_connector_.concurrency_strategy ();
  return this->base_connector_.close ();
}

TAO_Transport *
TAO::SSLIOP::Connector::connect (TAO::Profile_Transport_Resolver *resolver,
                                 TAO_Transport_Descriptor_Interface *desc,
                                 ACE_Time_Value *timeout)
{
  if (TAO_debug_level > 0)
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_SSLIOP (%P|%t) - Connector::connect, ")
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
    resolver->stub ()->get_policy (::Security::SecEstablishTrustPolicy);

  SecurityLevel2::EstablishTrustPolicy_var trust_policy =
    SecurityLevel2::EstablishTrustPolicy::_narrow (policy.in ());

  // We use a pointer and temporary to make it obvious to determine
  // if no establishment of trust policy was set.  Specifically, if
  // the "trust" pointer below is zero, then the SSLIOP pluggable
  // protocol default value will be used.
  ::Security::EstablishTrust trust = { 0 , 0 };
  if (!CORBA::is_nil (trust_policy.in ()))
    {
      trust = trust_policy->trust ();
    }

  // Flag that states whether any form of establishment of trust
  // should occur.
  CORBA::Boolean const establish_trust =
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
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO_SSLIOP (%P|%t) ERROR: ")
                      ACE_TEXT ("Cannot establish trust since ")
                      ACE_TEXT ("no SSLIOP tagged component was ")
                      ACE_TEXT ("found in the IOR.\n")));
        }

      throw CORBA::INV_POLICY ();
    }

  // Check if the user overrode the default Quality-of-Protection for
  // the current object.
  policy = resolver->stub ()->get_policy (::Security::SecQOPPolicy);

  SecurityLevel2::QOPPolicy_var qop_policy =
    SecurityLevel2::QOPPolicy::_narrow (policy.in ());

  // Temporary variable used to avoid overwriting the default value
  // set when the ORB was initialized.
  ::Security::QOP qop = this->qop_;

  if (!CORBA::is_nil (qop_policy.in ()))
    {
      qop = qop_policy->qop ();
    }

  // If the SSL port is zero, then no SSLIOP tagged component was
  // available in the IOR, meaning that there is no way to make a
  // secure invocation.  Throw an exception.
  if (qop != ::Security::SecQOPNoProtection
      && ssl_endpoint->ssl_component ().port == 0)
    {
      if (TAO_debug_level > 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO_SSLIOP (%P|%t) ERROR: ")
                      ACE_TEXT ("Cannot make secure invocation since ")
                      ACE_TEXT ("no SSLIOP tagged component was ")
                      ACE_TEXT ("found in the IOR.\n")));
        }

      throw CORBA::INV_POLICY ();
    }

  if ((!establish_trust && qop == ::Security::SecQOPNoProtection)
      || ssl_endpoint->ssl_component ().port == 0)
    {
      return this->iiop_connect (ssl_endpoint, resolver, timeout);
    }

  return this->ssliop_connect (ssl_endpoint,
                               qop,
                               trust,
                               resolver,
                               desc,
                               timeout);
}


TAO_Profile *
TAO::SSLIOP::Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile = 0;
  ACE_NEW_RETURN (pfile,
                  TAO_SSLIOP_Profile (this->orb_core ()),
                  0);

  if (pfile->decode (cdr) == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO::SSLIOP::Connector::make_profile (void)
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
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return profile;
}


TAO_Profile *
TAO::SSLIOP::Connector::make_secure_profile (void)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_SSLIOP_Profile (this->orb_core (),
                                          1), // SSL component
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return profile;
}



TAO_Profile *
TAO::SSLIOP::Connector::corbaloc_scan (const char *endpoint, size_t &len)
{
   int ssl_only = 0;
   if (this->check_prefix (endpoint) == 0)
   {
       ssl_only = 1;
   }
   else
   {
       if (this->TAO_IIOP_Connector::check_prefix (endpoint) != 0)
         return 0;
   }

   // Determine the (first in a list of possibly > 1) endpoint address
   const char *comma_pos = ACE_OS::strchr (endpoint,',');
   const char *slash_pos = ACE_OS::strchr (endpoint,'/');
   if (comma_pos == 0 && slash_pos == 0)
   {
       if (TAO_debug_level)
       {
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT("(%P|%t) SSLIOP_Connector::corbaloc_scan warning: ")
                        ACE_TEXT("supplied string contains no comma or slash: %s\n"),
                        endpoint));
       }
       len = ACE_OS::strlen (endpoint);
   }
   else if (slash_pos != 0 || comma_pos > slash_pos)
   {
       // The endpoint address does not extend past the first '/' or ','
       len = slash_pos - endpoint;
   }
   else
   {
       len = comma_pos - endpoint;
   }

   //Create the corresponding profile
   TAO_Profile *ptmp = 0;
   if (ssl_only)
     {
       ptmp = this->make_secure_profile ();
     }
   else
     {
       ptmp = this->make_profile ();
     }

   return ptmp;
}


int
TAO::SSLIOP::Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint) return -1;  // Failure

  const char *protocol[] = { "ssliop", "sslioploc" };

  size_t first_slot = ACE_OS::strchr (endpoint, ':') - endpoint;

  size_t len0 = ACE_OS::strlen (protocol[0]);
  size_t len1 = ACE_OS::strlen (protocol[1]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (first_slot == len0 && ACE_OS::strncmp (endpoint, protocol[0], len0) == 0)
    return 0;

  if (first_slot == len1 && ACE_OS::strncmp (endpoint, protocol[1], len1) == 0)
    return 0;

  // Failure: not an SSLIOP IOR
  // DO NOT throw an exception here.
  return -1;
}


TAO_Transport*
TAO::SSLIOP::Connector::iiop_connect (
  TAO_SSLIOP_Endpoint *ssl_endpoint,
  TAO::Profile_Transport_Resolver *resolver,
  ACE_Time_Value *timeout)
{
#if 0
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
    throw CORBA::NO_PERMISSION (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EPERM),
      CORBA::COMPLETED_NO);
#endif

  TAO_IIOP_Endpoint *iiop_endpoint = ssl_endpoint->iiop_endpoint ();

  // An IIOP-only transport descriptor must be used instead of the one
  // passed to this method since the latter is used for SSLIOP
  // connections.  Doing so prevents an IIOP-only cached transport
  // from being associated with an SSLIOP connection.
  TAO_Base_Transport_Property iiop_desc (iiop_endpoint);

  // Note that the IIOP-only transport descriptor is used!
  return
    this->TAO::IIOP_SSL_Connector::connect (resolver, &iiop_desc, timeout);
}

TAO_Transport *
TAO::SSLIOP::Connector::ssliop_connect (
  TAO_SSLIOP_Endpoint *ssl_endpoint,
  ::Security::QOP qop,
  const ::Security::EstablishTrust &trust,
  TAO::Profile_Transport_Resolver *resolver,
  TAO_Transport_Descriptor_Interface *desc,
  ACE_Time_Value *max_wait_time)
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
    throw CORBA::NO_PERMISSION (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EPERM),
      CORBA::COMPLETED_NO);

  // If the invocation wants integrity without confidentiality but the
  // server does not support "no protection," then it won't be
  // possible to provide integrity.  In order to support integrity
  // without confidentiality, encryption must be disabled but secure
  // hashes must remain enabled.  This is achieved using the "eNULL"
  // cipher.  However, the "eNULL" cipher is only enabled on the
  // server side if "no protection" is enabled.
  if (ACE_BIT_DISABLED (ssl_component.target_supports,
                        ::Security::NoProtection)
      && qop == ::Security::SecQOPIntegrity)
    throw CORBA::INV_POLICY ();

  const ACE_INET_Addr &remote_address = ssl_endpoint->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized
  // properly.  Failure can occur if hostname lookup failed when
  // initializing the remote ACE_INET_Addr.
  if (remote_address.get_type () != AF_INET
#if defined (ACE_HAS_IPV6)
      && remote_address.get_type () != AF_INET6
#endif /* ACE_HAS_IPV6 */
      )
    {
      if (TAO_debug_level > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
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

  // Before we can check the cache to find an existing connection, we
  // need to make sure the ssl_endpoint is fully initialized with the
  // local security information. This endpoint initalized by the
  // profile does not (and cannot) contain the desired QOP, trust, or
  // credential information which is necesary to uniquely identify
  // this connection.
  if (!ssl_endpoint->credentials_set ())
    {
      if (TAO_debug_level > 2)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) Initializing SSLIOP_Endpoint\n")
                    ));

      if (this->base_connector_.creation_strategy ()->make_svc_handler (
               svc_handler) != 0)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) Unable to create SSLIOP ")
                        ACE_TEXT ("service handler.\n")));

          return 0;
        }

      ACE_Event_Handler_var
        safe_handler (svc_handler);
      TAO::SSLIOP::OwnCredentials_var credentials =
        this->retrieve_credentials (resolver->stub (),
                                    svc_handler->peer ().ssl ());

      ssl_endpoint->set_sec_attrs (qop, trust, credentials.in());

      safe_handler.release ();
    }

  // Check the Cache first for connections
  size_t busy_count = 0;
  TAO::Transport_Cache_Manager::Find_Result found =
    this->orb_core ()->lane_resources ().transport_cache ().find_transport (
        desc,
        transport,
        busy_count);

  if (found == TAO::Transport_Cache_Manager::CACHE_FOUND_AVAILABLE)
    {
      // ...eliminate svc_handle memory leak...
      ACE_Event_Handler_var
        safe_handler (svc_handler);

      if (TAO_debug_level > 2)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - SSLIOP_Connector::ssliop_connect, ")
                    ACE_TEXT ("got existing transport[%d]\n"),
                    transport->id ()));

      // When the transport is not connected wait for completion
      if (!transport->is_connected())
        {
          if (!this->wait_for_connection_completion (resolver,
                                                     *desc,
                                                     transport,
                                                     max_wait_time))
            {
              ORBSVCS_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - SSLIOP_Connector::ssliop_connect,")
                          ACE_TEXT ("wait for completion failed\n")));

            }
        }
    }
  else
    {

      bool make_new_connection =
        (found == TAO::Transport_Cache_Manager::CACHE_FOUND_NONE) ||
        (found == TAO::Transport_Cache_Manager::CACHE_FOUND_BUSY
            && this->new_connection_is_ok (busy_count));

      if (make_new_connection)
        {

          if (TAO_debug_level > 4)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - SSLIOP_Connector::ssliop_connect, ")
                        ACE_TEXT ("making a new connection\n")));

          // Purge connections (if necessary)
          this->orb_core ()->lane_resources ().transport_cache ().purge ();

          // The svc_handler is created beforehand so that we can get
          // access to the underlying ACE_SSL_SOCK_Stream (the peer) and
          // its SSL pointer member prior to descending into the
          // ACE_Strategy_Connector (the "base_connector_").  This is
          // thread-safe and reentrant, hence no synchronization is
          // necessary.
          if (svc_handler == 0 &&
              this->base_connector_.creation_strategy ()->make_svc_handler (
                   svc_handler) != 0)
            {
              if (TAO_debug_level > 0)
                ORBSVCS_DEBUG ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) Unable to create SSLIOP ")
                            ACE_TEXT ("service handler.\n")));

              return 0;
            }

          ACE_Event_Handler_var
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
            verify_mode = ACE_SSL_Context::instance ()->default_verify_mode ();

          ::SSL_set_verify (svc_handler->peer ().ssl (), verify_mode, 0);

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
                ORBSVCS_DEBUG ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) Unable to set eNULL ")
                            ACE_TEXT ("SSL cipher.\n")));

              throw CORBA::INV_POLICY ();
            }

          // svc_handler is never reset..it still has the value
          (void)safe_handler.release ();

          // Get the right synch options
          ACE_Synch_Options synch_options;

          this->active_connect_strategy_->synch_options (max_wait_time,
                                                         synch_options);

          // The code used to set the timeout to zero, with the intent of
          // polling the reactor for connection completion. However, the side-effect
          // was to cause the connection to timeout immediately.

          // We obtain the transport in the <svc_handler> variable.  As we
          // know now that the connection is not available in Cache we can
          // make a new connection
          result = this->base_connector_.connect (svc_handler,
                                                  remote_address,
                                                  synch_options);

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
                                                             *desc,
                                                             transport,
                                                             max_wait_time))
                    {
                      if (TAO_debug_level > 2)
                        ORBSVCS_ERROR ((LM_ERROR, "TAO (%P|%t) - SSLIOP_Connector::"
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
                  ORBSVCS_DEBUG ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - SSL connection to ")
                              ACE_TEXT ("<%s:%d> failed (%p)\n"),
                              buffer,
                              remote_address.get_port_number (),
                              ACE_TEXT ("errno")));
                }

              return 0;
            }

          // fix for bug 2654
          if (svc_handler->keep_waiting ())
            {
              svc_handler->connection_pending ();
            }

          // fix for bug 2654
          if (svc_handler->error_detected ())
            {
              svc_handler->cancel_pending_connection ();
            }

          // At this point, the connection has be successfully connected.
          // #REFCOUNT# is one.
          if (TAO_debug_level > 2)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - SSLIOP_Connector::ssliop_connect, "
                        "new SSL connection to port %d on transport[%d]\n",
                        remote_address.get_port_number (),
                        svc_handler->peer ().get_handle ()));

          // Add the handler to Cache
          int retval =
            this->orb_core ()->
              lane_resources ().transport_cache ().cache_transport (desc,
                                                                    transport);

          // Failure in adding to cache.
        if (retval == -1)
            {
              // Close the handler.
              svc_handler->close ();

              if (TAO_debug_level > 0)
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              "TAO (%P|%t) - SLIIOP_Connector::ssliop_connect, "
                              "could not add the new connection to cache\n"));
                }

              return 0;
            }

          // fix for bug 2654
          if (svc_handler->error_detected ())
            {
              svc_handler->cancel_pending_connection ();
              transport->purge_entry();
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
                ORBSVCS_ERROR ((LM_ERROR,
                            "TAO (%P|%t) - SSLIOP_Connector [%d]::ssliop_connect, "
                            "could not register the transport "
                            "in the reactor.\n",
                            transport->id ()));

              return 0;
            }

          svc_handler_auto_ptr.release ();

        }
      else // not making new connection
        {
          (void) this->wait_for_transport (resolver, transport, max_wait_time, true);
        }
    }

  return transport;
}

TAO::SSLIOP::OwnCredentials *
TAO::SSLIOP::Connector::retrieve_credentials (TAO_Stub *stub,
                                              SSL *ssl)
{
  // Check if the user overrode the default invocation credentials.
  CORBA::Policy_var policy =
    stub->get_policy (::SecurityLevel3::ContextEstablishmentPolicyType);

  SecurityLevel3::ContextEstablishmentPolicy_var creds_policy =
    SecurityLevel3::ContextEstablishmentPolicy::_narrow (
      policy.in ());

  TAO::SSLIOP::OwnCredentials_var ssliop_credentials;

  // Set the Credentials (X.509 certificates and corresponding private
  // keys) to be used for this invocation.
  if (!CORBA::is_nil (creds_policy.in ()))
    {
      SecurityLevel3::OwnCredentialsList_var creds_list =
        creds_policy->creds_list ();

      if (creds_list->length () > 0)
        {
          // Assume that we've got an SSLIOP credential.
          SecurityLevel3::Credentials_ptr credentials =
            creds_list[0u];

          ssliop_credentials =
            TAO::SSLIOP::OwnCredentials::_narrow (credentials);

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

      /**
       * @todo Check if the CredentialsCurator contains a default set
       *       of SSLIOP OwnCredentials.
       */

      TAO::SSLIOP::OwnCredentials_ptr & c = ssliop_credentials.out ();
      ACE_NEW_THROW_EX (c,
                        TAO::SSLIOP::OwnCredentials (
                          ::SSL_get_certificate (ssl),
                          ::SSL_get_privatekey (ssl)),
                        CORBA::NO_MEMORY ());
    }

  return ssliop_credentials._retn ();
}

int
TAO::SSLIOP::Connector::cancel_svc_handler (
  TAO_Connection_Handler * svc_handler)
{
  TAO::SSLIOP::Connection_Handler* handler=
    dynamic_cast<TAO::SSLIOP::Connection_Handler*> (svc_handler);

  if (handler)
    // Cancel from the connector
    return this->base_connector_.cancel (handler);

  return -1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
