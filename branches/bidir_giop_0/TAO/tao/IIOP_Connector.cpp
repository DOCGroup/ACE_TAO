// This may look like C, but it's really -*- C++ -*-
// $Id$



#include "tao/IIOP_Connector.h"
#include "tao/IIOP_Profile.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"
#include "ace/Auto_Ptr.h"
#include "tao/RT_Policy_i.h"
#include "tao/Base_Connection_Property.h"
#include "ace/Strategies_T.h"

ACE_RCSID(tao, IIOP_Connector, "$Id$")


TAO_IIOP_Connector::TAO_IIOP_Connector (CORBA::Boolean flag)
  : TAO_Connector (TAO_TAG_IIOP_PROFILE),
    lite_flag_ (flag),
    connect_strategy_ (),
    base_connector_ ()
{
}

TAO_IIOP_Connector::~TAO_IIOP_Connector (void)
{
}

int
TAO_IIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  if (this->init_tcp_properties () != 0)
    return -1;

  /// Our connect creation strategy
  TAO_IIOP_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_IIOP_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       &(this->tcp_properties_),
                       this->lite_flag_),
                  -1);

  /// Our activation strategy
  TAO_IIOP_CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  TAO_IIOP_CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);

  return this->base_connector_.open (this->orb_core ()->reactor (),
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     concurrency_strategy);
}

int
TAO_IIOP_Connector::close (void)
{
  delete this->base_connector_.concurrency_strategy ();
  delete this->base_connector_.creation_strategy ();
  return this->base_connector_.close ();
}

int
TAO_IIOP_Connector::connect (TAO_Connection_Descriptor_Interface *desc,
                             TAO_Transport *&transport,
                             ACE_Time_Value *max_wait_time,
                             CORBA::Environment &)
{
  TAO_Endpoint *endpoint = desc->endpoint ();

  if (endpoint->tag () != TAO_TAG_IIOP_PROFILE)
    return -1;

  TAO_IIOP_Endpoint *iiop_endpoint =
    ACE_dynamic_cast (TAO_IIOP_Endpoint *,
                      endpoint );
  if (iiop_endpoint == 0)
    return -1;

  const ACE_INET_Addr &remote_address =
    iiop_endpoint->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized properly.
  // Failure can occur if hostname lookup failed when initializing the
  // remote ACE_INET_Addr.
  if (remote_address.get_type () != AF_INET)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) IIOP connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) This is most likely ")
                      ACE_TEXT ("due to a hostname lookup ")
                      ACE_TEXT ("failure.\n")));
        }

      return -1;
    }

  int result = 0;
  TAO_IIOP_Connection_Handler *svc_handler = 0;
  TAO_Connection_Handler *conn_handler = 0;

  // Check the Cache first for connections
  if (this->orb_core ()->connection_cache ().find_handler (desc,
                                                           conn_handler) == 0)
    {
      // We have found a connection and a handler
      svc_handler =
        ACE_dynamic_cast (TAO_IIOP_Connection_Handler *,
                          conn_handler);
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) IIOP_Connector::connect - ")
                    ACE_TEXT ("got an existing connection on HANDLE %d\n"),
                    svc_handler->peer ().get_handle ()));
    }
  else
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) IIOP_Connector::connect - ")
                    ACE_TEXT ("making a new connection\n")));

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

      if (result == -1)
        {
          // Give users a clue to the problem.
          if (TAO_debug_level)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %s:%u, connection to ")
                          ACE_TEXT ("%s:%d failed (%p)\n"),
                          __FILE__,
                          __LINE__,
                          iiop_endpoint->host (),
                          iiop_endpoint->port (),
                          ACE_TEXT ("errno")));
            }
          return -1;
        }

      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) IIOP_Connector::connect - ")
                    ACE_TEXT ("new connection on HANDLE %d\n"),
                    svc_handler->peer ().get_handle ()));

      // Add the handler to Cache
      int retval =
        this->orb_core ()->connection_cache ().cache_handler (desc,
                                                              svc_handler);

      if (retval != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) IIOP_Connector::connect ")
                      ACE_TEXT ("could not add the new connection to Cache \n")));
        }
    }

  transport = svc_handler->transport ();
  return 0;
}

