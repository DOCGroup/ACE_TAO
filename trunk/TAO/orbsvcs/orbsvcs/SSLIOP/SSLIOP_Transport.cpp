#include "SSLIOP_Transport.h"
#include "SSLIOP_Connection_Handler.h"
#include "SSLIOP_Profile.h"
#include "SSLIOP_Acceptor.h"

#include "tao/debug.h"

#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Sync_Strategies.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/Acceptor_Registry.h"


ACE_RCSID (SSLIOP,
           SSLIOP_Transport,
           "$Id$")


TAO::SSLIOP::Transport::Transport (
  TAO::SSLIOP::Connection_Handler *handler,
  TAO_ORB_Core *orb_core,
  CORBA::Boolean /* flag */)
  : TAO_Transport (IOP::TAG_INTERNET_IOP, orb_core),
    connection_handler_ (handler),
    messaging_object_ (0)
{
  // Use the normal GIOP object
  ACE_NEW (this->messaging_object_,
           TAO_GIOP_Message_Base (orb_core));
}

TAO::SSLIOP::Transport::~Transport (void)
{
  delete this->messaging_object_;
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

TAO_Pluggable_Messaging *
TAO::SSLIOP::Transport::messaging_object (void)
{
  return this->messaging_object_;
}

int
TAO::SSLIOP::Transport::handle_input (TAO_Resume_Handle &rh,
                                      ACE_Time_Value *max_wait_time,
                                      int block)
{
  int result = 0;

  // Set up the SSLIOP::Current object.
  TAO::SSLIOP::State_Guard ssl_state_guard (this->connection_handler_,
                                            result);

  if (result == -1)
    return -1;

  return TAO_Transport::handle_input (rh,
                                      max_wait_time,
                                      block);
}

ssize_t
TAO::SSLIOP::Transport::send (iovec *iov,
                              int iovcnt,
                              size_t &bytes_transferred,
                              const ACE_Time_Value *max_wait_time)
{
  const ssize_t retval =
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
  const ssize_t n = this->connection_handler_->peer ().recv (buf,
                                                             len,
                                                             max_wait_time);

  // Most of the errors handling is common for
  // Now the message has been read
  if (n == -1
      && TAO_debug_level > 4
      && errno != ETIME)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p \n"),
                  ACE_TEXT ("TAO - read message failure ")
                  ACE_TEXT ("recv_i () \n")));
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

  return 0;
}

int
TAO::SSLIOP::Transport::send_message (TAO_OutputCDR &stream,
                                      TAO_Stub *stub,
                                      int message_semantics,
                                      ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object_->format_message (stream) != 0)
    return -1;

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  // This guarantees to send all data (bytes) or return an error.
  const ssize_t n = this->send_message_shared (stub,
                                               message_semantics,
                                               stream.begin (),
                                               max_wait_time);

  if (n == -1)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO: (%P|%t|%N|%l) closing transport ")
                    ACE_TEXT ("%d after fault %p\n"),
                    this->id (),
                    ACE_TEXT ("send_message ()\n")));

      return -1;
    }

  return 1;
}


int
TAO::SSLIOP::Transport::generate_request_header (
  TAO_Operation_Details &opdetails,
  TAO_Target_Specification &spec,
  TAO_OutputCDR &msg)
{
  // Check whether we have a Bi Dir IIOP policy set, whether the
  // messaging objects are ready to handle bidirectional connections
  // and also make sure that we have not recd. or sent any information
  // regarding this before...
  if (this->orb_core ()->bidir_giop_policy ()
      && this->messaging_object_->is_ready_for_bidirectional (msg)
      && this->bidirectional_flag () < 0)
    {
      this->set_bidir_context_info (opdetails);

      // Set the flag to 0
     this->bidirectional_flag (0);
    }

  // Modify the request id if we have BiDirectional client/server
  // setup
  opdetails.modify_request_id (this->bidirectional_flag ());

  // We are going to pass on this request to the underlying messaging
  // layer. It should take care of this request
  return TAO_Transport::generate_request_header (opdetails,
                                                 spec,
                                                 msg);
}

int
TAO::SSLIOP::Transport::messaging_init (CORBA::Octet major,
                                        CORBA::Octet minor)
{
  this->messaging_object_->init (major,
                                 minor);
  return 1;
}


int
TAO::SSLIOP::Transport::tear_listen_point_list (TAO_InputCDR &cdr)
{
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;

  cdr.reset_byte_order (static_cast<int> (byte_order));

  IIOP::ListenPointList listen_list;
  if ((cdr >> listen_list) == 0)
    return -1;

  // As we have received a bidirectional information, set the flag to
  // 1
  this->bidirectional_flag (1);

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
      if ((*acceptor)->tag () == IOP::TAG_INTERNET_IOP)
        {
          if (this->get_listen_point (listen_point_list,
                                      *acceptor) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "TAO (%P|%t) - SSLIOP_Transport::set_bidir_info, ",
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
  const ACE_INET_Addr *endpoint_addr =
    ssliop_acceptor->endpoints ();

  // Get the count
  const size_t count =
    ssliop_acceptor->endpoint_count ();

  // The SSL port is stored in the SSLIOP::SSL component associated
  // with the SSLIOP_Acceptor.
  const ::SSLIOP::SSL &ssl = ssliop_acceptor->ssl_component ();

  // Get the local address of the connection
  ACE_INET_Addr local_addr;
  {
    if (this->connection_handler_->peer ().get_local_addr (local_addr)
        == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
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
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Could not resolve local host")
                         ACE_TEXT (" name \n")),
                        -1);
    }

  for (size_t index = 0; index < count; ++index)
    {
      if (local_addr.get_ip_address ()
          == endpoint_addr[index].get_ip_address ())
        {
          // Get the count of the number of elements
          const CORBA::ULong len = listen_point_list.length ();

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
