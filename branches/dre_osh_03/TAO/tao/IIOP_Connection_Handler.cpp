#include "tao/IIOP_Connection_Handler.h"
#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/IIOP_Transport.h"
#include "tao/IIOP_Endpoint.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Resume_Handle.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Wait_Strategy.h"

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Connection_Handler.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           IIOP_Connection_Handler,
           "$Id$")


TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_IIOP_SVC_HANDLER (t, 0 , 0),
    TAO_Connection_Handler (0),
    dscp_codepoint_ (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core () != 0);
}


TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (
  TAO_ORB_Core *orb_core,
  CORBA::Boolean flag,
  void *arg)
  : TAO_IIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    tcp_properties_ (*(ACE_static_cast
                       (TAO_IIOP_Properties *, arg))),
    dscp_codepoint_ (0)
{
  TAO_IIOP_Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
           TAO_IIOP_Transport (this, orb_core, flag));

  // store this pointer (indirectly increment ref count)
  this->transport (specific_transport);
  TAO_Transport::release (specific_transport);
}

TAO_IIOP_Connection_Handler::TAO_IIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                                          void *arg)
  : TAO_IIOP_SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core),
    tcp_properties_ (*(ACE_static_cast
                       (TAO_IIOP_Properties *, arg))),
    dscp_codepoint_ (0)
{
}


TAO_IIOP_Connection_Handler::~TAO_IIOP_Connection_Handler (void)
{
}

int
TAO_IIOP_Connection_Handler::open (void*)
{
  if (this->set_socket_option (this->peer (),
                               this->tcp_properties_.send_buffer_size,
                               this->tcp_properties_.recv_buffer_size) == -1)
    return -1;

#if !defined (ACE_LACKS_TCP_NODELAY)

  if (this->peer ().set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                (void *) &tcp_properties_.no_delay,
                                sizeof (int)) == -1)
    return -1;
#endif /* ! ACE_LACKS_TCP_NODELAY */

  if (this->transport ()->wait_strategy ()->non_blocking ())
    {
      if (this->peer ().enable (ACE_NONBLOCK) == -1)
        return -1;
    }

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.

  ACE_INET_Addr remote_addr;
  if (this->peer ().get_remote_addr (remote_addr) == -1)
    return -1;

  ACE_INET_Addr local_addr;
  if (this->peer ().get_local_addr (local_addr) == -1)
    return -1;

  if (local_addr.get_ip_address () == remote_addr.get_ip_address ()
      && local_addr.get_port_number () == remote_addr.get_port_number ())
    {
      if (TAO_debug_level > 0)
        {
          ACE_TCHAR remote_as_string[MAXHOSTNAMELEN + 16];
          ACE_TCHAR local_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_addr.addr_to_string (remote_as_string,
                                             sizeof(remote_as_string));
          (void) local_addr.addr_to_string (local_as_string,
                                            sizeof(local_as_string));
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT("TAO(%P|%t) - TAO_IIOP_Connection_Handler::open, ")
                      ACE_LIB_TEXT("Holy Cow! The remote addr and ")
                      ACE_LIB_TEXT("local addr are identical (%s == %s)\n"),
                      remote_as_string, local_as_string));
        }
      return -1;
    }


  if (TAO_debug_level > 0)
    {
      ACE_TCHAR client[MAXHOSTNAMELEN + 16];

      // Verify that we can resolve the peer hostname.
      if (remote_addr.addr_to_string (client, sizeof (client)) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("TAO (%P|%t) - Connection_Handler::open, IIOP ")
                  ACE_LIB_TEXT ("connection to peer <%s> on %d\n"),
                  client, this->peer ().get_handle ()));
    }

  // Set the id in the transport now that we're active.
  // Use C-style cast b/c otherwise we get warnings on lots of compilers
  this->transport ()->id ((size_t) this->get_handle ());

  this->state_changed (TAO_LF_Event::LFS_SUCCESS);

  return 0;
}

int
TAO_IIOP_Connection_Handler::activate (long flags,
                                       int n_threads,
                                       int force_active,
                                       long priority,
                                       int grp_id,
                                       ACE_Task_Base *task,
                                       ACE_hthread_t thread_handles[],
                                       void *stack[],
                                       size_t stack_size[],
                                       ACE_thread_t  thread_names[])
{
  if (TAO_debug_level)
    ACE_DEBUG  ((LM_DEBUG,
                 ACE_LIB_TEXT ("TAO (%P|%t) - IIOP_Connection_Handler::")
                 ACE_LIB_TEXT ("activate %d threads, flags = %d\n"),
                 n_threads,
                 flags,
                 THR_BOUND));

  // Set the id in the transport now that we're active.
  // Use C-style cast b/c otherwise we get warnings on lots of compilers
  this->transport ()->id ((size_t) this->get_handle ());

  return TAO_IIOP_SVC_HANDLER::activate (flags,
                                         n_threads,
                                         force_active,
                                         priority,
                                         grp_id,
                                         task,
                                         thread_handles,
                                         stack,
                                         stack_size,
                                         thread_names);
}

