// This may look like C, but it's really -*- C++ -*-
// $Id$


#include "UIOP_Connector.h"

#if TAO_HAS_UIOP == 1

#include "UIOP_Profile.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"
#include "ace/Auto_Ptr.h"
#include "tao/RT_Policy_i.h"
#include "tao/Base_Connection_Property.h"

ACE_RCSID(Strategies, UIOP_Connector, "$Id$")


TAO_UIOP_Connector::TAO_UIOP_Connector (CORBA::Boolean flag)
  : TAO_Connector (TAO_TAG_UIOP_PROFILE),
    connect_strategy_ (),
    base_connector_ (),
    lite_flag_ (flag)
{
}

TAO_UIOP_Connector::~TAO_UIOP_Connector (void)
{
}

int
TAO_UIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  if (this->init_uiop_properties () != 0)
    return -1;

  // Our connect creation strategy
  TAO_UIOP_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_UIOP_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       &(this->uiop_properties_),
                       this->lite_flag_),
                  -1);

  /// Our activation strategy
  TAO_UIOP_CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  TAO_UIOP_CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);

  return this->base_connector_.open (this->orb_core ()->reactor (),
                                     connect_creation_strategy,
                                     &this->connect_strategy_,
                                     concurrency_strategy);
}

int
TAO_UIOP_Connector::close (void)
{
  // Zap the creation strategy that we created earlier.
  delete this->base_connector_.creation_strategy ();
  delete this->base_connector_.concurrency_strategy ();

  return this->base_connector_.close ();
}


int
TAO_UIOP_Connector::connect (TAO_Connection_Descriptor_Interface *desc,
                             TAO_Transport *& transport,
                             ACE_Time_Value *max_wait_time,
                             CORBA::Environment &)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for UIOP connection.\n")));

  TAO_Endpoint *endpoint = desc->endpoint ();

  if (endpoint->tag () != TAO_TAG_UIOP_PROFILE)
    return -1;

  TAO_UIOP_Endpoint *uiop_endpoint =
    ACE_dynamic_cast (TAO_UIOP_Endpoint *,
                      endpoint);

  if (uiop_endpoint == 0)
    return -1;

  const ACE_UNIX_Addr &remote_address =
    uiop_endpoint->object_addr ();

  // @@ Note, POSIX.1g renames AF_UNIX to AF_LOCAL.

  // Verify that the remote ACE_UNIX_Addr was initialized properly.
  // Failure should never occur in the case of an ACE_UNIX_Addr!
  if (remote_address.get_type () != AF_UNIX)
    return -1;

  int result = 0;
  TAO_UIOP_Connection_Handler *svc_handler = 0;
  TAO_Connection_Handler *conn_handler = 0;

  // Check the Cache first for connections
  if (this->orb_core ()->connection_cache ().find_handler (desc,
                                                           conn_handler) == 0)
    {
      if (TAO_debug_level > 5)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                    ACE_TEXT ("got an existing connection \n")));

      // We have found a connection and a handler
      svc_handler =
        ACE_dynamic_cast (TAO_UIOP_Connection_Handler *,
                          conn_handler);
    }
  else
    {
      if (TAO_debug_level > 4)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
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
                    ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                    ACE_TEXT ("The result is <%d> \n"), result));

      if (result == -1)
        {
          // Give users a clue to the problem.
          if (TAO_debug_level)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %s:%u, connection to ")
                          ACE_TEXT ("%s failed (%p)\n"),
                          __FILE__,
                          __LINE__,
                          uiop_endpoint->rendezvous_point (),
                          ACE_TEXT ("errno")));
            }
          return -1;
        }

      // Add the handler to Cache
      int retval =
        this->orb_core ()->connection_cache ().cache_handler (desc,
                                                              svc_handler);

      if (retval != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                      ACE_TEXT ("could not add the new connection to Cache \n")));
        }
    }

  transport = svc_handler->transport ();

  return 0;
}

int
TAO_UIOP_Connector::preconnect (const char *preconnects)
{
  // Check for the proper protocol prefix.
  if (this->check_prefix (preconnects) != 0)
    return 0; // Failure: zero successful preconnections

  const char *protocol_removed =
    ACE_OS::strstr (preconnects, "://") + 3;
  // "+ 3" since strlen of "://" is 3.

  char *preconnections =
    ACE_OS::strdup (protocol_removed);

  int successes = 0;
  if (preconnections)
    {
      ACE_UNIX_Addr dest;
      ACE_Unbounded_Stack<ACE_UNIX_Addr> dests;

      size_t num_connections;

      char *nextptr = 0;
      char *where = 0;

      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          char *rendezvous_point = where;

          int version_offset = 0;
          // Additional offset to remove version from preconnect, if
          // it exists.

          if (isdigit (rendezvous_point[0]) &&
              rendezvous_point[1] == '.' &&
              isdigit (rendezvous_point[2]) &&
              rendezvous_point[3] == '@')
            version_offset = 4;

          // @@ For now, we just drop the version prefix.  However, at
          // some point in the future the version may become useful.

          dest.set (rendezvous_point + version_offset);

          dests.push (dest);
        }

      // Create an array of addresses from the stack, as well as an
      // array of eventual handlers.
      num_connections = dests.size ();
      ACE_UNIX_Addr *remote_addrs = 0;
      TAO_UIOP_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_UNIX_Addr[num_connections],
                      -1);

      ACE_Auto_Basic_Array_Ptr<ACE_UNIX_Addr> safe_remote_addrs (remote_addrs);

      ACE_NEW_RETURN (handlers,
                      TAO_UIOP_Connection_Handler *[num_connections],
                      -1);

      ACE_Auto_Basic_Array_Ptr<TAO_UIOP_Connection_Handler *>
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
           ++slot)
        {
          if (!failures[slot])
            {
              TAO_UIOP_Endpoint endpoint (remote_addrs[slot]);

              TAO_Base_Connection_Property prop (&endpoint);

              // Add the handler to Cache
              int retval =
                this->orb_core ()->connection_cache ().cache_handler (&prop,
                                                                      handlers[slot]);
              ++successes;

              if (retval != 0 && TAO_debug_level > 4)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("TAO (%P|%t) Unable to add handles\n"),
                            ACE_TEXT ("to cache \n")));

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "TAO (%P|%t) Preconnection <%s> succeeded.\n",
                            remote_addrs[slot].get_path_name ()));
            }
          else if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) Preconnection <%s> failed.\n",
                        remote_addrs[slot].get_path_name ()));
        }

      ACE_OS::free (preconnections);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) UIOP preconnections: %d successes and "
                    "%d failures.\n",
                    successes,
                    num_connections - successes));
    }

  return successes;
}