int
TAO_IIOP_Connector::preconnect (const char *preconnects)
{
  // Check for the proper protocol prefix.
  if (this->check_prefix (preconnects) != 0)
    return 0; // Failure: zero successful preconnections

  const char *protocol_removed =
    ACE_OS::strstr (preconnects,
                    "://") + 3;
  // "+ 3" since strlen of "://" is 3.

  char *preconnections =
    ACE_OS::strdup (protocol_removed);

  int successes = 0;
  if (preconnections)
    {
      ACE_INET_Addr dest;
      ACE_Unbounded_Stack<ACE_INET_Addr> dests;

      size_t num_connections;

      char *nextptr = 0;
      char *where = 0;
      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          int version_offset = 0;
          // Additional offset to remove version from preconnect, if it exists.

          if (isdigit (where[0]) &&
              where[1] == '.' &&
              isdigit (where[2]) &&
              where[3] == '@')
            version_offset = 4;

          // @@ For now, we just drop the version prefix.  However, at
          //    some point in the future the version may become useful.

          char *tport = 0;
          char *thost = where + version_offset;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';
              tport = sep + 1;

              dest.set ((u_short) ACE_OS::atoi (tport), thost);
              dests.push (dest);
            }
          else
            {
              // No port was specified so assume that the port will be the
              // IANA assigned port for IIOP.
              //
              //    IIOP:           683
              //    IIOP over SSL:  684

              dest.set (683, thost);
              dests.push (dest);

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) No port specified for <%s>.  ")
                              ACE_TEXT ("Using <%d> as default port.\n"),
                              where,
                              dest.get_port_number ()));
                }
            }
        }

      // Create an array of addresses from the stack, as well as an
      // array of eventual handlers.
      num_connections = dests.size ();
      ACE_INET_Addr *remote_addrs = 0;
      TAO_IIOP_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_INET_Addr[num_connections],
                      -1);

      ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_remote_addrs (remote_addrs);

      ACE_NEW_RETURN (handlers,
                      TAO_IIOP_Connection_Handler *[num_connections],
                      -1);

      ACE_Auto_Basic_Array_Ptr<TAO_IIOP_Connection_Handler*>
        safe_handlers (handlers);

      ACE_NEW_RETURN (failures,
                      char[num_connections],
                      -1);

      // No longer need to worry about exception safety at this point.
      remote_addrs = safe_remote_addrs.release ();
      handlers = safe_handlers.release ();

      size_t slot = 0;

      // Fill in the remote address array
      while (dests.pop (remote_addrs[slot]) == 0)
        handlers[slot++] = 0;

      // Finally, try to connect.
      this->base_connector_.connect_n (num_connections,
                                       handlers,
                                       remote_addrs,
                                       failures);
      // Loop over all the failures and set the handlers that
      // succeeded to idle state.
      for (slot = 0;
           slot < num_connections;
           slot++)
        {
          if (!failures[slot])
            {
              TAO_IIOP_Endpoint endpoint (remote_addrs[slot],
                                          0);
              TAO_Base_Connection_Property prop (&endpoint);

              // Add the handler to Cache
              int retval =
                this->orb_core ()->connection_cache ().cache_handler (&prop,
                                                                      handlers[slot]);
              successes++;

              if (retval != 0 && TAO_debug_level > 4)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("TAO (%P|%t) Unable to add handles\n"),
                            ACE_TEXT ("to cache \n")));

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("TAO (%P|%t) Preconnection <%s:%d> ")
                            ACE_TEXT ("succeeded.\n"),
                            remote_addrs[slot].get_host_name (),
                            remote_addrs[slot].get_port_number ()));
            }
          else if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) Preconnection <%s:%d> failed.\n"),
                        remote_addrs[slot].get_host_name (),
                        remote_addrs[slot].get_port_number ()));
        }

      ACE_OS::free (preconnections);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) IIOP preconnections: %d successes and ")
                    ACE_TEXT ("%d failures.\n"),
                    successes,
                    num_connections - successes));
    }
  return successes;
}

TAO_Profile *
TAO_IIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_IIOP_Profile (this->orb_core ()),
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
TAO_IIOP_Connector::make_profile (const char *endpoint,
                                  TAO_Profile *&profile,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  ACE_NEW_THROW_EX (profile,
                    TAO_IIOP_Profile (endpoint,
                                      this->orb_core (),
                                      ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  ACE_CHECK;
}

int
TAO_IIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "iiop", "iioploc" };

  size_t slot = ACE_OS::strchr (endpoint, ':') - endpoint;

  size_t len0 = ACE_OS::strlen (protocol[0]);
  size_t len1 = ACE_OS::strlen (protocol[1]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint, protocol[0], len0) == 0)
    return 0;
  else if (slot == len1
           && ACE_OS::strncasecmp (endpoint, protocol[1], len1) == 0)
    return 0;

  return -1;
  // Failure: not an IIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_IIOP_Connector::object_key_delimiter (void) const
{
  return TAO_IIOP_Profile::object_key_delimiter_;
}

