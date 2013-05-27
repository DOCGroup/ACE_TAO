// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.h"
#include "orbsvcs/SSLIOP/SSLIOP_Transport.h"
#include "orbsvcs/SSLIOP/SSLIOP_Profile.h"
#include "orbsvcs/SSLIOP/SSLIOP_Acceptor.h"

#include "tao/debug.h"

#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Transport::Transport (
  TAO::SSLIOP::Connection_Handler *handler,
  TAO_ORB_Core *orb_core)
  : TAO_Transport (IOP::TAG_INTERNET_IOP, orb_core),
    connection_handler_ (handler)
{
}

TAO::SSLIOP::Transport::~Transport (void)
{
}

ACE_Event_Handler *
TAO::SSLIOP::Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO::SSLIOP::Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

int
TAO::SSLIOP::Transport::handle_input (TAO_Resume_Handle &rh,
                                      ACE_Time_Value *max_wait_time)
{
  int result = 0;

  // Set up the SSLIOP::Current object.
  TAO::SSLIOP::State_Guard ssl_state_guard (this->connection_handler_, result);

  if (result == -1)
    return -1;

  return TAO_Transport::handle_input (rh, max_wait_time);
}

ssize_t
TAO::SSLIOP::Transport::send (iovec *iov,
                              int iovcnt,
                              size_t &bytes_transferred,
                              const ACE_Time_Value *max_wait_time)
{
  ssize_t const retval =
    this->connection_handler_->peer ().sendv (iov, iovcnt, max_wait_time);

  if (retval > 0)
    bytes_transferred = retval;

  return retval;
}

ssize_t
TAO::SSLIOP::Transport::recv (char *buf,
                              size_t len,
                              const ACE_Time_Value *max_wait_time)
{
  ssize_t const n = this->connection_handler_->peer ().recv (buf,
                                                             len,
                                                             max_wait_time);

  // Most of the errors handling is common for
  // Now the message has been read
  if (n == -1 && TAO_debug_level > 4 && errno != ETIME)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - SSLIOP_Transport[%d]::recv, ")
                  ACE_TEXT ("read failure - %m errno %d\n"),
                  this->id (),
                  errno));
    }

  // Error handling
  if (n == -1)
    {
      if (errno == EWOULDBLOCK)
        return 0;

      return -1;
    }
  // @@ What are the other error handling here??
  else if (n == 0)
    {
      return -1;
    }

  return n;
}

int
TAO::SSLIOP::Transport::send_request (TAO_Stub *stub,
                                      TAO_ORB_Core *orb_core,
                                      TAO_OutputCDR &stream,
                                      TAO_Message_Semantics message_semantics,
                                      ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core, message_semantics) == -1)
    return -1;

  if (this->send_message (stream,
                          stub,
                          0,
                          message_semantics,
                          max_wait_time) == -1)

    return -1;

  return 0;
}

int
TAO::SSLIOP::Transport::send_message (TAO_OutputCDR &stream,
                                      TAO_Stub *stub,
                                      TAO_ServerRequest *request,
                                      TAO_Message_Semantics message_semantics,
                                      ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object ()->format_message (stream, stub, request) != 0)
    return -1;

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  // This guarantees to send all data (bytes) or return an error.
  ssize_t const n = this->send_message_shared (stub,
                                               message_semantics,
                                               stream.begin (),
                                               max_wait_time);

  if (n == -1)
    {
      if (TAO_debug_level)
        ORBSVCS_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - SSLIOP_Transport::send_message, ")
                    ACE_TEXT ("closing transport %d after fault %p\n"),
                    this->id (),
                    ACE_TEXT ("send_message ()\n")));

      return -1;
    }

  return 1;
}

int
TAO::SSLIOP::Transport::tear_listen_point_list (TAO_InputCDR &cdr)
{
  CORBA::Boolean byte_order;
  if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
    return -1;

  cdr.reset_byte_order (static_cast<int> (byte_order));

  IIOP::ListenPointList listen_list;
  if (!(cdr >> listen_list))
    return -1;

  // As we have received a bidirectional information, set the flag to
  // 0
  this->bidirectional_flag (0);

  return this->connection_handler_->process_listen_point_list (listen_list);
}



