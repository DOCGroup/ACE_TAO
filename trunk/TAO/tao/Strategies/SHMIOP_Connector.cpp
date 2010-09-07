// $Id$

#include "tao/Strategies/SHMIOP_Connector.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Strategies/SHMIOP_Profile.h"
#include "tao/Strategies/SHMIOP_Endpoint.h"
#include "tao/debug.h"
#include "tao/Base_Transport_Property.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/SystemException.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Blocked_Connect_Strategy.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_SHMIOP_Connector::TAO_SHMIOP_Connector (void)
  : TAO_Connector (TAO_TAG_SHMEM_PROFILE),
    connect_strategy_ (),
    base_connector_ ()
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
  // @todo: There are better ways of doing this. Let it be like this
  // for the  present.
  ACE_NEW_RETURN (this->active_connect_strategy_,
                  TAO_Blocked_Connect_Strategy (orb_core),
                  -1);

  // Our connect creation strategy
  TAO_SHMIOP_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_SHMIOP_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core),
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
    dynamic_cast <TAO_SHMIOP_Endpoint *>(endpoint);
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
           ACE_ERROR ((LM_ERROR,
                       ACE_TEXT ("TAO (%P|%t) SHMIOP connection failed.\n")
                       ACE_TEXT ("TAO (%P|%t) This is most likely ")
                       ACE_TEXT ("due to a hostname lookup ")
                       ACE_TEXT ("failure.\n")));
         }

       return -1;
     }

   return 0;

}

TAO_Transport *
TAO_SHMIOP_Connector::make_connection (TAO::Profile_Transport_Resolver *,
                                       TAO_Transport_Descriptor_Interface &desc,
                                       ACE_Time_Value *timeout)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - SHMIOP_Connector::make_connection, ")
                  ACE_TEXT ("looking for SHMIOP connection.\n")));

  TAO_SHMIOP_Endpoint *shmiop_endpoint =
    this->remote_endpoint (desc.endpoint ());

  if (shmiop_endpoint == 0)
    return 0;

  const ACE_INET_Addr &remote_address =
    shmiop_endpoint->object_addr ();

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - SHMIOP_Connector::make_connection, "
                "making a new connection to <%C:%d>\n",
                shmiop_endpoint->host (),
                shmiop_endpoint->port ()));

  // Get the right synch options
  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (timeout,
                                                 synch_options);

  TAO_SHMIOP_Connection_Handler *svc_handler = 0;

  // Connect.
  int result = this->base_connector_.connect (svc_handler,
                                              remote_address,
                                              synch_options);

  // Make sure that we always do a remove_reference
  ACE_Event_Handler_var svc_handler_auto_ptr (svc_handler);

  // In case of errors.
  if (result == -1)
    {
      // Give users a clue to the problem.
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - SHMIOP_Connector::make_connection, ")
                      ACE_TEXT ("connection to <%C:%u> failed (%p)\n"),
                      shmiop_endpoint->host (),
                      shmiop_endpoint->port (),
                      ACE_TEXT ("errno")));
        }

      return 0;
    }

  if (svc_handler->keep_waiting ())
    {
      svc_handler->connection_pending ();
    }

  if (svc_handler->error_detected ())
    {
      svc_handler->cancel_pending_connection ();
    }

  TAO_Transport *transport =
    svc_handler->transport ();

  // At this point, the connection has be successfully connected.
  // #REFCOUNT# is one.
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - SHMIOP_Connector::make_connection, "
                "new %C connection to <%C:%d> on Transport[%d]\n",
                transport->is_connected() ? "connected" : "not connected",
                shmiop_endpoint->host (),
                shmiop_endpoint->port (),
                svc_handler->peer ().get_handle ()));

  // Add the handler to Cache
  int retval =
    this->orb_core ()->lane_resources ().transport_cache ().cache_transport (&desc,
                                                                             transport);

  // Failure in adding to cache.
  if (retval == -1)
    {
      // Close the handler.
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("TAO (%P|%t) - SHMIOP_Connector::make_connection, ")
                      ACE_TEXT("could not add the new connection to cache\n")));
        }

      return 0;
    }

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
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - SHMIOP_Connector [%d]::make_connection, "
                    "could not register the transport in the reactor.\n",
                    transport->id ()));

      return 0;
    }

  svc_handler_auto_ptr.release ();
  return transport;
}

TAO_Profile *
TAO_SHMIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile = 0;
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
TAO_SHMIOP_Connector::make_profile (void)
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
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

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
    dynamic_cast <TAO_SHMIOP_Endpoint *>(endpoint);
  if (shmiop_endpoint == 0)
    return 0;

  return shmiop_endpoint;
}

int
TAO_SHMIOP_Connector::cancel_svc_handler (
  TAO_Connection_Handler * svc_handler)
{
  TAO_SHMIOP_Connection_Handler* handler=
    dynamic_cast<TAO_SHMIOP_Connection_Handler*>(svc_handler);

  if (handler)
    // Cancel from the connector
    return this->base_connector_.cancel (handler);

  return -1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