int
TAO_IIOP_Connector::init_tcp_properties (void)
{

#if (TAO_HAS_RT_CORBA == 1)

  // Connector protocol properties are obtained from ORB-level
  // RTCORBA::ClientProtocolProperties policy override.
  // If the override doesn't exist or doesn't contain the
  // properties, we use ORB default.
  //
  // Currently, we do not use Object-level and Current-level policy
  // overrides for protocol configuration because connection
  // lookup and caching are not done based on protocol
  // properties.

  ACE_DECLARE_NEW_CORBA_ENV;

  // Check ORB-level override for tcp properties.
  TAO_ClientProtocolPolicy *client_protocols =
    this->orb_core ()->policy_manager ()->client_protocol ();
  CORBA::Object_var auto_release = client_protocols;
  RTCORBA::TCPProtocolProperties_var tcp_properties =
    RTCORBA::TCPProtocolProperties::_nil ();

  if (client_protocols != 0)
    {
      RTCORBA::ProtocolList & protocols = client_protocols->protocols_rep ();

      for (CORBA::ULong j = 0; j < protocols.length (); ++j)
        if (protocols[j].protocol_type == TAO_TAG_IIOP_PROFILE)
          {
            tcp_properties =
              RTCORBA::TCPProtocolProperties::_narrow
            (protocols[j].transport_protocol_properties.in (),
             ACE_TRY_ENV);
            ACE_CHECK_RETURN (-1);
            break;
          }
    }

  if (CORBA::is_nil (tcp_properties.in ()))
    {
      // No tcp properties in ORB-level override.  Use ORB defaults.
      // Orb defaults should never be null - they were initialized by
      // the ORB_Core.
      client_protocols = this->orb_core ()->default_client_protocol ();
      auto_release = client_protocols;
      RTCORBA::ProtocolList & protocols = client_protocols->protocols_rep ();
      for (CORBA::ULong j = 0; j < protocols.length (); ++j)
        if (protocols[j].protocol_type == TAO_TAG_IIOP_PROFILE)
          {
            tcp_properties =
              RTCORBA::TCPProtocolProperties::_narrow
              (protocols[j].transport_protocol_properties.in (),
               ACE_TRY_ENV);
            ACE_CHECK_RETURN (-1);
            break;
          }
    }

  // Extract and locally store properties of interest.
  this->tcp_properties_.send_buffer_size =
    tcp_properties->send_buffer_size ();
  this->tcp_properties_.recv_buffer_size =
    tcp_properties->recv_buffer_size ();
  this->tcp_properties_.no_delay =
    tcp_properties->no_delay ();

#else /* TAO_HAS_RT_CORBA == 1 */

  // Without RTCORBA, protocol configuration properties come from ORB
  // options.
  this->tcp_properties_.send_buffer_size =
    this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  this->tcp_properties_.recv_buffer_size =
    this->orb_core ()->orb_params ()->sock_rcvbuf_size ();
  this->tcp_properties_.no_delay =
    this->orb_core ()->orb_params ()->nodelay ();

#endif /* TAO_HAS_RT_CORBA == 1 */

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_INET_Addr>;
template class ACE_Unbounded_Stack<ACE_INET_Addr>;
template class ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>;
template class ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr>;
template class ACE_Hash<ARHR<ACE_INET_Addr> >;
template class ACE_Equal_To<ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr> >;

template class ACE_Concurrency_Strategy<TAO_IIOP_Client_Connection_Handler>;
template class ACE_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>;

template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>*>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Auto_Basic_Array_Ptr<TAO_IIOP_Client_Connection_Handler*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_INET_Addr>
#pragma instantiate ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#pragma instantiate ACE_Equal_To<ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr> >
#pragma instantiate ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr>
#pragma instantiate ACE_Hash<ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr> >

#pragma instantiate ACE_Concurrency_Strategy<TAO_IIOP_Client_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_CONNECTOR>

#pragma instantiate ACE_Connector<TAO_IIOP_Client_Connection_Handler, ACE_SOCK_Connector>
#pragma instantiate ACE_Creation_Strategy<TAO_IIOP_Client_Connection_Handler>
#pragma instantiate ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>
#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_IIOP_Client_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<TAO_IIOP_Client_Connection_Handler*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
