// $Id$

#include "orbsvcs/HTIOP/HTIOP_Transport.h"

#include "orbsvcs/HTIOP/HTIOP_Connection_Handler.h"
#include "orbsvcs/HTIOP/HTIOP_Acceptor.h"
#include "orbsvcs/HTIOP/HTIOP_Profile.h"
#include "ace/HTBP/HTBP_Session.h"

#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/operation_details.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Adapter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::HTIOP::Transport::Transport (TAO::HTIOP::Connection_Handler *h,
                                  TAO_ORB_Core *orb_core)
  : TAO_Transport (OCI_TAG_HTIOP_PROFILE, orb_core),
    connection_handler_ (h)
{
}

TAO::HTIOP::Transport::~Transport (void)
{
}

ACE_Event_Handler *
TAO::HTIOP::Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO::HTIOP::Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

ssize_t
TAO::HTIOP::Transport::send (iovec *iov, int iovcnt,
                            size_t &bytes_transferred,
                            const ACE_Time_Value *max_wait_time)
{
  ssize_t retval = this->connection_handler_->peer ().sendv (iov, iovcnt,
                                                             max_wait_time);
  if (retval > 0)
    {
      bytes_transferred = retval;
    }

  return retval;
}

ssize_t
TAO::HTIOP::Transport::recv (char *buf,
                             size_t len,
                             const ACE_Time_Value *max_wait_time)
{
  ssize_t n = this->connection_handler_->peer ().recv (buf,
                                                       len,
                                                       max_wait_time);

  // Do not print the error message if it is a timeout, which could
  // occur in thread-per-connection.
  if (n == -1 &&
      TAO_debug_level > 4 &&
      errno != ETIME)
    {

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - TAO::HTIOP::Transport[%d]::recv_i, ")
                  ACE_TEXT ("read failure - %m"),
                  this->id ()));
    }

  // Error handling
  if (n == -1)
    {
      if (errno == EWOULDBLOCK)
        return 0;


      return -1;
    }

  // Most of the errors handling is common for
  // Now the message has been read

  // @@ What are the other error handling here??
  else if (n == 0)
    {
      return -1;
    }

  return n;
}

int
TAO::HTIOP::Transport::register_handler (void)
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - TAO::HTIOP::Transport[%d]::register_handler\n"),
                  this->id ()));
    }

  ACE_Reactor *r = this->orb_core_->reactor ();
  if (r == this->connection_handler_->reactor ())
    return 0;

  // Set the flag in the Connection Handler and in the Wait Strategy
  // @@Maybe we should set these flags after registering with the
  // reactor. What if the  registration fails???
  this->ws_->is_registered (1);

  ACE::HTBP::Session *s = this->connection_handler_->peer().session();
  if (s)
    s->reactor(r);
  ACE::HTBP::Channel *ch = s->inbound();
  return r->register_handler (ch->notifier(),
                              ACE_Event_Handler::READ_MASK);
}


int
TAO::HTIOP::Transport::send_request (TAO_Stub *stub,
                                     TAO_ORB_Core *orb_core,
                                     TAO_OutputCDR &stream,
                                     TAO_Message_Semantics message_semantics,
                                     ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  message_semantics) == -1)
    {
      return -1;
    }

  if (this->send_message (stream,
                          stub,
                          0,
                          message_semantics,
                          max_wait_time) == -1)
    {
      return -1;
    }

  this->first_request_sent();

  return this->idle_after_send ();
}

int
TAO::HTIOP::Transport::send_message (TAO_OutputCDR &stream,
                                     TAO_Stub *stub,
                                     TAO_ServerRequest *request,
                                     TAO_Message_Semantics message_semantics,
                                     ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object ()->format_message (stream, stub, request) != 0)
    {
      return -1;
    }

  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = this->send_message_shared (stub,
                                         message_semantics,
                                         stream.begin (),
                                         max_wait_time);

  if (n == -1)
    {
      // Dont try to be smart and request for %p in the debug
      // statement.  If the event handler is destroyed the transport
      // would return -1 with errno set to ENOENT. %p then would dump
      // a core. %m would then be softer on this.
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - TAO::HTIOP::Transport[%d]::send_message, ")
                      ACE_TEXT (" write failure - %m\n"),
                      this->id ()));
        }

      return -1;
    }

  return 1;
}

