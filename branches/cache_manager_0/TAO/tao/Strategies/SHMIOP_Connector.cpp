// $Id$


#include "SHMIOP_Connector.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "SHMIOP_Profile.h"
#include "tao/debug.h"
#include "tao/Base_Transport_Property.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"
#include "ace/Auto_Ptr.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Invocation.h"
#include "tao/Thread_Lane_Resources.h"

ACE_RCSID (Strategies,
           SHMIOP_Connector,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Connect_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_SHMIOP_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>;
template class ACE_Connector<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>;
template class ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler>;

template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<ACE_HANDLE,ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler>*>;
template class ACE_Map_Iterator<ACE_HANDLE,ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE,ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>;
template class ACE_Auto_Basic_Array_Ptr<TAO_SHMIOP_Connection_Handler*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>
#pragma instantiate ACE_Connector<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler>

#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler> *, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE,ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler>*>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE,ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE,ACE_Svc_Tuple<TAO_SHMIOP_Connection_Handler>*,TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<TAO_SHMIOP_Connection_Handler*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


TAO_SHMIOP_Connector::TAO_SHMIOP_Connector (CORBA::Octet flag)
  : TAO_Connector (TAO_TAG_SHMEM_PROFILE),
    connect_strategy_ (),
    base_connector_ (),
    lite_flag_ (flag)
{
}

TAO_SHMIOP_Connector::~TAO_SHMIOP_Connector (void)
{
}


int
TAO_SHMIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  // Our connect creation strategy
  TAO_SHMIOP_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_SHMIOP_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
                       0,
                       this->lite_flag_),
                  -1);

  /// Our activation strategy
  TAO_SHMIOP_CONNECT_CONCURRENCY_STRATEGY *concurrency_strategy = 0;

  ACE_NEW_RETURN (concurrency_strategy,
                  TAO_SHMIOP_CONNECT_CONCURRENCY_STRATEGY (orb_core),
                  -1);

  if (this->base_connector_.open (this->orb_core ()->reactor (),
                                  connect_creation_strategy,
                                  &this->connect_strategy_,
                                  concurrency_strategy) == -1)
    return -1;
  // We can take advantage of the multithreaded shared-memory transport
  // if the client will block on read (i.e., will not allow callback.)
  else if (orb_core->client_factory ()->allow_callback () == 0)

    {
      this->base_connector_.connector ().preferred_strategy (ACE_MEM_IO::MT);
      this->connect_strategy_.connector ().preferred_strategy (ACE_MEM_IO::MT);
    }
  return 0;
}

int
TAO_SHMIOP_Connector::close (void)
{
  delete this->base_connector_.concurrency_strategy ();
  delete this->base_connector_.creation_strategy ();
  return this->base_connector_.close ();
}

