// -*- C++ -*-
//
// $Id$

#include "SSLIOP_Connector.h"
#include "SSLIOP_Profile.h"
#include "SSLIOP_Util.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Invocation.h"
#include "tao/Thread_Lane_Resources.h"

#include "ace/Auto_Ptr.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Connector,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


template class TAO_Connect_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_SSLIOP_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Connector<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>;


template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>*>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;
template class ACE_Auto_Basic_Array_Ptr<TAO_SSLIOP_Connection_Handler*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_SSLIOP_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>

#pragma instantiate ACE_Connector<TAO_SSLIOP_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_SSLIOP_Connection_Handler>
#pragma instantiate ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>
#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_SSLIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<TAO_SSLIOP_Connection_Handler*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


TAO_SSLIOP_Connector::TAO_SSLIOP_Connector (Security::QOP qop)
  : TAO_IIOP_SSL_Connector (),
    qop_ (qop),
    connect_strategy_ (),
    base_connector_ (),
    handler_state_ ()
{
}

int
TAO_SSLIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  if (this->TAO_IIOP_SSL_Connector::open (orb_core) == -1)
    return -1;

  if (TAO_SSLIOP_Util::setup_handler_state (orb_core,
                                            &(this->tcp_properties_),
                                            this->handler_state_) != 0)
      return -1;

  // Our connect creation strategy
  TAO_SSLIOP_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_SSLIOP_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       &(this->handler_state_),
                       this->lite_flag_),
                  -1);

  // Our activation strategy
  TAO_SSLIOP_CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  TAO_SSLIOP_CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);

  ACE_Reactor *r = this->orb_core ()->reactor ();

  return this->base_connector_.open (r,
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     concurrency_strategy);
}

int
TAO_SSLIOP_Connector::close (void)
{
  (void) this->TAO_IIOP_SSL_Connector::close ();

  delete this->base_connector_.creation_strategy ();
  delete this->base_connector_.concurrency_strategy ();
  return this->base_connector_.close ();
}