int
TAO::HTIOP::Transport::tear_listen_point_list (TAO_InputCDR &cdr)
{
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;

  cdr.reset_byte_order (static_cast<int> (byte_order));

  ::HTIOP::ListenPointList listen_list;
  if ((cdr >> listen_list) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"tear_listen_point_list: no list\n"),-1);
  //return -1;

  // As we have received a bidirectional information, set the flag to
  // 1 (i.e., non-originating side)
  this->bidirectional_flag (0);

  return this->connection_handler_->process_listen_point_list (listen_list);
}

void
TAO::HTIOP::Transport::set_bidir_context_info (TAO_Operation_Details &opdetails)
{
  // Get a handle to the acceptor registry
  TAO_Acceptor_Registry &ar =
    this->orb_core ()->lane_resources ().acceptor_registry ();

  // Get the first acceptor in the registry
  TAO_AcceptorSetIterator acceptor = ar.begin ();

  ::HTIOP::ListenPointList listen_point_list;

  for (;
       acceptor != ar.end ();
       acceptor++)
    {
      // Check whether it is a HTIOP acceptor
      if ((*acceptor)->tag () == this->tag ())
        {
          if (this->get_listen_point (listen_point_list,
                                      *acceptor) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("TAO (%P|%t) - TAO::HTIOP::Transport::set_bidir_info, "),
                          ACE_TEXT("error getting listen_point\n")));

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
TAO::HTIOP::Transport::get_listen_point (::HTIOP::ListenPointList &lp_list,
                                         TAO_Acceptor *acceptor)
{
  TAO::HTIOP::Acceptor *htiop_acceptor =
    dynamic_cast<TAO::HTIOP::Acceptor *> (acceptor );

  // Get the array of endpoints serviced by TAO::HTIOP::Acceptor
  const ACE::HTBP::Addr *endpoint_addr =
    htiop_acceptor->endpoints ();

  // Get the endpoint count
  size_t count =
    htiop_acceptor->endpoint_count ();

  // Get the local address of the connection
  ACE::HTBP::Addr local_addr;
  {
    // Just make sure that the connection handler is sane before we go
    // head and do anything with it.
    ACE_GUARD_RETURN (ACE_Lock,
                      ace_mon,
                      *this->handler_lock_,
                      -1);

    if (this->connection_handler_->peer ().get_local_addr (local_addr)
        == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Could not resolve local ")
                           ACE_TEXT ("host address in ")
                           ACE_TEXT ("get_listen_point()\n")),
                          -1);
      }
  }

  if (local_addr.get_port_number() != 0)
    {

      // Note: Looks like there is no point in sending the list of
      // endpoints on interfaces on which this connection has not
      // been established. If this is wrong, please correct me.
      CORBA::String_var local_interface;

      // Get the hostname for the local address
      if (htiop_acceptor->hostname (this->orb_core_,
                                    local_addr,
                                    local_interface.out ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) Could not resolve local host")
                             ACE_TEXT (" name\n")),
                            -1);
        }

      for (size_t index = 0;
           index != count;
           index++)
        {
          if (local_addr.is_ip_equal (endpoint_addr[index]))
            {
              // Get the count of the number of elements
              CORBA::ULong len = lp_list.length ();

              // Increase the length by 1
              lp_list.length (len + 1);

              // We have the connection and the acceptor endpoint on the
              // same interface
              ::HTIOP::ListenPoint &point = lp_list[len];
              point.host = CORBA::string_dup (local_interface.in ());
              point.port = endpoint_addr[index].get_port_number ();
            }
        }
    }
  else
    {
      // Only add a single listen point based on the htid in the addr
      lp_list.length (1);
      ::HTIOP::ListenPoint &point = lp_list[0];
      point.host = CORBA::string_dup ("");
      point.port = 0;
      point.htid = endpoint_addr[0].get_htid();
    }
  return 1;
}


TAO_Connection_Handler *
TAO::HTIOP::Transport::invalidate_event_handler_i (void)
{
  TAO_Connection_Handler * eh = this->connection_handler_;
  this->connection_handler_ = 0;
  return eh;
}

TAO_END_VERSIONED_NAMESPACE_DECL
