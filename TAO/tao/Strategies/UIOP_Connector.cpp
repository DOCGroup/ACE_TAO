// $Id$

#include "UIOP_Connector.h"

#if TAO_HAS_UIOP == 1

#include "UIOP_Profile.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"
#include "ace/Auto_Ptr.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Invocation.h"
#include "tao/Thread_Lane_Resources.h"

ACE_RCSID(Strategies,
          UIOP_Connector,
          "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_UNIX_Addr>;
template class ACE_Unbounded_Stack<ACE_UNIX_Addr>;
template class ACE_Auto_Basic_Array_Ptr<ACE_UNIX_Addr>;

template class TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>;

template class ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*>;
template class ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;
template class ACE_Auto_Basic_Array_Ptr<TAO_UIOP_Connection_Handler*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_UNIX_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_UNIX_Addr>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<ACE_UNIX_Addr>

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_UIOP_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_UIOP_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Connector<TAO_UIOP_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>

#pragma instantiate ACE_Map_Manager<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<int, ACE_Svc_Tuple<TAO_UIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<int,ACE_Svc_Tuple<TAO_UIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<TAO_UIOP_Connection_Handler*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

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
TAO_UIOP_Connector::connect (TAO_GIOP_Invocation *invocation,
                             TAO_Transport_Descriptor_Interface *desc
                             ACE_ENV_ARG_DECL_NOT_USED)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for UIOP connection.\n")));

  TAO_Transport *&transport = invocation->transport ();
  ACE_Time_Value *max_wait_time = invocation->max_wait_time ();
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
  TAO_Transport *base_transport = 0;

  // Check the Cache first for connections
  // If transport found, reference count is incremented on assignment
  if (this->orb_core ()->lane_resources ().transport_cache ().find_transport (desc,
                                                                              base_transport) == 0)
    {
      if (TAO_debug_level > 5)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                    ACE_TEXT ("got an existing transport with id %d \n"),
                    base_transport->id ()));
    }
  else
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                    ACE_TEXT ("making a new connection \n")));

      // Purge connections (if necessary)
      this->orb_core ()->lane_resources ().transport_cache ().purge ();

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

      base_transport = TAO_Transport::_duplicate (svc_handler->transport ());
      // Add the handler to Cache
      int retval =
        this->orb_core ()->lane_resources ().transport_cache ().cache_transport (desc,
                                                                                 base_transport);

      if (retval != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) UIOP_Connector::connect ")
                      ACE_TEXT ("could not add the new connection to Cache \n")));
        }
    }

  // No need to _duplicate and release since base_transport
  // is going out of scope.  transport now has control of base_transport.
  transport = base_transport;

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

              TAO_Base_Transport_Property prop (&endpoint);

              // Add the handler to Cache
              int retval =
                this->orb_core ()->lane_resources ().transport_cache ().cache_transport (&prop,
                                                                                         handlers[slot]->transport ());
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

TAO_Profile *
TAO_UIOP_Connector::make_profile (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_UIOP_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  ACE_CHECK_RETURN (0);

  return profile;
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

  int send_buffer_size = this->orb_core ()->orb_params ()->sock_sndbuf_size ();
  int recv_buffer_size = this->orb_core ()->orb_params ()->sock_rcvbuf_size ();
  int no_delay = 0;

  TAO_Protocols_Hooks *tph = this->orb_core ()->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (tph != 0)
    {
      const char protocol [] = "uiop";
      const char *protocol_type = protocol;
      int hook_result =
        tph->call_client_protocols_hook (send_buffer_size,
                                         recv_buffer_size,
                                         no_delay,
                                         protocol_type);

      if(hook_result == -1)
        return -1;
    }

    // Extract and locally store properties of interest.
    this->uiop_properties_.send_buffer_size =
      send_buffer_size;
    this->uiop_properties_.recv_buffer_size =
      recv_buffer_size;

  return 0;
}





#endif /* TAO_HAS_UIOP == 1 */
