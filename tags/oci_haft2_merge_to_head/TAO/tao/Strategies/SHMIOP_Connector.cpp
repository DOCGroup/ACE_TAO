// $Id$


#include "SHMIOP_Connector.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "SHMIOP_Profile.h"
#include "SHMIOP_Endpoint.h"
#include "tao/debug.h"
#include "tao/Base_Transport_Property.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Environment.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Invocation.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Blocked_Connect_Strategy.h"

ACE_RCSID (Strategies,
           SHMIOP_Connector,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Connect_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>;
template class TAO_Connect_Creation_Strategy<TAO_SHMIOP_Connection_Handler>;
template class ACE_Strategy_Connector<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>;
template class ACE_Connector_Base<TAO_SHMIOP_Connection_Handler>;
template class ACE_Connector<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>;
template class ACE_NonBlocking_Connect_Handler<TAO_SHMIOP_Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Connect_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate TAO_Connect_Creation_Strategy<TAO_SHMIOP_Connection_Handler>
#pragma instantiate ACE_Strategy_Connector<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>
#pragma instantiate ACE_Connector_Base<TAO_SHMIOP_Connection_Handler>
#pragma instantiate ACE_Connector<TAO_SHMIOP_Connection_Handler, ACE_MEM_CONNECTOR>
#pragma instantiate ACE_NonBlocking_Connect_Handler<TAO_SHMIOP_Connection_Handler>

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

  // The SHMIOP always uses a blocked connect strategy
  // @@todo: There are better ways of doing this. Let it be like this
  // for the  present.
  ACE_NEW_RETURN (this->active_connect_strategy_,
                  TAO_Blocked_Connect_Strategy (orb_core),
                  -1);

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
TAO_SHMIOP_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != TAO_TAG_SHMEM_PROFILE)
    return -1;

  TAO_SHMIOP_Endpoint *shmiop_endpoint =
    ACE_dynamic_cast (TAO_SHMIOP_Endpoint *,
                      endpoint );
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
                       ACE_LIB_TEXT ("TAO (%P|%t) SHMIOP connection failed.\n")
                       ACE_LIB_TEXT ("TAO (%P|%t) This is most likely ")
                       ACE_LIB_TEXT ("due to a hostname lookup ")
                       ACE_LIB_TEXT ("failure.\n")));
         }

       return -1;
     }

   return 0;

}

int
TAO_SHMIOP_Connector::make_connection (TAO_GIOP_Invocation *invocation,
                                       TAO_Transport_Descriptor_Interface *desc,
                                       ACE_Time_Value *max_wait_time)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Connector::connect - ")
                  ACE_TEXT ("looking for SHMIOP connection.\n")));

  TAO_SHMIOP_Endpoint *shmiop_endpoint =
    this->remote_endpoint (desc->endpoint ());

  if (shmiop_endpoint == 0)
    return -1;

  const ACE_INET_Addr &remote_address =
    shmiop_endpoint->object_addr ();

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) SHMIOP_Connector::connect ")
                ACE_TEXT ("making a new connection \n")));

  // Get the right synch options
  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (max_wait_time,
                                                 synch_options);

  TAO_SHMIOP_Connection_Handler *svc_handler = 0;

  // Connect.
  int result = this->base_connector_.connect (svc_handler,
                                              remote_address,
                                              synch_options);

  // This call creates the service handler and bumps the #REFCOUNT# up
  // one extra.  There are two possibilities: (a) connection succeeds
  // immediately - in this case, the #REFCOUNT# on the handler is two;
  // (b) connection fails immediately - in this case, the #REFCOUNT#
  // on the handler is one since close() gets called on the handler.
  // We always use a blocking connection so the connection is never
  // pending.

  // Irrespective of success or failure, remove the extra #REFCOUNT#.
  svc_handler->remove_reference ();

  // In case of errors.
  if (result == -1)
    {
      // Give users a clue to the problem.
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %s:%u, connection to ")
                      ACE_TEXT ("<%s:%p> failed (%p)\n"),
                      __FILE__,
                      __LINE__,
                      shmiop_endpoint->host (),
                      shmiop_endpoint->port (),
                      ACE_TEXT ("errno")));
        }

      return -1;
    }

  // At this point, the connection has be successfully connected.
  // #REFCOUNT# is one.
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - SHMIOP_Connector::make_connection, "
                "new connection to <%s:%d> on Transport[%d]\n",
                shmiop_endpoint->host (), shmiop_endpoint->port (),
                svc_handler->peer ().get_handle ()));

  TAO_Transport *transport =
    svc_handler->transport ();

  // Add the handler to Cache
  int retval =
    this->orb_core ()->lane_resources ().transport_cache ().cache_transport (desc,
                                                                             transport);

  // Failure in adding to cache.
  if (retval != 0)
    {
      // Close the handler.
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - SHMIOP_Connector::make_connection, "
                      "could not add the new connection to cache\n"));
        }

      return -1;
    }

  // If the wait strategy wants us to be registered with the reactor
  // then we do so. If registeration is required and it succeeds,
  // #REFCOUNT# becomes two.
  retval =  transport->wait_strategy ()->register_handler ();

  // Registration failures.
  if (retval != 0)
    {
      // Purge from the connection cache.
      transport->purge_entry ();

      // Close the handler.
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO (%P|%t) - SHMIOP_Connector::make_connection, "
                      "could not register the new connection in the reactor\n"));
        }

      return -1;
    }

  // Handover the transport pointer to the Invocation class.
  TAO_Transport *&invocation_transport =
    invocation->transport ();
  invocation_transport = transport;

  return 0;
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
TAO_SHMIOP_Connector::make_profile (ACE_ENV_SINGLE_ARG_DECL)
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

TAO_SHMIOP_Endpoint *
TAO_SHMIOP_Connector::remote_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != TAO_TAG_SHMEM_PROFILE)
    return 0;

  TAO_SHMIOP_Endpoint *shmiop_endpoint =
    ACE_dynamic_cast (TAO_SHMIOP_Endpoint *,
                      endpoint );
  if (shmiop_endpoint == 0)
    return 0;

  return shmiop_endpoint;
}

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