int
TAO_SHMIOP_Connector::connect (TAO_GIOP_Invocation *invocation,
                               TAO_Transport_Descriptor_Interface *desc
                               TAO_ENV_ARG_DECL_NOT_USED)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for SHMIOP connection.\n")));

  TAO_Transport *&transport = invocation->transport ();
  ACE_Time_Value *max_wait_time = invocation->max_wait_time ();
  TAO_Endpoint *endpoint = desc->endpoint ();

  if (endpoint->tag () != TAO_TAG_SHMEM_PROFILE)
    return -1;

  TAO_SHMIOP_Endpoint *shmiop_endpoint =
    ACE_dynamic_cast (TAO_SHMIOP_Endpoint *,
                      endpoint);

  if (shmiop_endpoint == 0)
    return -1;

  const ACE_INET_Addr &remote_address =
    shmiop_endpoint->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized properly.
  // Failure can occur if hostname lookup failed when initializing the
  // remote ACE_INET_Addr.
  if (remote_address.get_type () != AF_INET)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) SHMIOP connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) This is most likely ")
                      ACE_TEXT ("due to a hostname lookup ")
                      ACE_TEXT ("failure.\n")));
        }

      return -1;
    }

  int result = 0;
  TAO_SHMIOP_Connection_Handler *svc_handler = 0;
  TAO_Transport *base_transport = 0;

  // Check the Cache first for connections
  // If transport found, reference count is incremented on assignment
  if (this->orb_core ()->lane_resources ().transport_cache ().find_transport (desc,
                                                                              base_transport) == 0)
    {
      if (TAO_debug_level > 5)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SHMIOP_Connector::connect ")
                    ACE_TEXT ("got an existing transport with id %d \n"),
                    base_transport->id ()));
    }
  else
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SHMIOP_Connector::connect ")
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

      if (TAO_debug_level > 4)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) SHMIOP_Connector::connect ")
                    ACE_TEXT ("The result is <%d> \n"), result));

      if (result == -1)
        {
          char buffer [MAXNAMELEN * 2];
          endpoint->addr_to_string (buffer,
                                    (MAXNAMELEN * 2) - 1);

          // Give users a clue to the problem.
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %s:%u, connection to ")
                          ACE_TEXT ("%s failed (%p)\n"),
                          __FILE__,
                          __LINE__,
                          buffer,
                          ACE_TEXT ("errno")));
            }
          return -1;
        }

      base_transport = TAO_Transport::_duplicate (svc_handler->transport ());
      // Add the handler to Cache
      int retval =
        this->orb_core ()->lane_resources ().transport_cache ().cache_transport (desc,
                                                                                 svc_handler->transport ());

      if (retval != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) SHMIOP_Connector::connect ")
                      ACE_TEXT ("could not add the new  connection to Cache \n")));
        }
    }

  // No need to _duplicate and release since base_transport
  // is going out of scope.  transport now has control of base_transport.
  transport = base_transport;

  return 0;
}


int
TAO_SHMIOP_Connector::preconnect (const char *preconnects)
{
#if 0
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

          // @@ Notice we reuqire the host name in preconnect
          // so the MEM_Connector can identify if we are trying
          // to a local host or not.

          if (sep)
            {
              *sep = '\0';
              tport = sep + 1;

              dest.set ((u_short) ACE_OS::atoi (tport), thost);
              dests.push (dest);
            }
          // @@ Skip the entry if we don't have a host:port pair available.
        }

      // Create an array of addresses from the stack, as well as an
      // array of eventual handlers.
      num_connections = dests.size ();
      ACE_INET_Addr *remote_addrs = 0;
      TAO_SHMIOP_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_INET_Addr[num_connections],
                      -1);

      ACE_Auto_Basic_Array_Ptr<ACE_INET_Addr> safe_remote_addrs (remote_addrs);

      ACE_NEW_RETURN (handlers,
                      TAO_SHMIOP_Connection_Handler *[num_connections],
                      -1);

      ACE_Auto_Basic_Array_Ptr<TAO_SHMIOP_Connection_Handler*>
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
              TAO_SHMIOP_Endpoint endpoint (remote_addrs[slot]);

              TAO_Base_Transport_Property prop (&endpoint);

              // Add the handler to Cache
              int retval =
                this->orb_core ()->lane_resources ().transport_cache ().cache_transport (&prop,
                                                                                         handlers[slot]->transport ());
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
                    ACE_TEXT ("TAO (%P|%t) SHMIOP preconnections: %d successes and ")
                    ACE_TEXT ("%d failures.\n"),
                    successes,
                    num_connections - successes));
    }
  return successes;
#else
  ACE_UNUSED_ARG (preconnects);
  ACE_NOTSUP_RETURN (-1);
#endif
}

TAO_Profile *
TAO_SHMIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_SHMIOP_Profile (this->orb_core ()),
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
TAO_SHMIOP_Connector::make_profile (TAO_ENV_SINGLE_ARG_DECL)
{
  // The endpoint should be of the form:
  //    N.n@port/object_key
  // or:
  //    port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_SHMIOP_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return profile;
}

int
TAO_SHMIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "shmiop", "shmioploc" };

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
  // Failure: not an SHMIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_SHMIOP_Connector::object_key_delimiter (void) const
{
  return TAO_SHMIOP_Profile::object_key_delimiter_;
}


#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