int
TAO_SSLIOP_Connector::connect (TAO_GIOP_Invocation *invocation,
                               TAO_Transport_Descriptor_Interface *desc
                               TAO_ENV_ARG_DECL)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for SSLIOP connection.\n")));

  TAO_Endpoint *endpoint = desc->endpoint ();

  if (endpoint->tag () != IOP::TAG_INTERNET_IOP)
    return -1;

  TAO_SSLIOP_Endpoint *ssl_endpoint =
    ACE_dynamic_cast (TAO_SSLIOP_Endpoint *,
                      endpoint);

  if (ssl_endpoint == 0)
    return -1;

  // @@ TODO:  The EstablishTrust policy should be evaluated once per
  //           connection, not once per invocation.  This should
  //           improve performance.
  //
  // Check if the user overrode the default establishment of trust
  // policy for the current object.
  CORBA::Policy_var policy =
    invocation->stub ()->get_policy (Security::SecEstablishTrustPolicy
                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  SecurityLevel2::EstablishTrustPolicy_var trust_policy =
    SecurityLevel2::EstablishTrustPolicy::_narrow (policy.in ()
                                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We use a pointer and temporary to make it obvious to determine
  // if no establishment of trust policy was set.  Specifically, if
  // the "trust" pointer below is zero, then the SSLIOP pluggable
  // protocol default value will be used.
  Security::EstablishTrust trust = { 0 , 0 };
  if (!CORBA::is_nil (trust_policy.in ()))
    {
      trust = trust_policy->trust (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
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
                        -1);
    }

  // Check if the user overrode the default Quality-of-Protection for
  // the current object.
  policy = invocation->stub ()->get_policy (Security::SecQOPPolicy
                                            TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  SecurityLevel2::QOPPolicy_var qop_policy =
    SecurityLevel2::QOPPolicy::_narrow (policy.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Temporary variable used to avoid overwriting the default value
  // set when the ORB was initialized.
  Security::QOP qop = this->qop_;

  if (!CORBA::is_nil (qop_policy.in ()))
    {
      qop = qop_policy->qop (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  // If the SSL port is zero, then no SSLIOP tagged component was
  // available in the IOR, meaning that there is no way to make a
  // secure invocation.  Throw an exception.
  if (qop != Security::SecQOPNoProtection
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
                        -1);
    }

  if ((!establish_trust && qop == Security::SecQOPNoProtection)
      || ssl_endpoint->ssl_component ().port == 0)
    {
      return this->iiop_connect (ssl_endpoint,
                                 invocation
                                 TAO_ENV_ARG_PARAMETER);
    }

  return this->ssliop_connect (ssl_endpoint,
                               qop,
                               trust,
                               invocation,
                               desc
                               TAO_ENV_ARG_PARAMETER);
}


TAO_Profile *
TAO_SSLIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_SSLIOP_Profile (this->orb_core ()),
                  0);

  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO_SSLIOP_Connector::make_profile (TAO_ENV_SINGLE_ARG_DECL)
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

int
TAO_SSLIOP_Connector::iiop_connect (TAO_SSLIOP_Endpoint *ssl_endpoint,
                                    TAO_GIOP_Invocation *invocation
                                    TAO_ENV_ARG_DECL)
{
  const SSLIOP::SSL &ssl_component = ssl_endpoint->ssl_component ();

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
                        Security::NoProtection))
    ACE_THROW_RETURN (CORBA::NO_PERMISSION (
                        CORBA_SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EPERM),
                        CORBA::COMPLETED_NO),
                      -1);

  TAO_IIOP_Endpoint *iiop_endpoint = ssl_endpoint->iiop_endpoint ();

  // An IIOP-only transport descriptor must be used instead of the one
  // passed to this method since the latter is used for SSLIOP
  // connections.  Doing so prevents an IIOP-only cached transport
  // from being associated with an SSLIOP connection.
  TAO_Base_Transport_Property iiop_desc (iiop_endpoint);

  // Note that the IIOP-only transport descriptor is used!
  return this->TAO_IIOP_SSL_Connector::connect (invocation,
                                                &iiop_desc
                                                TAO_ENV_ARG_PARAMETER);
}

