#include "IIOP_Transport.h"
#include "IIOP_Connection_Handler.h"
#include "IIOP_Acceptor.h"
#include "IIOP_Profile.h"
#include "Acceptor_Registry.h"
#include "Thread_Lane_Resources.h"
#include "operation_details.h"
#include "Timeprobe.h"
#include "CDR.h"
#include "Transport_Mux_Strategy.h"
#include "Wait_Strategy.h"
#include "Sync_Strategies.h"
#include "ORB_Core.h"
#include "debug.h"
#include "GIOP_Message_Base.h"
#include "GIOP_Message_Lite.h"
#include "Protocols_Hooks.h"
#include "Adapter.h"

#if !defined (__ACE_INLINE__)
# include "IIOP_Transport.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           IIOP_Transport,
           "$Id$")


TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Connection_Handler *handler,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Boolean flag)
  : TAO_Transport (IOP::TAG_INTERNET_IOP,
                   orb_core)
  , connection_handler_ (handler)
  , messaging_object_ (0)
{
  if (connection_handler_ != 0)
    {
      // REFCNT: Matches one of
      // TAO_Transport::connection_handler_close() or
      // TAO_Transport::close_connection_shared.
      this->connection_handler_->incr_refcount();
    }
  if (flag)
    {
      // Use the lite version of the protocol
      ACE_NEW (this->messaging_object_,
               TAO_GIOP_Message_Lite (orb_core));
    }
  else
    {
      // Use the normal GIOP object
      ACE_NEW (this->messaging_object_,
               TAO_GIOP_Message_Base (orb_core));
    }
}

TAO_IIOP_Transport::~TAO_IIOP_Transport (void)
{
  ACE_ASSERT(this->connection_handler_ == 0);
  delete this->messaging_object_;
}

ACE_Event_Handler *
TAO_IIOP_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO_IIOP_Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Pluggable_Messaging *
TAO_IIOP_Transport::messaging_object (void)
{
  return this->messaging_object_;
}

ssize_t
TAO_IIOP_Transport::send_i (iovec *iov, int iovcnt,
                            size_t &bytes_transferred,
                            const ACE_Time_Value *max_wait_time)
{
  ssize_t retval = this->connection_handler_->peer ().sendv (iov, iovcnt,
                                                             max_wait_time);
  if (retval > 0)
    bytes_transferred = retval;

  return retval;
}

ssize_t
TAO_IIOP_Transport::recv_i (char *buf,
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
                  ACE_TEXT ("TAO (%P|%t) - IIOP_Transport[%d]::recv_i, ")
                  ACE_TEXT ("read failure - %m\n"),
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
TAO_IIOP_Transport::register_handler_i (void)
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - IIOP_Transport[%d]::register_handler\n",
                  this->id ()));
    }

  ACE_Reactor *r = this->orb_core_->reactor ();

  if (r == this->connection_handler_->reactor ())
    return 0;

  // Set the flag in the Connection Handler and in the Wait Strategy
  // @@Maybe we should set these flags after registering with the
  // reactor. What if the  registration fails???
  this->ws_->is_registered (1);

  // Register the handler with the reactor
  return  r->register_handler (this->connection_handler_,
                               ACE_Event_Handler::READ_MASK);
}


int
TAO_IIOP_Transport::send_request (TAO_Stub *stub,
                                  TAO_ORB_Core *orb_core,
                                  TAO_OutputCDR &stream,
                                  int message_semantics,
                                  ACE_Time_Value *max_wait_time)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_Protocols_Hooks *tph =
        this->orb_core_->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (tph != 0)
        {
          int result =
            tph->update_client_protocol_properties (stub,
                                                    this,
                                                    "iiop");

          if (result == -1)
            return -1;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "TAO (%P|%t) - TAO_IIOP_Transport::send_request - "
                             "get_protocol_hooks");

      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  if (this->ws_->sending_request (orb_core,
                                  message_semantics) == -1)

    return -1;

  if (this->send_message (stream,
                          stub,
                          message_semantics,
                          max_wait_time) == -1)
    return -1;

  this->first_request_sent();

  return this->idle_after_send ();
}

int
TAO_IIOP_Transport::send_message (TAO_OutputCDR &stream,
                                  TAO_Stub *stub,
                                  int message_semantics,
                                  ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object_->format_message (stream) != 0)
    return -1;

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
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - IIOP_Transport[%d]::send_message, ")
                    ACE_TEXT (" write failure - %m\n"),
                    this->id ()));
      return -1;
    }

  return 1;
}

int
TAO_IIOP_Transport::send_message_shared (
  TAO_Stub *stub,
  int message_semantics,
  const ACE_Message_Block *message_block,
  ACE_Time_Value *max_wait_time)
{
  int r;

  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

    if (this->check_event_handler_i ("IIOP_Transport::send_message_shared") == -1)
      return -1;

    if (TAO_debug_level > 6)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("IIOP_Transport::send_message_shared, ")
                  ACE_TEXT ("enable_network_priority = %d\n"),
                  this->connection_handler_->enable_network_priority ()));

    if (this->connection_handler_ != 0)
      this->connection_handler_->set_dscp_codepoint ();

    r = this->send_message_shared_i (stub, message_semantics,
                                     message_block, max_wait_time);
  }

  if (r == -1)
    {
      this->close_connection ();
    }

  return r;
}

