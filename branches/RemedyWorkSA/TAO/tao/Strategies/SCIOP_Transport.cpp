// $Id$

#include "tao/Strategies/SCIOP_Transport.h"

#if TAO_HAS_SCIOP == 1

#include "tao/Strategies/SCIOP_Connection_Handler.h"
#include "tao/Strategies/SCIOP_Acceptor.h"
#include "tao/Strategies/SCIOP_Profile.h"
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

TAO_SCIOP_Transport::TAO_SCIOP_Transport (TAO_SCIOP_Connection_Handler *handler,
                                          TAO_ORB_Core *orb_core)
  : TAO_Transport (TAO_TAG_SCIOP_PROFILE,
                   orb_core)
  , connection_handler_ (handler)
{
}

TAO_SCIOP_Transport::~TAO_SCIOP_Transport (void)
{
}

ACE_Event_Handler *
TAO_SCIOP_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO_SCIOP_Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

ssize_t
TAO_SCIOP_Transport::send (iovec *iov, int iovcnt,
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
TAO_SCIOP_Transport::recv (char *buf,
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
                  ACE_TEXT ("TAO (%P|%t) - SCIOP_Transport[%d]::recv, ")
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
TAO_SCIOP_Transport::send_request (TAO_Stub *stub,
                                   TAO_ORB_Core *orb_core,
                                   TAO_OutputCDR &stream,
                                   TAO_Message_Semantics message_semantics,
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
TAO_SCIOP_Transport::send_message (TAO_OutputCDR &stream,
                                   TAO_Stub *stub,
                                   TAO_Message_Semantics message_semantics,
                                   ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object ()->format_message (stream, stub) != 0)
    return -1;

  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = this->send_message_shared (stub,
                                         message_semantics,
                                         stream.begin (),
                                         max_wait_time);

  if (n == -1)
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - SCIOP_Transport[%d]::send_message, ")
                    ACE_TEXT (" write failure - %m\n"),
                    this->id ()));
      return -1;
    }

  return 1;
}

int
TAO_SCIOP_Transport::tear_listen_point_list (TAO_InputCDR &cdr)
{
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;

  cdr.reset_byte_order (static_cast<int> (byte_order));

  IIOP::ListenPointList listen_list;
  if (!(cdr >> listen_list))
    return -1;

  // As we have received a bidirectional information, set the flag to
  // 1 (i.e., non-originating side)
  this->bidirectional_flag (0);

  return this->connection_handler_->process_listen_point_list (listen_list);
}

void
TAO_SCIOP_Transport::set_bidir_context_info (TAO_Operation_Details &opdetails)
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
      // Check whether it is a SCIOP acceptor
      if ((*acceptor)->tag () == this->tag ())
        {
          if (this->get_listen_point (listen_point_list,
                                      *acceptor) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "TAO (%P|%t) - SCIOP_Transport::set_bidir_info, "
                          "error getting listen_point\n"));

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
  opdetails.request_service_context ().set_context (IOP::BI_DIR_IIOP, cdr);
}

int
TAO_SCIOP_Transport::get_listen_point (
    IIOP::ListenPointList &listen_point_list,
    TAO_Acceptor *acceptor)
{
  TAO_SCIOP_Acceptor *sciop_acceptor =
    dynamic_cast<TAO_SCIOP_Acceptor *> (acceptor );

  // Get the array of endpoints serviced by TAO_SCIOP_Acceptor
  const ACE_INET_Addr *endpoint_addr =
    sciop_acceptor->endpoints ();

  // Get the endpoint count
  size_t count =
    sciop_acceptor->endpoint_count ();

  // Get the local address of the connection
  ACE_INET_Addr local_addr;

  if (this->connection_handler_->peer ().get_local_addr (local_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Could not resolve local ")
                         ACE_TEXT ("host address in ")
                         ACE_TEXT ("get_listen_point()\n")),
                        -1);
    }

  // Note: Looks like there is no point in sending the list of
  // endpoints on interfaces on which this connection has not
  // been established. If this is wrong, please correct me.
  CORBA::String_var local_interface;

  // Get the hostname for the local address
  if (sciop_acceptor->hostname (this->orb_core_,
                               local_addr,
                               local_interface.out ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Could not resolve local host")
                         ACE_TEXT (" name\n")),
                        -1);
    }

  for (size_t index = 0; index != count; index++)
    {
      if (local_addr.get_ip_address() == endpoint_addr[index].get_ip_address())
        {
          // Get the count of the number of elements
          CORBA::ULong const len = listen_point_list.length ();

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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SCIOP == 1 */
