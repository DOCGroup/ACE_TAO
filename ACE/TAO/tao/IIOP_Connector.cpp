#include "tao/IIOP_Connector.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/IIOP_Profile.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Connect_Strategy.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Transport.h"
#include "tao/Wait_Strategy.h"
#include "tao/SystemException.h"
#include "tao/LF_Multi_Event.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"

ACE_RCSID (tao,
           IIOP_Connector,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL



//-----------------------------------------------------------------------------

/**
 * @class TAO_Event_Handler_Array_var
 *
 * @brief Auto pointer like class for an array of Event Handlers.
 *
 * Used to manage lifecycle of handlers. This class calls
 * ACE_Event_Handler::remove_reference() on each handler in its destructor
 * This class started out life as a replacement for the ACE_Event_Handle_var
 * but is now pared down to be very specific in its role..
 */
class TAO_IIOP_Connection_Handler_Array_Guard
{
public:
  TAO_IIOP_Connection_Handler_Array_Guard (TAO_IIOP_Connection_Handler **p, unsigned count);
  ~TAO_IIOP_Connection_Handler_Array_Guard (void);

private:
  /// Handler.
  TAO_IIOP_Connection_Handler **ptr_;
  unsigned count_;
};

TAO_IIOP_Connection_Handler_Array_Guard::TAO_IIOP_Connection_Handler_Array_Guard (TAO_IIOP_Connection_Handler **p,
                                                          unsigned count)
  : ptr_ (p),
    count_ (count)
{
}

TAO_IIOP_Connection_Handler_Array_Guard::~TAO_IIOP_Connection_Handler_Array_Guard (void)
{
  if (this->ptr_ != 0)
    {
      for (unsigned i = 0; i < this->count_; i++)
        this->ptr_[i]->remove_reference ();
    }
}

//---------------------------------------------------------------------------


TAO_IIOP_Connector::~TAO_IIOP_Connector (void)
{
}

//@@ TAO_CONNECTOR_SPL_COPY_HOOK_START
TAO_IIOP_Connector::TAO_IIOP_Connector (CORBA::Boolean flag)
  : TAO_Connector (IOP::TAG_INTERNET_IOP)
  , lite_flag_ (flag)
  , connect_strategy_ ()
  , base_connector_ ()
{
}

int
TAO_IIOP_Connector::open (TAO_ORB_Core *orb_core)
{
  // @@todo: The functionality of the following two statements could
  // be done in the constructor, but that involves changing the
  // interface of the pluggable transport factory.

  // Set the ORB Core
  this->orb_core (orb_core);

  // Create our connect strategy
  if (this->create_connect_strategy () == -1)
    return -1;

  /// Our connect creation strategy
  TAO_IIOP_CONNECT_CREATION_STRATEGY *connect_creation_strategy = 0;

  ACE_NEW_RETURN (connect_creation_strategy,
                  TAO_IIOP_CONNECT_CREATION_STRATEGY
                      (orb_core->thr_mgr (),
                       orb_core,
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
TAO_IIOP_Connector::supports_parallel_connects(void) const
{
  return 1;
}

int
TAO_IIOP_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  TAO_IIOP_Endpoint *iiop_endpoint =
    this->remote_endpoint (endpoint);

  if (iiop_endpoint == 0)
    return -1;

   const ACE_INET_Addr &remote_address =
     iiop_endpoint->object_addr ();

   // Verify that the remote ACE_INET_Addr was initialized properly.
   // Failure can occur if hostname lookup failed when initializing the
   // remote ACE_INET_Addr.
#if defined (ACE_HAS_IPV6)
   if (remote_address.get_type () != AF_INET &&
       remote_address.get_type () != AF_INET6)
#else /* ACE_HAS_IPV6 */
   if (remote_address.get_type () != AF_INET)
#endif /* !ACE_HAS_IPV6 */
     {
       if (TAO_debug_level > 0)
         {
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("(%P|%t) IIOP connection failed.\n")
                       ACE_TEXT ("     This is most likely ")
                       ACE_TEXT ("due to a hostname lookup ")
                       ACE_TEXT ("failure.\n")));
         }

       return -1;
     }

   return 0;
}

TAO_Transport *
TAO_IIOP_Connector::make_connection (TAO::Profile_Transport_Resolver *r,
                                     TAO_Transport_Descriptor_Interface &desc,
                                     ACE_Time_Value *timeout)
{
  TAO_IIOP_Connection_Handler *svc_handler = 0;
  TAO_IIOP_Endpoint *iiop_endpoint =
    this->remote_endpoint (desc.endpoint());
  int result = -1;
  if (iiop_endpoint == 0)
    return 0;

  result = this->begin_connection (svc_handler, r, iiop_endpoint, timeout);

  if (result == -1 && errno != EWOULDBLOCK)
    {
      // connect completed unsuccessfully
      svc_handler->remove_reference();
       // Give users a clue to the problem.
      if (TAO_debug_level > 1)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) IIOP_Connector::make_connection, ")
                      ACE_TEXT("connection to <%s:%d> failed (%p)\n"),
                      ACE_TEXT_CHAR_TO_TCHAR (iiop_endpoint->host ()),
                      iiop_endpoint->port (),
                      ACE_TEXT("errno")));
        }
    return 0;
    }
  TAO_IIOP_Connection_Handler **sh_ptr = &svc_handler;
  TAO_IIOP_Endpoint **ep_ptr = &iiop_endpoint;
  TAO_LF_Multi_Event mev;
  mev.add_event(svc_handler);
  return this->complete_connection (result, desc,
                                    sh_ptr, ep_ptr,
                                    1U, r, &mev,  timeout);
}

