// This may look like C, but it's really -*- C++ -*-
//
// $Id$


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

TAO_SSLIOP_Connector::TAO_SSLIOP_Connector (int no_protection)
  : TAO_IIOP_SSL_Connector (),
    no_protection_ (no_protection),
    null_activation_strategy_ (),
    connect_strategy_ (),
    base_connector_ ()
{
}

int
TAO_SSLIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  if (this->TAO_IIOP_SSL_Connector::open (orb_core) == -1)
  return -1;

  TAO_SSLIOP_Connect_Creation_Strategy *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_SSLIOP_Connect_Creation_Strategy
                  (this->orb_core ()->thr_mgr (),
                   this->orb_core (),
                   &(this->tcp_properties_)),
                  -1);

  return this->base_connector_.open (this->orb_core ()->reactor (),
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     &this->null_activation_strategy_);
}

int
TAO_SSLIOP_Connector::close (void)
{
  (void) this->TAO_IIOP_SSL_Connector::close ();

  delete this->base_connector_.creation_strategy ();
  return this->base_connector_.close ();
}

int
TAO_SSLIOP_Connector::connect (TAO_Connection_Descriptor_Interface *desc,
                               TAO_Transport *&transport,
                               ACE_Time_Value *max_wait_time,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for SSLIOP connection.\n")));

  TAO_Endpoint *endpoint = desc->endpoint ();

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

      return this->TAO_IIOP_SSL_Connector::connect (
                     desc,
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

  int result = 0;
  TAO_SSLIOP_Client_Connection_Handler *svc_handler = 0;
  TAO_Connection_Handler *conn_handler = 0;

  // Check the Cache first for connections
  if (this->orb_core ()->connection_cache ().find_handler (desc,
                                                           conn_handler) == 0)
    {
      if (TAO_debug_level > 5)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SSLIOP_Connector::connect ")
                    ACE_TEXT ("got an existing connection \n")));

      // We have found a connection and a handler
      svc_handler =
        ACE_dynamic_cast (TAO_SSLIOP_Client_Connection_Handler *,
                          conn_handler);
    }
  else
    {
      if (TAO_debug_level > 4)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SSLIOP_Connector::connect ")
                    ACE_TEXT ("making a new connection \n")));

      // @@ This needs to change in the next round when we implement a
      // policy that will not allow new connections when a connection
      // is busy.
      if (max_wait_time != 0)
        {
          ACE_Synch_Options synch_options (ACE_Synch_Options::USE_TIMEOUT,
                                           *max_wait_time);

          // We obtain the transport in the <svc_handler> variable. As
          // we know now that the connection is not available in Cache
          // we can make a new connection
          result = this->base_connector_.connect (svc_handler,
                                                  remote_address,
                                                  synch_options);
        }
      else
        {
          // We obtain the transport in the <svc_handler> variable. As
          // we know now that the connection is not available in Cache
          // we can make a new connection
          result = this->base_connector_.connect (svc_handler,
                                                  remote_address);
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SSLIOP_Connector::connect ")
                    ACE_TEXT ("The result is <%d> \n"), result));

      if (result == -1)
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

          return -1;
        }


      // Add the handler to Cache
      int retval =
        this->orb_core ()->connection_cache ().cache_handler (desc,
                                                              svc_handler);

      if (retval != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) SSLIOP_Connector::connect ")
                      ACE_TEXT ("could not add the new connection to Cache \n")));
        }
    }

  transport = svc_handler->transport ();

  return 0;
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
                                        this->orb_core (),
                                        0, // @@ ssl_port
                                        ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  ACE_CHECK;
}



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_NOOP_Concurrency_Strategy<TAO_SSLIOP_Client_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_SSLIOP_Client_Connection_Handler>;
template class ACE_Creation_Strategy<TAO_SSLIOP_Client_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_SSLIOP_Client_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_SSLIOP_Client_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Connector<TAO_SSLIOP_Client_Connection_Handler, ACE_SSL_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>;

template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>*>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Auto_Basic_Array_Ptr<TAO_SSLIOP_Client_Connection_Handler*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_NOOP_Concurrency_Strategy<TAO_SSLIOP_Client_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_SSLIOP_Client_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_SSLIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_SSLIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>

#pragma instantiate ACE_Connector<TAO_SSLIOP_Client_Connection_Handler, ACE_SOCK_Connector>
#pragma instantiate ACE_Creation_Strategy<TAO_SSLIOP_Client_Connection_Handler>
#pragma instantiate ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>
#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_SSLIOP_Client_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<TAO_SSLIOP_Client_Connection_Handler*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