int
TAO_SSLIOP_Connector::ssliop_connect (TAO_SSLIOP_Endpoint *ssl_endpoint,
                                      Security::QOP qop,
                                      const Security::EstablishTrust &trust,
                                      TAO_GIOP_Invocation *invocation,
                                      TAO_Transport_Descriptor_Interface *desc
                                      TAO_ENV_ARG_DECL)
{
  TAO_Transport *&transport = invocation->transport ();
  ACE_Time_Value *max_wait_time = invocation->max_wait_time ();

  const SSLIOP::SSL &ssl_component = ssl_endpoint->ssl_component ();

  // @@ The following check for "required insecurity" seems odd, but
  //    I haven't seen anything in the Security spec that says this
  //    policy isn't possible.
  //      -Ossama

  // If the endpoint requires an insecure connection, i.e. the
  // Security::NoProtection security association bit in the
  // SSLIOP::SSL::target_requires field is enabled, then prevent an
  // SSL connection from occuring.
  if (ACE_BIT_ENABLED (ssl_component.target_requires,
                       Security::NoProtection))
    ACE_THROW_RETURN (CORBA::NO_PERMISSION (
                        CORBA_SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EPERM),
                        CORBA::COMPLETED_NO),
                      -1);

  // If the invocation wants integrity without confidentiality but the
  // server does not support "no protection," then it won't be
  // possible provide integrity.  In order to support integrity
  // without confidentiality, encryption must be disabled but secure
  // hashes must remain enabled.  This is achieved using the "eNULL"
  // cipher.  However, the "eNULL" cipher is only enabled on the
  // server side if "no protection" is enabled.
  if (ACE_BIT_DISABLED (ssl_component.target_supports,
                        Security::NoProtection)
      && qop == Security::SecQOPIntegrity)
    ACE_THROW_RETURN (CORBA::INV_POLICY (), -1);

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

      return -1;
    }

  int result = 0;
  TAO_SSLIOP_Connection_Handler *svc_handler = 0;
  TAO_Transport *base_transport = 0;

  // Check the Cache first for connections
  if (this->orb_core ()->lane_resources ().transport_cache ().find_transport (
        desc,
        base_transport) == 0)
    {
      if (TAO_debug_level > 5)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SSLIOP_Connector::connect ")
                    ACE_TEXT ("got an existing transport with ID %d\n"),
                    base_transport->id ()));
    }
  else
    {
      if (TAO_debug_level > 4)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SSLIOP_Connector::connect ")
                    ACE_TEXT ("making a new connection \n")));

      // Purge connections (if necessary)
      this->orb_core ()->lane_resources ().transport_cache ().purge ();

      // The svc_handler is created beforehand so that we can get
      // access to the underlying ACE_SSL_SOCK_Stream (the peer) and
      // its SSL pointer member prior to descending into the
      // ACE_Strategy_Connector (the "base_connector_").  This is
      // thread-safe and reentrant, hence no synchronization is
      // necessary.
      if (this->base_connector_.creation_strategy ()->make_svc_handler (
               svc_handler) != 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Unable to create SSLIOP ")
                        ACE_TEXT ("service handler.\n")));

          return -1;
        }
        

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

      // Trust in neither the client nor the target is required.
      else
        verify_mode = SSL_VERIFY_NONE;

      ::SSL_set_verify (svc_handler->peer ().ssl (),
                        verify_mode,
                        0);

      // The "eNULL" cipher disables encryption but still uses a
      // secure hash (e.g. SHA1 or MD5) to ensure integrity.  (Try the
      // command "openssl ciphers -v eNULL".)
      //
      // Note that it is not possible to completely disable protection
      // here.
      if (qop == Security::SecQOPNoProtection
          || qop == Security::SecQOPIntegrity
          && ::SSL_set_cipher_list (svc_handler->peer ().ssl (),
                                    "eNULL") == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Unable to set eNULL ")
                        ACE_TEXT ("SSL cipher.\n")));

          ACE_THROW_RETURN (CORBA::INV_POLICY (), -1);
        }

      // @@ This needs to change in the next round when we implement
      //    a policy that will not allow new connections when a
      //    connection is busy.
      if (max_wait_time != 0)
        {
          ACE_Synch_Options synch_options (ACE_Synch_Options::USE_TIMEOUT,
                                           *max_wait_time);

          // We obtain the transport in the <svc_handler> variable.
          // As we know now that the connection is not available in
          // Cache we can make a new connection
          result = this->base_connector_.connect (svc_handler,
                                                  remote_address,
                                                  synch_options);
        }
      else
        {
          // We obtain the transport in the <svc_handler> variable.
          // As we know now that the connection is not available in
          // Cache we can make a new connection
          result = this->base_connector_.connect (svc_handler,
                                                  remote_address);
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SSLIOP_Connector::connect -  ")
                    ACE_TEXT ("The result is <%d> \n"), result));

      if (result == -1)
        {
          if (TAO_debug_level > 0)
            {
              char buffer [MAXHOSTNAMELEN + 6 + 1];
              ssl_endpoint->addr_to_string (buffer,
                                            sizeof (buffer) - 1);
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %N:%l, connection to ")
                          ACE_TEXT ("%s, SSL port %d failed (%p)\n"),
                          buffer,
                          remote_address.get_port_number (),
                          ACE_TEXT ("errno")));
            }

          return -1;
        }

      base_transport = TAO_Transport::_duplicate (svc_handler->transport ());

      // Add the handler to Cache
      int retval =
        this->orb_core ()->lane_resources ().transport_cache ().cache_transport (
          desc,
          base_transport);

      if (retval != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) SSLIOP_Connector::connect ")
                      ACE_TEXT ("could not add the new connection to ")
                      ACE_TEXT ("Cache.\n")));
        }
    }

  // No need to _duplicate and release since base_transport
  // is going out of scope.  transport now has control of base_transport.
  transport = base_transport;

  return 0;
}