TAO_Transport *
TAO_IIOP_Connector::make_parallel_connection (TAO::Profile_Transport_Resolver *r,
                                     TAO_Transport_Descriptor_Interface &desc,
                                     ACE_Time_Value *timeout)
{
  TAO_Endpoint *root_ep = desc.endpoint();
  unsigned max_count = 1;
  unsigned long ns_stagger =
    this->orb_core()->orb_params()->parallel_connect_delay();
  unsigned long sec_stagger = ns_stagger/1000;
  ns_stagger = (ns_stagger % 1000) * 1000000;
  for (TAO_Endpoint *ep = root_ep->next_filtered (this->orb_core(),0);
       ep != 0;
       ep = ep->next_filtered(this->orb_core(),root_ep))
    max_count++;

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - IIOP_Connector::")
                ACE_TEXT ("make_parallel_connection, ")
                ACE_TEXT ("to %d endpoints\n"), max_count));
  TAO_IIOP_Endpoint **eplist = 0;
  TAO_IIOP_Connection_Handler **shlist = 0;
  ACE_NEW_RETURN (shlist, TAO_IIOP_Connection_Handler *[max_count], 0);
  ACE_NEW_RETURN (eplist, TAO_IIOP_Endpoint *[max_count], 0);

  TAO_LF_Multi_Event mev;
  int result = 0;
  unsigned count = 0;
  for (TAO_Endpoint *ep = root_ep->next_filtered (this->orb_core(),0);
       ep != 0;
       ep = ep->next_filtered(this->orb_core(),root_ep))
    {
      eplist[count] = this->remote_endpoint (ep);
      shlist[count] = 0;
      result = this->begin_connection (shlist[count],
                                       r,
                                       eplist[count],
                                       timeout);

      // The connection may fail because it is slow, or for other reasons.
      // If it was an incomplete non-blocking connection, add it to the list
      // to be waited on, otherwise remove the reference to the handler and
      // move on to the next endpoint.
      if (result == -1)
        {
          if (errno == EWOULDBLOCK)
            {
              mev.add_event(shlist[count++]);
              if (ep->next() != 0)
                {
                  struct timespec nsleep = {sec_stagger, ns_stagger};
                  ACE_OS::nanosleep (&nsleep);
                  result = this->active_connect_strategy_->poll (&mev);
                  if (result != -1)
                    break;
                }
            }
          else
            {
              shlist[count]->remove_reference(); // done bump the list count
            }
          continue;
        }

      if (result != -1) // we have a winner!
        {
          count++;
          break; // no waiting involved since a connection is completed
        }
    }

  TAO_Transport *winner = 0;
  if (count > 0) // only complete if at least one pending or success
    winner = this->complete_connection (result,desc,
                                        shlist,eplist,count,r,&mev,timeout);
  delete [] shlist; // reference reductions should have been done already
  delete [] eplist;
  return winner;
}

