// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "ace/config-all.h"

#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1

#include "SSLIOP_Connector.h"
#include "SSLIOP_Profile.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(TAO_SSLIOP, SSLIOP_Connector, "$Id$")

TAO_SSLIOP_Connect_Creation_Strategy::
  TAO_SSLIOP_Connect_Creation_Strategy (ACE_Thread_Manager* t,
                                        TAO_ORB_Core *orb_core,
                                        void *arg)
    :  ACE_Creation_Strategy<TAO_SSLIOP_Client_Connection_Handler> (t),
       orb_core_ (orb_core),
       arg_ (arg)
{
}

int
TAO_SSLIOP_Connect_Creation_Strategy::make_svc_handler
  (TAO_SSLIOP_Client_Connection_Handler *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh,
                    TAO_SSLIOP_Client_Connection_Handler
                    (this->orb_core_->thr_mgr (),
                     this->orb_core_,
                     0,
                     this->arg_),
                    -1);
  return 0;
}

// ****************************************************************

#if !defined (TAO_USES_ROBUST_CONNECTION_MGMT)
typedef ACE_Cached_Connect_Strategy<TAO_SSLIOP_Client_Connection_Handler,
                                    ACE_SSL_SOCK_CONNECTOR,
                                    TAO_Cached_Connector_Lock>
        TAO_CACHED_CONNECT_STRATEGY;
#endif /* ! TAO_USES_ROBUST_CONNECTION_MGMT */

TAO_SSLIOP_Connector::TAO_SSLIOP_Connector (int no_protection)
  : TAO_IIOP_Connector (),
    no_protection_ (no_protection),
    base_connector_ ()
#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
    ,
    cached_connect_strategy_ (0),
    caching_strategy_ (0)
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */
{
}

int
TAO_SSLIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  if (this->TAO_IIOP_Connector::open (orb_core) == -1)
    return -1;

  TAO_SSLIOP_Connect_Creation_Strategy *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_SSLIOP_Connect_Creation_Strategy
                  (this->orb_core_->thr_mgr (),
                   this->orb_core_,
                   &(this->tcp_properties_)),
                  -1);

  auto_ptr<TAO_SSLIOP_Connect_Creation_Strategy>
    new_connect_creation_strategy (connect_creation_strategy);

  TAO_Cached_Connector_Lock *connector_lock = 0;
  ACE_NEW_RETURN (connector_lock,
                  TAO_Cached_Connector_Lock (this->orb_core_),
                  -1);

  auto_ptr<TAO_Cached_Connector_Lock> new_connector_lock (connector_lock);

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  ACE_NEW_RETURN (this->cached_connect_strategy_,
                  TAO_CACHED_CONNECT_STRATEGY (*this->caching_strategy_,
                                               new_connect_creation_strategy.get (),
                                               0,
                                               0,
                                               new_connector_lock.get (),
                                               1),
                  -1);
#else /* TAO_USES_ROBUST_CONNECTION_MGMT */
  TAO_CACHED_CONNECT_STRATEGY *cached_connect_strategy = 0;
  ACE_NEW_RETURN (cached_connect_strategy,
                  TAO_CACHED_CONNECT_STRATEGY
                  (new_connect_creation_strategy.get (),
                   0,
                   0,
                   new_connector_lock.get (),
                   1),
                  -1);
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

  // Finally everything is fine.  Make sure to take ownership away
  // from the auto pointer.
  connect_creation_strategy =
    new_connect_creation_strategy.release ();
  connector_lock =
    new_connector_lock.release ();

#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  return this->base_connector_.open (this->orb_core_->reactor (),
                                     &this->null_creation_strategy_,
                                     this->cached_connect_strategy_,
                                     &this->null_activation_strategy_);
#else /* TAO_USES_ROBUST_CONNECTION_MGMT */
  return this->base_connector_.open (this->orb_core_->reactor (),
                                     &this->null_creation_strategy_,
                                     cached_connect_strategy,
                                     &this->null_activation_strategy_);
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */
}

int
TAO_SSLIOP_Connector::close (void)
{
  this->base_connector_.close ();

  // Zap the creation strategy that we created earlier
#if defined (TAO_USES_ROBUST_CONNECTION_MGMT)
  delete this->cached_connect_strategy_->creation_strategy ();
  delete this->cached_connect_strategy_;
  delete this->caching_strategy_;
#else /* TAO_USES_ROBUST_CONNECTION_MGMT */
  TAO_CACHED_CONNECT_STRATEGY *cached_connect_strategy =
    ACE_dynamic_cast (TAO_CACHED_CONNECT_STRATEGY *,
                      this->base_connector_.connect_strategy ());

  delete cached_connect_strategy->creation_strategy ();
  delete cached_connect_strategy;
#endif /* TAO_USES_ROBUST_CONNECTION_MGMT */

  return 0;
}