int
TAO_IIOP_Transport::generate_request_header (TAO_Operation_Details &opdetails,
                                             TAO_Target_Specification &spec,
                                             TAO_OutputCDR &msg)
{
  // Check whether we have a Bi Dir IIOP policy set, whether the
  // messaging objects are ready to handle bidirectional connections
  // and also make sure that we have not recd. or sent any information
  // regarding this before...
  if (this->orb_core ()->bidir_giop_policy () &&
      this->messaging_object_->is_ready_for_bidirectional (msg) &&
      this->bidirectional_flag () < 0)
    {
      this->set_bidir_context_info (opdetails);

      // Set the flag to 0  (i.e., originating side)
      this->bidirectional_flag (0);
    }

  // Modify the request id if we have BiDirectional client/server
  // setup
  opdetails.modify_request_id (this->bidirectional_flag ());


  return TAO_Transport::generate_request_header (opdetails,
                                                 spec,
                                                 msg);
}

int
TAO_IIOP_Transport::messaging_init (CORBA::Octet major,
                                    CORBA::Octet minor)
{
  this->messaging_object_->init (major, minor);

  return 1;
}

int
TAO_IIOP_Transport::tear_listen_point_list (TAO_InputCDR &cdr)
{
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;

  cdr.reset_byte_order (ACE_static_cast (int, byte_order));

  IIOP::ListenPointList listen_list;
  if ((cdr >> listen_list) == 0)
    return -1;

  // As we have received a bidirectional information, set the flag to
  // 1 (i.e., non-originating side)
  this->bidirectional_flag (1);

  // Just make sure that the connection handler is sane before we go
  // head and do anything with it.
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->handler_lock_,
                    -1);

  if (this->check_event_handler_i ("IIOP_Transport::tear_listen_point_list")
      == -1)
    return -1;

  return this->connection_handler_->process_listen_point_list (listen_list);
}

void
TAO_IIOP_Transport::set_bidir_context_info (TAO_Operation_Details &opdetails)
{
  // Get a handle to the acceptor registry
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
      if ((*acceptor)->tag () == IOP::TAG_INTERNET_IOP)
        {
          if (this->get_listen_point (listen_point_list,
                                      *acceptor) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "TAO (%P|%t) - IIOP_Transport::set_bidir_info, "
                          "error getting listen_point \n"));

              return;
            }
        }
    }

  // We have the ListenPointList at this point. Create a output CDR
  // stream at this point
  TAO_OutputCDR cdr;

  // Marshall the information into the stream
  if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER) == 0)
      || (cdr << listen_point_list) == 0)
    return;

  // Add this info in to the svc_list
  opdetails.request_service_context ().set_context (IOP::BI_DIR_IIOP,
                                                    cdr);

  return;
}

int
TAO_IIOP_Transport::get_listen_point (
    IIOP::ListenPointList &listen_point_list,
    TAO_Acceptor *acceptor)
{
  TAO_IIOP_Acceptor *iiop_acceptor =
    ACE_dynamic_cast (TAO_IIOP_Acceptor *,
                      acceptor );

  // Get the array of endpoints serviced by TAO_IIOP_Acceptor
  const ACE_INET_Addr *endpoint_addr =
    iiop_acceptor->endpoints ();

  // Get the endpoint count
  size_t count =
    iiop_acceptor->endpoint_count ();

  // Get the local address of the connection
  ACE_INET_Addr local_addr;
  {
    // Just make sure that the connection handler is sane before we go
    // head and do anything with it.
    ACE_GUARD_RETURN (ACE_Lock,
                      ace_mon,
                      *this->handler_lock_,
                      -1);

    if (this->check_event_handler_i ("IIOP_Transport::get_listen_point")
        == -1)
      return -1;

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

  // Note: Looks like there is no point in sending the list of
  // endpoints on interfaces on which this connection has not
  // been established. If this is wrong, please correct me.
  CORBA::String_var local_interface;

  // Get the hostname for the local address
  if (iiop_acceptor->hostname (this->orb_core_,
                               local_addr,
                               local_interface.out ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Could not resolve local host")
                         ACE_TEXT (" name \n")),
                        -1);
    }

  for (size_t index = 0;
       index != count;
       index++)
    {
      if (local_addr.get_ip_address()
          == endpoint_addr[index].get_ip_address())
        {
          // Get the count of the number of elements
          CORBA::ULong len = listen_point_list.length ();

          // Increase the length by 1
          listen_point_list.length (len + 1);

          // We have the connection and the acceptor endpoint on the
          // same interface
          IIOP::ListenPoint &point = listen_point_list[len];
          point.host = CORBA::string_dup (local_interface.in ());
          point.port = endpoint_addr[index].get_port_number ();
        }
    }

  return 1;
}

TAO_Connection_Handler *
TAO_IIOP_Transport::invalidate_event_handler_i (void)
{
  TAO_Connection_Handler * eh = this->connection_handler_;
  this->connection_handler_ = 0;
  return eh;
}

void
TAO_IIOP_Transport::update_protocol_properties (int snd_buf_sz,
                                                int rcv_buf_sz,
                                                int no_delay,
                                                int enable_nw_prio)
{
  // Just make sure that the connection handler is sane before we go
  // head and do anything with it.
  ACE_GUARD (ACE_Lock,
             ace_mon,
             *this->handler_lock_);

  if (this->check_event_handler_i (
          "IIOP_Transport::update_protocol_properties") == -1)
    return;

  /// Making an outbound call holding a lock. This is not suposed to
  /// be done. But it would be dangerous to leave the lock since the
  /// connection handler could dissappear.
  this->connection_handler_->update_protocol_properties (
      snd_buf_sz,
      rcv_buf_sz,
      no_delay,
      enable_nw_prio);
}