int
TAO_IIOP_Connector::begin_connection (TAO_IIOP_Connection_Handler *&svc_handler,
                                      TAO::Profile_Transport_Resolver *r,
                                      TAO_IIOP_Endpoint *iiop_endpoint,
                                      ACE_Time_Value *timeout)
{
  const ACE_INET_Addr &remote_address =
    iiop_endpoint->object_addr ();

  u_short port = 0;
  const ACE_UINT32 ia_any = INADDR_ANY;
  ACE_INET_Addr local_addr(port, ia_any);

  if (iiop_endpoint->is_preferred_network ())
    local_addr.set (port,
                    iiop_endpoint->preferred_network ());
#if defined (ACE_HAS_IPV6)
  else if (remote_address.get_type () == AF_INET6)
    local_addr.set (port,
                    ACE_IPV6_ANY);
#endif /* ACE_HAS_IPV6 */

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - IIOP_Connector::begin_connection, ")
                ACE_TEXT ("to <%s:%d> which should %s\n"),
                ACE_TEXT_CHAR_TO_TCHAR(iiop_endpoint->host()),
                iiop_endpoint->port(),
                r->blocked_connect () ? ACE_TEXT("block") : ACE_TEXT("nonblock")));

  // Get the right synch options
  ACE_Synch_Options synch_options;

  this->active_connect_strategy_->synch_options (timeout,
                                                 synch_options);

  // If we don't need to block for a transport just set the timeout to
  // be zero.
  ACE_Time_Value tmp_zero (ACE_Time_Value::zero);
  if (!r->blocked_connect ())
    {
      synch_options.timeout (ACE_Time_Value::zero);
      timeout = &tmp_zero;
    }

  svc_handler = 0;

  int result =
    this->base_connector_.connect (svc_handler,
                                   remote_address,
                                   synch_options,
                                   local_addr);

  // The connect() method creates the service handler and bumps the
  // #REFCOUNT# up one extra.  There are four possibilities from
  // calling connect(): (a) connection succeeds immediately - in this
  // case, the #REFCOUNT# on the handler is two; (b) connection
  // completion is pending - in this case, the #REFCOUNT# on the
  // handler is also two; (c) connection fails immediately - in this
  // case, the #REFCOUNT# on the handler is one since close() gets
  // called on the handler; (d) the connect immediately returns when we
  // have specified that it shouldn't block.
  //
  // The extra reference count in
  // TAO_Connect_Creation_Strategy::make_svc_handler() is needed in
  // the case when connection completion is pending and we are going
  // to wait on a variable in the handler to changes, signifying
  // success or failure.  Note, that this increment cannot be done
  // once the connect() returns since this might be too late if
  // another thread pick up the completion and potentially deletes the
  // handler before we get a chance to increment the reference count.
  return result;
}