int
TAO_SSLIOP_Connector::connect (TAO_Endpoint *endpoint,
                               TAO_Transport *&transport,
                               ACE_Time_Value *max_wait_time,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for SSLIOP connection.\n")));

  if (endpoint->tag () != TAO_TAG_IIOP_PROFILE)
    return -1;

  TAO_SSLIOP_Endpoint *ssl_endpoint =
    ACE_dynamic_cast (TAO_SSLIOP_Endpoint *,
                      endpoint);
  if (endpoint == 0)
    return -1;

  const SSLIOP::SSL &ssl_component = ssl_endpoint->ssl_component ();

  // @@ Use the policies to decide if SSL is the right protocol...
  if (this->no_protection_)
    {
      // Only allow connection to the insecure IIOP port if the
      // endpoint explicitly allows it, i.e. if the
      // Security::NoProtection security association bit is set in the
      // SSLIOP::SSL::target_supports field.  The server performs the
      // same permission check, so this check is an optimization since
      // a connection will not be established needlessly,
      // i.e. rejected due to lack of permission.
      //
      // Note that it is still possible for the standard non-SSLIOP
      // aware IIOP pluggable protocol to attempt to connect to the
      // insecure port.  In that case, the server will have to prevent
      // the connection, and subsequently the request, from
      // completing.
      if (ACE_BIT_DISABLED (ssl_component.target_supports,
                            Security::NoProtection))
        ACE_THROW_RETURN (CORBA::NO_PERMISSION (
                            CORBA_SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              EPERM),
                            CORBA::COMPLETED_NO),
                          -1);

      return this->TAO_IIOP_Connector::connect (
                     ssl_endpoint->iiop_endpoint (),
                     transport,
                     max_wait_time,
                     ACE_TRY_ENV);
    }

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

  ACE_INET_Addr remote_address =
    ssl_endpoint->iiop_endpoint ()->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized properly.
  // Failure can occur if hostname lookup failed when initializing the
  // remote ACE_INET_Addr.
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

  remote_address.set_port_number (ssl_component.port);

  TAO_SSLIOP_Client_Connection_Handler *svc_handler = 0;
  int result = 0;

  if (max_wait_time != 0)
    {
      ACE_Synch_Options synch_options (ACE_Synch_Options::USE_TIMEOUT,
                                       *max_wait_time);

      // The connect call will set the hint () stored in the Endpoint
      // object; but we obtain the transport in the <svc_handler>
      // variable. Other threads may modify the hint, but we are not
      // affected.
      result = this->base_connector_.connect (ssl_endpoint->ssl_hint (),
                                              svc_handler,
                                              remote_address,
                                              synch_options);
    }
  else
    {
      // The connect call will set the hint () stored in the Endpoint
      // object; but we obtain the transport in the <svc_handler>
      // variable. Other threads may modify the hint, but we are not
      // affected.
      result = this->base_connector_.connect (ssl_endpoint->ssl_hint (),
                                              svc_handler,
                                              remote_address);
    }

  if (result == -1)
    {
      // Give users a clue to the problem.
      if (TAO_orbdebug)
        {
          char buffer [MAXHOSTNAMELEN + 6 + 1];
          ssl_endpoint->addr_to_string (buffer,
                                    sizeof (buffer) - 1);
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %s:%u, connection to ")
                      ACE_TEXT ("%s, SSL port %d failed (%p)\n"),
                      __FILE__,
                      __LINE__,
                      buffer,
                      remote_address.get_port_number (),
                      ACE_TEXT ("errno")));
        }
      return -1;
    }

  transport = svc_handler->transport ();

  return 0;
}

TAO_Profile *
TAO_SSLIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_SSLIOP_Profile (this->orb_core_),
                  0);

  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

void
TAO_SSLIOP_Connector::make_profile (const char *endpoint,
                                  TAO_Profile *&profile,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  ACE_NEW_THROW_EX (profile,
                    TAO_SSLIOP_Profile (endpoint,
                                        this->orb_core_,
                                        0, // @@ ssl_port
                                        ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  ACE_CHECK;
}

#endif  /* ACE_HAS_SSL */
