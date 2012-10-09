// $Id$

#include "tao/IIOP_Transport.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Connection_Handler *handler,
                                        TAO_ORB_Core *orb_core)
  : TAO_Transport (IOP::TAG_INTERNET_IOP,
                   orb_core)
  , connection_handler_ (handler)
{
}

TAO_IIOP_Transport::~TAO_IIOP_Transport (void)
{
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
                      ACE_TEXT ("send failure (errno: %d) - %m\n"),
                      this->id (), ACE_ERRNO_GET));
        }
    }

  return retval;
}

#if TAO_HAS_SENDFILE == 1
ssize_t
TAO_IIOP_Transport::sendfile (TAO_MMAP_Allocator * allocator,
                              iovec * iov,
                              int iovcnt,
                              size_t &bytes_transferred,
                              TAO::Transport::Drain_Constraints const & dc)
{
  // @@ We should probably set the TCP_CORK socket option to minimize
  //    network operations.  It may also be useful to adjust the
  //    socket send buffer size accordingly.

  // If we don't have an allocator, fallback to the regular way of sending
  // data
  if (allocator == 0)
    return this->send (iov, iovcnt, bytes_transferred, this->io_timeout(dc));

  // We can only use sendfile when all data is coming from the mmap allocator,
  // if not, we just fallback to to the regular way of sending data
  iovec * const off_check_begin = iov;
  iovec * const off_check_end   = iov + iovcnt;
  for (iovec * index = off_check_begin; index != off_check_end; ++index)
    {
      if (-1 == allocator->offset (index->iov_base))
        return this->send (iov, iovcnt, bytes_transferred,
                           this->io_timeout(dc));
    }

  ssize_t retval = -1;

  ACE_HANDLE const in_fd = allocator->handle ();

  if (in_fd == ACE_INVALID_HANDLE)
    return retval;

  ACE_HANDLE const out_fd =
    this->connection_handler_->peer ().get_handle ();

  iovec * const begin = iov;
  iovec * const end   = iov + iovcnt;
  for (iovec * i = begin; i != end; ++i)
    {
      off_t offset = allocator->offset (i->iov_base);

      if (this->io_timeout(dc))
        {
          int val = 0;
          if (ACE::enter_send_timedwait (out_fd,
                                         this->io_timeout(dc), val) == -1)
            return retval;
          else
            {
              retval =
                ACE_OS::sendfile (out_fd, in_fd, &offset, i->iov_len);
              ACE::restore_non_blocking_mode (out_fd, val);
            }
        }
      else
        {
          retval = ACE_OS::sendfile (out_fd, in_fd, &offset, i->iov_len);
        }

      if (retval <= 0)  // Report errors below.
        break;

      bytes_transferred += static_cast<size_t> (retval);
    }

  if (retval <= 0 && TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - IIOP_Transport[%d]::sendfile, ")
                  ACE_TEXT ("sendfile failure - %m (errno: %d)\n"),
                  this->id (),
                  ACE_ERRNO_GET));
    }

  return retval;
}
#endif  /* TAO_HAS_SENDFILE==1 */

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
  if (n == -1 && TAO_debug_level > 4 && errno != ETIME)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - IIOP_Transport[%d]::recv, ")
                  ACE_TEXT ("read failure - %m errno %d\n"),
                  this->id (),
                  ACE_ERRNO_GET));
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
                                  TAO_Message_Semantics message_semantics,
                                  ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core, message_semantics) == -1)
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

  return 0;
}

int
TAO_IIOP_Transport::send_message (TAO_OutputCDR &stream,
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
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - IIOP_Transport[%d]::send_message, ")
                      ACE_TEXT ("write failure - %m\n"),
                      this->id ()));
        }
      return -1;
    }

  return 1;
}

int
TAO_IIOP_Transport::tear_listen_point_list (TAO_InputCDR &cdr)
{
  CORBA::Boolean byte_order;
  if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
    return -1;

  cdr.reset_byte_order (static_cast<int> (byte_order));

  IIOP::ListenPointList listen_list;
  if (!(cdr >> listen_list))
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

  TAO_AcceptorSetIterator const end = ar.end ();

  for (TAO_AcceptorSetIterator acceptor = ar.begin ();
       acceptor != end;
       ++acceptor)
    {
      // Check whether it is an IIOP acceptor
      if ((*acceptor)->tag () == this->tag ())
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
  if (!(cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
      || (!(cdr << listen_point_list)))
    return;

  // Add this info in to the svc_list
  opdetails.request_service_context ().set_context (IOP::BI_DIR_IIOP, cdr);

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
  size_t count = iiop_acceptor->endpoint_count ();

#if defined (TAO_USE_BROKEN_BIDIR)

  // Note: Looks like there is no point in sending the list of
  // endpoints on interfaces on which this connection has not
  // been established. If this is wrong, please correct me.

  ACE_INET_Addr local_addr;
  if (this->connection_handler_->peer ().get_local_addr (local_addr)
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - IIOP_Transport::get_listen_point, ")
                         ACE_TEXT ("could not resolve local host address\n")),
                        -1);
    }
#endif /* TAO_USE_BROKEN_BIDIR */

  for (size_t index = 0; index < count; index++)
    {
#if defined (TAO_USE_BROKEN_BIDIR)
      // Make sure port numbers are equal so the following comparison
      // only concerns the IP(v4/v6) address.
      local_addr.set_port_number (endpoint_addr[index].get_port_number ());

      if (local_addr != endpoint_addr[index])
        continue;
#endif /* TAO_USE_BROKEN_BIDIR */

      // Get the local address of the connection
      CORBA::String_var interface_name;

      // Get the hostname for the local address
      if (iiop_acceptor->hostname (this->orb_core_,
                                   endpoint_addr[index],
                                   interface_name.out ()) == -1)
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
      if (endpoint_addr[index].get_type () == PF_INET6 &&
          (cp_scope = ACE_OS::strchr (interface_name.in (), '%')) != 0)
        {
          CORBA::ULong len = cp_scope - interface_name.in ();
          interface_name[len] = '\0';
        }
#endif /* ACE_HAS_IPV6 */

      // Get the count of the number of elements
      CORBA::ULong const len = listen_point_list.length ();

      // Increase the length by 1
      listen_point_list.length (len + 1);

      // We have the connection and the acceptor endpoint on the
      // same interface
      IIOP::ListenPoint & point = listen_point_list[len];
      point.host = CORBA::string_dup (interface_name.in ());
      point.port = endpoint_addr[index].get_port_number ();

      if (TAO_debug_level >= 5)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Listen_Point_List[%d] = <%C:%d>\n"),
                      len,
                      point.host.in (),
                      point.port));
        }
    }
  return 1;
}
//@@ TAO_TRANSPORT_SPL_COPY_HOOK_END
/*
 * End of copy hook.
 */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */
