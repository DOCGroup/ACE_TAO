#include "tao/IIOP_Transport.h"
#include "tao/IIOP_Acceptor.h"
#include "tao/IIOPC.h"
#include "tao/Acceptor_Registry.h"
#include "tao/operation_details.h"
#include "tao/Wait_Strategy.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/Protocols_Hooks.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/MMAP_Allocator.h"

#include "ace/OS_NS_sys_sendfile.h"

ACE_RCSID (tao,
           IIOP_Transport,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Connection_Handler *handler,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Boolean )
  : TAO_Transport (IOP::TAG_INTERNET_IOP,
                   orb_core)
  , connection_handler_ (handler)
  , messaging_object_ (0)
{
  // Use the normal GIOP object
  ACE_NEW (this->messaging_object_,
           TAO_GIOP_Message_Base (orb_core));
}

TAO_IIOP_Transport::~TAO_IIOP_Transport (void)
{
  delete this->messaging_object_;
}

/*
 * Hook to copy over all concrete implementations
 * of Transport class from this class to the base
 * class as a part of the specialization.
 * All enhancements to the IIOP_Transport
 * class, i.e., addition of new concrete non virtual
 * methods should be added within this hook.
 */

//@@ TAO_TRANSPORT_SPL_COPY_HOOK_START
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
TAO_IIOP_Transport::send (iovec *iov, int iovcnt,
                          size_t &bytes_transferred,
                          const ACE_Time_Value *max_wait_time)
{
  ssize_t const retval =
    this->connection_handler_->peer ().sendv (iov,
                                              iovcnt,
                                              max_wait_time);

  if (retval > 0)
    bytes_transferred = retval;
  else
    {
      if (TAO_debug_level > 4)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - IIOP_Transport[%d]::send, ")
                      ACE_TEXT ("send failure - %m (errno: %d)\n"),
                      this->id (), errno));
        }
    }

  return retval;
}

#ifdef ACE_HAS_SENDFILE
ssize_t
TAO_IIOP_Transport::sendfile (ACE_Message_Block * data,
                              size_t & bytes_transferred,
                              ACE_Time_Value const * timeout)
{
  // @@ We should probably set the TCP_CORK socket option to minimize
  //    network operations.  It may also be useful to adjust the
  //    socket send buffer size accordingly.

  ssize_t retval = -1;

  for (ACE_Message_Block const * message_block = data;
       message_block != 0;
       message_block = message_block->cont ())
    {
      size_t const mb_length = message_block->length ();

      // Check if this block has any data to be sent.
      if (mb_length > 0)
        {
          // @@ How costly is this dynamic_cast<>.
          TAO_MMAP_Allocator * const allocator =
            dynamic_cast<TAO_MMAP_Allocator *> (
              message_block->data_block ()->allocator_strategy ());

          if (allocator == 0)
            return 0;  // Can't use sendfile().

          ACE_HANDLE const in_fd = allocator->handle ();

          if (in_fd == ACE_INVALID_HANDLE)
            return retval;

          ACE_HANDLE const out_fd =
            this->connection_handler_->peer ().get_handle ();

          ACE_LOFF_T offset =
            allocator->offset (message_block->rd_ptr ());

          if (timeout)
            {
              int val = 0;
              if (ACE::enter_send_timedwait (out_fd, timeout, val) == -1)
                return retval;
              else
                {
                  retval =
                    ACE_OS::sendfile (out_fd, in_fd, &offset, mb_length);
                  ACE::restore_non_blocking_mode (out_fd, val);

                }
            }
          else
            {
              retval = ACE_OS::sendfile (out_fd, in_fd, &offset, mb_length);
            }

          if (retval <= 0)  // Report errors below.
            break;
        }
    }

  if (retval > 0)
    bytes_transferred = static_cast<size_t> (retval);
  else if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - IIOP_Transport[%d]::sendfile, ")
                  ACE_TEXT ("sendfile failure - %m (errno: %d)\n"),
                  this->id (), errno));
    }


  return retval;
}
#endif  /* ACE_HAS_SENDFILE */

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          const ACE_Time_Value *max_wait_time)
{
  ssize_t const n = this->connection_handler_->peer ().recv (buf,
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
TAO_IIOP_Transport::send_request (TAO_Stub *stub,
                                  TAO_ORB_Core *orb_core,
                                  TAO_OutputCDR &stream,
                                  int message_semantics,
                                  ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  message_semantics) == -1)

    return -1;

  if (this->send_message (stream,
                          stub,
                          message_semantics,
                          max_wait_time) == -1)
    return -1;

  this->first_request_sent();

  return 0;
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
  ssize_t const n = this->send_message_shared (stub,
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
                    ACE_TEXT ("write failure - %m\n"),
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

      // Set the flag to 1 (i.e., originating side)
      this->bidirectional_flag (1);

      // At the moment we enable BiDIR giop we have to get a new
      // request id to make sure that we follow the even/odd rule
      // for request id's. We only need to do this when enabled
      // it, after that the Transport Mux Strategy will make sure
      // that the rule is followed
      opdetails.request_id (this->tms ()->request_id ());
    }

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

  cdr.reset_byte_order (static_cast<int> (byte_order));

  IIOP::ListenPointList listen_list;
  if ((cdr >> listen_list) == 0)
    return -1;

  // As we have received a bidirectional information, set the flag to
  // 0 (i.e., non-originating side)
  this->bidirectional_flag (0);

  return this->connection_handler_->process_listen_point_list (listen_list);
}