TAO_Transport *
TAO_IIOP_Connector::complete_connection (int result,
                                         TAO_Transport_Descriptor_Interface &desc,
                                         TAO_IIOP_Connection_Handler **&sh_list,
                                         TAO_IIOP_Endpoint **ep_list,
                                         unsigned count,
                                         TAO::Profile_Transport_Resolver *r,
                                         TAO_LF_Multi_Event *mev,
                                         ACE_Time_Value *timeout)
{
  // Make sure that we always do a remove_reference for every member
  // of the list
  TAO_IIOP_Connection_Handler_Array_Guard svc_handler_auto_ptr (sh_list,count);
  TAO_Transport *transport  = 0;
  TAO_Transport **tlist = 0;
  ACE_NEW_RETURN (tlist,TAO_Transport*[count],0);

  //  populate the transport list
  for (unsigned i = 0; i < count; i++)
    tlist[i] = sh_list[i]->transport();

  if (result != -1)
    {
      // We received a compeleted connection and 0 or more pending.
      // the winner is the last member of the list, because the
      // iterator stopped on a successful connect.
      transport = tlist[count-1];
    }
  else
    {
      if (count == 1)
        {
          transport = tlist[0];
          if (!this->wait_for_connection_completion (r,
                                                     transport,
                                                     timeout))
            {
              if (TAO_debug_level > 2)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) - IIOP_Connector::")
                            ACE_TEXT ("complete_connection, wait for completion ")
                            ACE_TEXT ("failed for 1 pending connect\n")));
            }
        }
      else
        {
          if (!this->wait_for_connection_completion (r,
                                                     transport,
                                                     tlist,
                                                     count,
                                                     mev,
                                                     timeout))
            {
              if (TAO_debug_level > 2)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) - IIOP_Connector::")
                            ACE_TEXT ("complete_connection, wait for completion ")
                            ACE_TEXT ("failed for %d pending connects\n"),
                            count));
            }
        }
    }
  // At this point, the connection has be successfully created
  // connected or not connected, but we have a connection.
  TAO_IIOP_Connection_Handler *svc_handler = 0;
  TAO_IIOP_Endpoint *iiop_endpoint = 0;

  if (transport != 0)
    {
      for (unsigned i = 0; i < count; i++)
        {
          if (transport == tlist[i])
            {
              svc_handler = sh_list[i];
              if (transport->connection_handler()->keep_waiting())
                {
                  svc_handler->add_reference();
                }
              iiop_endpoint = ep_list[i];
              break;
            }
        }
    }


  // Done with the transport list. It was a temporary that did not
  // affect the reference count.
  delete [] tlist;

  // In case of errors transport is zero
  if (transport == 0)
    {
      // Give users a clue to the problem.
      if (TAO_debug_level > 3)
        {
          for (unsigned i = 0; i < count; i++)
          ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) IIOP_Connector::make_connection, ")
                        ACE_TEXT("connection to <%s:%d> failed (%p)\n"),
                        ACE_TEXT_CHAR_TO_TCHAR (ep_list[i]->host ()),
                        ep_list[i]->port (),
                      ACE_TEXT("errno")));
        }

      return 0;
    }


  if (TAO_debug_level > 2)
    {
    ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - IIOP_Connector::make_connection, ")
                  ACE_TEXT ("new %s connection to <%s:%d> on Transport[%d]\n"),
                  transport->is_connected() ?
                  ACE_TEXT("connected") : ACE_TEXT("not connected"),
                  ACE_TEXT_CHAR_TO_TCHAR (iiop_endpoint->host ()),
                iiop_endpoint->port (),
                svc_handler->peer ().get_handle ()));
    }

  // Add the handler to Cache
  int retval = -1;
  if (count == 1U || desc.reset_endpoint(iiop_endpoint))
    {
      retval = this->orb_core ()->
        lane_resources ().transport_cache ().cache_transport (&desc,
                                                              transport);
    }

  // Failure in adding to cache.
  if (retval != 0)
    {
      // Close the handler.
      svc_handler->close ();

      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) IIOP_Connector::make_connection, ")
                      ACE_TEXT ("could not add new connection to cache\n")));
        }

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
                    ACE_TEXT ("(%P|%t) IIOP_Connector [%d]::make_connection, ")
                    ACE_TEXT ("could not register the transport ")
                    ACE_TEXT ("in the reactor.\n"),
                    transport->id ()));

      return 0;
    }

  return transport;
}

TAO_Profile *
TAO_IIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile = 0;
  ACE_NEW_RETURN (pfile,
                  TAO_IIOP_Profile (this->orb_core ()),
                  0);

  const int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO_IIOP_Connector::make_profile (ACE_ENV_SINGLE_ARG_DECL)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_IIOP_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return profile;
}

int
TAO_IIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  static const char *protocol[] = { "iiop", "iioploc" };

  const size_t slot = ACE_OS::strchr (endpoint, ':') - endpoint;
  if (slot == 0) // an empty string is valid for corbaloc.
    return 0;

  const size_t len0 = ACE_OS::strlen (protocol[0]);
  const size_t len1 = ACE_OS::strlen (protocol[1]);

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

TAO_IIOP_Endpoint *
TAO_IIOP_Connector::remote_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != IOP::TAG_INTERNET_IOP)
    return 0;

  TAO_IIOP_Endpoint *iiop_endpoint =
    dynamic_cast<TAO_IIOP_Endpoint *> (endpoint );

  if (iiop_endpoint == 0)
    return 0;

  return iiop_endpoint;
}

int
TAO_IIOP_Connector::cancel_svc_handler (
  TAO_Connection_Handler * svc_handler)
{
  TAO_IIOP_Connection_Handler* handler=
    dynamic_cast<TAO_IIOP_Connection_Handler*>(svc_handler);

  // Cancel from the connector
  if (handler)
    {
      handler->abort();
      return this->base_connector_.cancel (handler);
    }

  return -1;
}


//@@ TAO_CONNECTOR_SPL_COPY_HOOK_END

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