TAO_Profile *
TAO_UIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_UIOP_Profile (this->orb_core ()),
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
TAO_UIOP_Connector::make_profile (const char *endpoint,
                                  TAO_Profile *&profile,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // The endpoint should be of the form:
  //    N.n@rendezvous_point|object_key
  // or:
  //    rendezvous_point|object_key

  ACE_NEW_THROW_EX (profile,
                    TAO_UIOP_Profile (endpoint,
                                      this->orb_core (),
                                      ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  ACE_CHECK;

}

int
TAO_UIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "uiop", "uioploc" };

  size_t slot = ACE_OS::strchr (endpoint, ':') - endpoint;

  size_t len0 = ACE_OS::strlen (protocol[0]);
  size_t len1 = ACE_OS::strlen (protocol[1]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint,
                              protocol[0],
                              len0) == 0)
    return 0;
  else if (slot == len1
           && ACE_OS::strncasecmp (endpoint,
                                   protocol[1],
                                   len1) == 0)
    return 0;

  return -1;
  // Failure: not an UIOP IOR DO NOT throw an exception here.
}

char
TAO_UIOP_Connector::object_key_delimiter (void) const
{
  return TAO_UIOP_Profile::object_key_delimiter_;
}

int
TAO_UIOP_Connector::init_uiop_properties (void)
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
  RTCORBA::UnixDomainProtocolProperties_var uiop_properties =
    RTCORBA::UnixDomainProtocolProperties::_nil ();

  if (client_protocols != 0)
    {
      RTCORBA::ProtocolList & protocols = client_protocols->protocols_rep ();

      for (CORBA::ULong j = 0; j < protocols.length (); ++j)
        if (protocols[j].protocol_type == TAO_TAG_UIOP_PROFILE)
          {
            uiop_properties =
              RTCORBA::UnixDomainProtocolProperties::_narrow
            (protocols[j].transport_protocol_properties.in (),
             ACE_TRY_ENV);
            ACE_CHECK_RETURN (-1);
            break;
          }
    }

  if (CORBA::is_nil (uiop_properties.in ()))
    {
      // No tcp properties in ORB-level override.  Use ORB defaults.
      // Orb defaults should never be null - they were initialized by
      // the ORB_Core.
      client_protocols = this->orb_core ()->default_client_protocol ();
      auto_release = client_protocols;
      RTCORBA::ProtocolList & protocols = client_protocols->protocols_rep ();
      for (CORBA::ULong j = 0; j < protocols.length (); ++j)
        if (protocols[j].protocol_type == TAO_TAG_UIOP_PROFILE)
          {
            uiop_properties =
              RTCORBA::UnixDomainProtocolProperties::_narrow
              (protocols[j].transport_protocol_properties.in (),
               ACE_TRY_ENV);
            ACE_CHECK_RETURN (-1);
            break;
          }
    }

  // Extract and locally store properties of interest.
  this->uiop_properties_.send_buffer_size =
    uiop_properties->send_buffer_size ();
  this->uiop_properties_.recv_buffer_size =
    uiop_properties->recv_buffer_size ();

#else /* TAO_HAS_RT_CORBA == 1 */

  // Without RTCORBA, protocol configuration properties come from ORB
  // options.
  this->uiop_properties_.send_buffer_size =
    this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  this->uiop_properties_.recv_buffer_size =
    this->orb_core ()->orb_params ()->sock_rcvbuf_size ();

#endif /* TAO_HAS_RT_CORBA == 1 */

  return 0;
}




#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_UNIX_Addr>;
template class ACE_Unbounded_Stack<ACE_UNIX_Addr>;
template class ACE_Refcounted_Hash_Recyclable<ACE_UNIX_Addr>;
template class ACE_Auto_Basic_Array_Ptr<ACE_UNIX_Addr>;
template class ACE_Hash<ARHR<ACE_UNIX_Addr> >;
template class ACE_Equal_To<ACE_Refcounted_Hash_Recyclable<ACE_UNIX_Addr> >;

template class TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>;

template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>;
template class ACE_Auto_Basic_Array_Ptr<TAO_UIOP_Connection_Handler*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_UNIX_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_UNIX_Addr>
#pragma instantiate ACE_Refcounted_Hash_Recyclable<ACE_UNIX_Addr>
#pragma instantiate ACE_Equal_To<ACE_Refcounted_Hash_Recyclable<ACE_UNIX_Addr> >
#pragma instantiate ACE_Auto_Basic_Array_Ptr<ACE_UNIX_Addr>
#pragma instantiate ACE_Hash<ACE_Refcounted_Hash_Recyclable<ACE_UNIX_Addr> >

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>

#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<TAO_UIOP_Connection_Handler*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_UIOP == 1 */