void
TAO::SSLIOP::Transport::set_bidir_context_info (
  TAO_Operation_Details &opdetails)
{
  // Get a handle on to the acceptor registry
  TAO_Acceptor_Registry &ar =
    this->orb_core ()->lane_resources ().acceptor_registry ();

  // Get the first acceptor in the registry
  TAO_AcceptorSetIterator acceptor = ar.begin ();

  IIOP::ListenPointList listen_point_list;

  for (;
       acceptor != ar.end ();
       acceptor++)
    {
      // Check whether it is a IIOP acceptor
      if ((*acceptor)->tag () == this->tag ())
        {
          if (this->get_listen_point (listen_point_list, *acceptor) == -1)
            {
              ORBSVCS_ERROR ((LM_ERROR,
                          "TAO (%P|%t) - SSLIOP_Transport::set_bidir_info, ",
                          "error getting listen_point\n"));
              return;
            }
        }
    }

  // We have the ListenPointList at this point. Create a output CDR
  // stream at this point
  TAO_OutputCDR cdr;

  // Marshall the information into the stream
  if (!(cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
      || !(cdr << listen_point_list))
    return;

  // Add this info in to the svc_list
  opdetails.request_service_context ().set_context (IOP::BI_DIR_IIOP, cdr);
  return;
}


int
TAO::SSLIOP::Transport::get_listen_point (
  IIOP::ListenPointList &listen_point_list,
  TAO_Acceptor *acceptor)
{
  TAO::SSLIOP::Acceptor *ssliop_acceptor =
    dynamic_cast<TAO::SSLIOP::Acceptor *> (acceptor);

  if (ssliop_acceptor == 0)
    return -1;

  // Get the array of IIOP (not SSLIOP!) endpoints serviced by the
  // SSLIOP_Acceptor.
  const ACE_INET_Addr *endpoint_addr = ssliop_acceptor->endpoints ();

  // Get the count
  size_t const count = ssliop_acceptor->endpoint_count ();

  // The SSL port is stored in the SSLIOP::SSL component associated
  // with the SSLIOP_Acceptor.
  const ::SSLIOP::SSL &ssl = ssliop_acceptor->ssl_component ();

  // Get the local address of the connection
  ACE_INET_Addr local_addr;
  {
    if (this->connection_handler_->peer ().get_local_addr (local_addr) == -1)
      {
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Could not resolve local host")
                           ACE_TEXT (" address in get_listen_point()\n")),
                        -1);
      }

  }

  // Note: Looks like there is no point in sending the list of
  // endpoints on interfaces on which this connection has not
  // been established. If this is wrong, please correct me.
  CORBA::String_var local_interface;

  // Get the hostname for the local address
  if (ssliop_acceptor->hostname (this->orb_core_,
                                 local_addr,
                                 local_interface.out ()) == -1)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Could not resolve local host")
                         ACE_TEXT (" name\n")),
                        -1);
    }

#if defined (ACE_HAS_IPV6)
  // If this is an IPv6 decimal linklocal address containing a scopeid than
  // remove the scopeid from the information being sent.
  const char *cp_scope = 0;
  if (local_addr.get_type () == PF_INET6 &&
        (cp_scope = ACE_OS::strchr (local_interface.in (), '%')) != 0)
    {
      CORBA::ULong len = cp_scope - local_interface.in ();
      local_interface[len] = '\0';
    }
#endif /* ACE_HAS_IPV6 */

  for (size_t index = 0; index < count; ++index)
    {
      // Make sure port numbers are equal so the following comparison
      // only concerns the IP(v4/v6) address.
      local_addr.set_port_number (endpoint_addr[index].get_port_number ());

      if (local_addr == endpoint_addr[index])
        {
          // Get the count of the number of elements
          CORBA::ULong const len = listen_point_list.length ();

          // Increase the length by 1
          listen_point_list.length (len + 1);

          // We have the connection and the acceptor endpoint on the
          // same interface
          IIOP::ListenPoint & point = listen_point_list[len];
          point.host = CORBA::string_dup (local_interface.in ());

          // All endpoints, if more than one, serviced by the
          // SSLIOP_Acceptor should be listening on the same port (due
          // to the bind to the INADDR_ANY address).
          point.port = ssl.port;
        }
    }

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