void
TAO_IIOP_Transport::set_bidir_context_info (TAO_Operation_Details &opdetails)
{
  // Get a handle to the acceptor registry
  TAO_Acceptor_Registry &ar =
    this->orb_core ()->lane_resources ().acceptor_registry ();

  IIOP::ListenPointList listen_point_list;

  const TAO_AcceptorSetIterator end = ar.end ();

  for (TAO_AcceptorSetIterator acceptor = ar.begin ();
       acceptor != end;
       ++acceptor)
    {
      // Check whether it is an IIOP acceptor
      if ((*acceptor)->tag () == IOP::TAG_INTERNET_IOP)
        {
          if (this->get_listen_point (listen_point_list, *acceptor) == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "TAO (%P|%t) - IIOP_Transport::set_bidir_context_info, "
                            "error getting listen_point\n"));

              return;
            }
        }
    }

  if (listen_point_list.length () == 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - IIOP_Transport::set_bidir_context_info, "
                    "listen_point list is empty, client should send a list "
                    "with at least one point\n"));

      return;
    }

  // We have the ListenPointList at this point. Create a output CDR
  // stream at this point
  TAO_OutputCDR cdr;

  // Marshal the information into the stream
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
    dynamic_cast<TAO_IIOP_Acceptor *> (acceptor);

  if (iiop_acceptor == 0)
    return -1;

  // Get the array of endpoints serviced by TAO_IIOP_Acceptor
  const ACE_INET_Addr *endpoint_addr =
    iiop_acceptor->endpoints ();

  // Get the endpoint count
  size_t const count =
    iiop_acceptor->endpoint_count ();

  // Get the local address of the connection
  ACE_INET_Addr local_addr;

  if (this->connection_handler_->peer ().get_local_addr (local_addr)
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - IIOP_Transport::get_listen_point, ")
                         ACE_TEXT ("could not resolve local host address\n")),
                        -1);
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
                         ACE_TEXT ("TAO (%P|%t) - IIOP_Transport::get_listen_point, ")
                         ACE_TEXT ("could not resolve local host name\n")),
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

  for (size_t index = 0;
       index < count;
       ++index)
    {
      // Make sure port numbers are equal so the following comparison
      // only concerns the IP(v4/v6) address.
      local_addr.set_port_number (endpoint_addr[index].get_port_number ());

      if (local_addr == endpoint_addr[index])
        {
          // Get the count of the number of elements
          const CORBA::ULong len = listen_point_list.length ();

          // Increase the length by 1
          listen_point_list.length (len + 1);

          // We have the connection and the acceptor endpoint on the
          // same interface
          IIOP::ListenPoint & point = listen_point_list[len];
          point.host = CORBA::string_dup (local_interface.in ());
          point.port = endpoint_addr[index].get_port_number ();

          if (TAO_debug_level >= 5)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO (%P:%t) Listen_Point_List[%d] = <%s:%d>"),
                        len,
                        point.host.in (),
                        point.port));
          }

        }
    }

  return 1;
}
//@@ TAO_TRANSPORT_SPL_COPY_HOOK_END
/*
 * End of copy hook.
 */

TAO_END_VERSIONED_NAMESPACE_DECL
