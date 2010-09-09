// $Id$

#include "orbsvcs/HTIOP/HTIOP_Connection_Handler.h"

#include "orbsvcs/HTIOP/HTIOP_Transport.h"
#include "orbsvcs/HTIOP/HTIOP_Endpoint.h"

#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Session.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/CDR.h"
#include "tao/Server_Strategy_Factory.h"

#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Resume_Handle.h"
#include "tao/Protocols_Hooks.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::HTIOP::Connection_Handler::Connection_Handler (ACE_Thread_Manager *t)
  : SVC_HANDLER (t,0,0),
    TAO_Connection_Handler (0)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core () != 0);
}

TAO::HTIOP::Connection_Handler::Connection_Handler (TAO_ORB_Core *orb_core)
  : SVC_HANDLER (orb_core->thr_mgr (), 0, 0),
    TAO_Connection_Handler (orb_core)
{
  TAO::HTIOP::Transport* specific_transport = 0;
  ACE_NEW(specific_transport,
          TAO::HTIOP::Transport (this, orb_core));

  this->transport (specific_transport);
}

TAO::HTIOP::Connection_Handler::~Connection_Handler (void)
{
  delete this->transport ();
  int const result =
    this->release_os_resources ();

  if (result == -1 && TAO_debug_level)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - HTIOP_Connection_Handler::")
                  ACE_TEXT("~HTIOP_Connection_Handler, ")
                  ACE_TEXT("release_os_resources() failed %m\n")));
    }
}

int
TAO::HTIOP::Connection_Handler::open_handler (void *v)
{
  return this->open (v);
}

int
TAO::HTIOP::Connection_Handler::open (void*)
{
  if (this->shared_open() == -1)
    return -1;

  if (this->transport ()->wait_strategy ()->non_blocking ())
    {
      if (this->peer ().enable (ACE_NONBLOCK) == -1)
        return -1;
    }

  // Called by the <Strategy_Acceptor> when the handler is
  // completely connected.

  ACE::HTBP::Addr remote_addr;
  if (this->peer ().get_remote_addr (remote_addr) == -1)
    return -1;

  ACE::HTBP::Addr local_addr;
  if (this->peer ().get_local_addr (local_addr) == -1)
    return -1;

  if (local_addr.is_ip_equal (remote_addr)
      && local_addr.get_port_number () == remote_addr.get_port_number ())
    {

      if (TAO_debug_level > 0)
        {
          ACE_TCHAR remote_as_string[MAXHOSTNAMELEN + 16];
          ACE_TCHAR local_as_string[MAXHOSTNAMELEN + 16];

          (void) remote_addr.addr_to_string (remote_as_string,
                                             sizeof(remote_as_string),
                                             0);
          (void) local_addr.addr_to_string (local_as_string,
                                            sizeof(local_as_string),
                                            0);
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("TAO(%P|%t) - TAO::HTIOP::Connection_Handler::open, ")
                      ACE_TEXT("Holy Cow! The remote addr and ")
                      ACE_TEXT("local addr are identical (%s == %s)\n"),
                      remote_as_string, local_as_string));
        }
      return -1;
    }

  if (TAO_debug_level > 0)
    {
      ACE_TCHAR client[MAXHOSTNAMELEN + 16];

      // Verify that we can resolve the peer hostname.
      if (remote_addr.addr_to_string (client, sizeof (client), 0) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) TAO_HTIOP connection to peer ")
                  ACE_TEXT ("<%s> on %d\n"),
                  client, this->peer ().get_handle ()));
    }

  // Set the id in the transport now that we're active.
  // Use C-style cast b/c otherwise we get warnings on lots of compilers
  if (!this->transport ()->post_open ((size_t) this->get_handle ()))
    return -1;

  this->state_changed (TAO_LF_Event::LFS_SUCCESS,
                       this->orb_core ()->leader_follower ());

  return 0;
}

int
TAO::HTIOP::Connection_Handler::resume_handler (void)
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO::HTIOP::Connection_Handler::close_connection (void)
{
  return this->close_connection_eh (this);
}

int
TAO::HTIOP::Connection_Handler::handle_input (ACE_HANDLE h)
{
  // Figure out if the peer is associated with an identified session.
  // If not, do whatever is necessary to complete the linkage.
  ACE::HTBP::Session *session = peer().session();
  if (session == 0)
    {
      int result = this->handle_input_eh (h,this);

      if (result != -1 && peer().session())
        {
          if (TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("TAO::HTIOP::Connection_Handler::handle_input: ")
                        ACE_TEXT("now binding to %d\n"),
                        peer().get_handle()));
        }
      return result;
    }
  return this->handle_input_eh (h, this);
}

int
TAO::HTIOP::Connection_Handler::handle_output (ACE_HANDLE handle)
{
  int result =
    this->handle_output_eh (handle, this);

  if (result == -1)
    {
      this->close_connection ();
      return 0;
    }

  return result;
}

int
TAO::HTIOP::Connection_Handler::handle_close (ACE_HANDLE ,
                                        ACE_Reactor_Mask )
{
  ACE_ASSERT (0);
  return 0;
}

int
TAO::HTIOP::Connection_Handler::close (u_long)
{
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED,
                       this->orb_core ()->leader_follower ());
  this->transport ()->remove_reference ();
  return 0;
}

int
TAO::HTIOP::Connection_Handler::release_os_resources (void)
{
  int result = this->peer().close ();
  return result;
}

int
TAO::HTIOP::Connection_Handler::add_transport_to_cache (void)
{
  ACE::HTBP::Addr addr;

  // Get the peername.
  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  // Construct an  TAO::HTIOP::Endpoint object
  TAO::HTIOP::Endpoint endpoint (addr,
                               this->orb_core()->
                               orb_params()->use_dotted_decimal_addresses ());

  // Construct a property object
  TAO_Base_Transport_Property prop (&endpoint);

  TAO::Transport_Cache_Manager &cache =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Idle the transport..
  return cache.cache_transport (&prop, this->transport ());
}

int
TAO::HTIOP::Connection_Handler::process_listen_point_list
(::HTIOP::ListenPointList &listen_list)
{
  // Get the size of the list
  CORBA::ULong len = listen_list.length ();

  for (CORBA::ULong i = 0; i < len; ++ i)
    {
      ::HTIOP::ListenPoint listen_point = listen_list[i];
      ACE::HTBP::Addr addr;
      if (listen_point.port)
        addr.ACE_INET_Addr::set (listen_point.port,
                  listen_point.host.in ());
      else
        addr.set_htid (listen_point.htid);

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("(%P|%t) Listening port [%d] on [%C],[%C]\n"),
                      listen_point.port,
                      listen_point.host.in (),
                      listen_point.htid.in()));
        }

      // Construct an  TAO::HTIOP::Endpoint object
      TAO::HTIOP::Endpoint endpoint (addr,
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

int
TAO::HTIOP::Connection_Handler::set_dscp_codepoint (CORBA::Long /*dscp*/)
{
  return 0;
}

int
TAO::HTIOP::Connection_Handler::set_dscp_codepoint (
  CORBA::Boolean /*enable_network_priority*/)
{
  return 0;
}

int
TAO::HTIOP::Connection_Handler::handle_write_ready (const ACE_Time_Value *t)
{
  return ACE::handle_write_ready (this->peer ().get_handle (), t);
}

TAO_END_VERSIONED_NAMESPACE_DECL