int
TAO_IIOP_Connection_Handler::svc (void)
{
  // This method is called when an instance is "activated", i.e.,
  // turned into an active object.  Presumably, activation spawns a
  // thread with this method as the "worker function".

  // Clear the non-blocking mode here
  ACE_Flag_Manip::clr_flags (this->get_handle (),
                             ACE_NONBLOCK);

  // Call the implementation here
  return this->svc_i ();
}

int
TAO_IIOP_Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO_IIOP_Connection_Handler::close_connection (void)
{
  return this->close_connection_eh (this);
}

int
TAO_IIOP_Connection_Handler::handle_input (ACE_HANDLE h)
{
  return this->handle_input_eh (h, this);
}

int
TAO_IIOP_Connection_Handler::handle_output (ACE_HANDLE handle)
{
  return this->handle_output_eh (handle, this);
}

int
TAO_IIOP_Connection_Handler::handle_close (ACE_HANDLE handle,
                                           ACE_Reactor_Mask rm)
{
  return this->handle_close_eh (handle, rm, this);
}

int
TAO_IIOP_Connection_Handler::release_os_resources (void)
{
  return this->peer().close ();
}

int
TAO_IIOP_Connection_Handler::add_transport_to_cache (void)
{
  ACE_INET_Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  IIOP_Endpoint object
  TAO_IIOP_Endpoint endpoint (
      addr,
      this->orb_core()->orb_params()->use_dotted_decimal_addresses ());

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  TAO_Transport_Cache_Manager &cache =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Idle the transport..
  return cache.cache_idle_transport (&prop,
                                     this->transport ());
}

int
TAO_IIOP_Connection_Handler::process_listen_point_list (
    IIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  CORBA::ULong len = listen_list.length ();

  for (CORBA::ULong i = 0; i < len; ++ i)
    {
      IIOP::ListenPoint listen_point = listen_list[i];
      ACE_INET_Addr addr (listen_point.port,
                          listen_point.host.in ());

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("(%P|%t) Listening port [%d] on [%s]\n"),
                      listen_point.port,
                      ACE_TEXT_CHAR_TO_TCHAR(listen_point.host.in ())));
        }

      // Construct an  IIOP_Endpoint object
      TAO_IIOP_Endpoint endpoint (addr,
        this->orb_core()->orb_params()->use_dotted_decimal_addresses ());

      // Construct a property object
      TAO_Base_Transport_Property prop (&endpoint);

      // Mark the connection as bidirectional
      prop.set_bidir_flag (1);

      // The property for this handler has changed. Recache the
      // handler with this property
      int retval = this->transport ()->recache_transport (&prop);
      if (retval == -1)
        return retval;

      // Make the handler idle and ready for use
      this->transport ()->make_idle ();
    }

  return 0;
}

void
TAO_IIOP_Connection_Handler::update_protocol_properties (
  int send_buffer_size,
  int recv_buffer_size,
  int no_delay,
  int enable_network_priority)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_IIOP_Connection_Handler::update_protocol_properties\n"
                "enable_network_priority = %d\n",
                enable_network_priority));

  if (this->tcp_properties_.send_buffer_size != send_buffer_size)
    this->tcp_properties_.send_buffer_size = send_buffer_size;

  if (this->tcp_properties_.recv_buffer_size != recv_buffer_size)
    this->tcp_properties_.recv_buffer_size = recv_buffer_size;

  if (this->tcp_properties_.no_delay != no_delay)
    this->tcp_properties_.no_delay = no_delay;

  if (this->tcp_properties_.enable_network_priority != enable_network_priority)
    this->tcp_properties_.enable_network_priority = enable_network_priority;

}

int
TAO_IIOP_Connection_Handler::enable_network_priority (void)
{
  return this->tcp_properties_.enable_network_priority;
}

int
TAO_IIOP_Connection_Handler::set_dscp_codepoint (void)
{
  int tos;
  if (this->enable_network_priority ())
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          TAO_Protocols_Hooks *tph =
            this->orb_core ()->get_protocols_hooks (
              ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (tph != 0)
            {
              CORBA::Long codepoint =
                tph->get_dscp_codepoint ();

              tos = (int)(codepoint) << 2;
            }
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "TAO_IIOP_Connection_Handler::"
                                 "set_dscp_codepoint - "
                                 "get_protocol_hooks");

          return -1;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);
    }
  else
    tos = IPDSFIELD_DSCP_DEFAULT << 2;

  if (tos != this->dscp_codepoint_)
    {
      int ret = this->peer ().set_option (IPPROTO_IP,
                                          IP_TOS,
                                          (int *) &tos ,
                                          (int) sizeof (tos));

      if(TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - IIOP_Connection_Handler::"
                      "set_dscp_codepoint, failed to set Diffserv"
                      " codepoint - try running as superuser\n"));

          ACE_DEBUG((LM_DEBUG,
                     "TAO (%P|%t) - IIOP_Connection_Handler::"
                     "set_dscp_codepoint, set tos: ret: %d %x\n",
                     ret, tos));
        }

      this->dscp_codepoint_ = tos;
    }

  return 0;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
