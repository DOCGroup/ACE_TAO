// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/IIOP_Transport.h"
#include "tao/IIOP_Connection_Handler.h"
#include "tao/IIOP_Acceptor.h"
#include "tao/IIOP_Profile.h"
#include "tao/Acceptor_Registry.h"
#include "tao/operation_details.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Sync_Strategies.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/GIOP_Message_Lite.h"

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Transport.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao, IIOP_Transport, "$Id$")

TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Connection_Handler *handler,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Boolean flag)
  : TAO_Transport (TAO_TAG_IIOP_PROFILE,
                   orb_core)
  , connection_handler_ (handler)
  , messaging_object_ (0)
{
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
  delete this->messaging_object_;
}

TAO_IIOP_SVC_HANDLER *
TAO_IIOP_Transport::service_handler (void)
{
  return this->connection_handler_;
}

ACE_HANDLE
TAO_IIOP_Transport::handle (void)
{
  return this->connection_handler_->get_handle ();
}

ACE_Event_Handler *
TAO_IIOP_Transport::event_handler (void)
{
  return this->connection_handler_;
}

void
TAO_IIOP_Transport::close_connection (void)
{
  // First close the handle
  this->connection_handler_->handle_close ();

  // Now, purge the entry
  this->connection_handler_->purge_entry ();
}

int
TAO_IIOP_Transport::idle (void)
{
  return this->connection_handler_->make_idle ();
}

ssize_t
TAO_IIOP_Transport::send (const ACE_Message_Block *message_block,
                          const ACE_Time_Value *max_wait_time,
                          size_t *bytes_transferred)
{
  return ACE::send_n (this->handle (),
                      message_block,
                      max_wait_time,
                      bytes_transferred);
}

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          const ACE_Time_Value *max_wait_time)
{
  return this->service_handler ()->peer ().recv (buf,
                                                 len,
                                                 max_wait_time);
}


int
TAO_IIOP_Transport::read_process_message (ACE_Time_Value *max_wait_time,
                                          int block)
{
  // Read the message of the socket
  int result =  this->messaging_object_->read_message (this,
                                                       block,
                                                       max_wait_time);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %p\n"),
                    ACE_TEXT ("IIOP_Transport::read_message, failure in read_message ()")));

      this->tms_->connection_closed ();
      return -1;
    }
  if (result == 0)
    return result;

  // Now we know that we have been able to read the complete message
  // here.. We loop here to see whether we have read more than one
  // message in our read.
  do
    {
      result = this->process_message ();
    }
  while (result > 1);

  return result;
}


int
TAO_IIOP_Transport::register_handler (void)
{
  // @@ It seems like this method should go away, the right reactor is
  //    picked at object creation time.
  ACE_Reactor *r = this->orb_core_->reactor ();

  if (r == this->connection_handler_->reactor ())
    return 0;

  // About to be registered with the reactor, so bump the ref
  // count
  this->connection_handler_->incr_ref_count ();

  // Set the flag in the Connection Handler
  this->connection_handler_->is_registered (1);


  // Register the handler with the reactor
  return  r->register_handler (this->connection_handler_,
                               ACE_Event_Handler::READ_MASK);
}


int
TAO_IIOP_Transport::send_request (TAO_Stub *stub,
                                  TAO_ORB_Core *orb_core,
                                  TAO_OutputCDR &stream,
                                  int two_way,
                                  ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  two_way) == -1)
    return -1;

  if (this->send_message (stream,
                          stub,
                          two_way,
                          max_wait_time) == -1)

    return -1;

  return this->idle_after_send ();
}

int
TAO_IIOP_Transport::send_message (TAO_OutputCDR &stream,
                                  TAO_Stub *stub,
                                  int twoway,
                                  ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object_->format_message (stream) != 0)
    return -1;

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = this->send_or_buffer (stub,
                                    twoway,
                                    stream.begin (),
                                    max_wait_time);

  if (n == -1)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO: (%P|%t|%N|%l) closing conn %d after fault %p\n"),
                    this->handle (),
                    ACE_TEXT ("send_message ()\n")));

      return -1;
    }

  // EOF.
  if (n == 0)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO: (%P|%t|%N|%l) send_message () \n")
                    ACE_TEXT ("EOF, closing conn %d\n"),
                    this->handle()));
      return -1;
    }

  return 1;
}


void
TAO_IIOP_Transport::start_request (TAO_ORB_Core * /*orb_core*/,
                                   TAO_Target_Specification & /*spec */,
                                   TAO_OutputCDR & /*output */,
                                   CORBA::Environment & /*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_CLIENT_TRANSPORT_START_REQUEST_START);

  // @@ This method should NO longer be required..

  /*  if (this->client_mesg_factory_->write_protocol_header
      (TAO_PLUGGABLE_MESSAGE_REQUEST,
       output) == 0)
       ACE_THROW (CORBA::MARSHAL ());*/
}


void
TAO_IIOP_Transport::start_locate (TAO_ORB_Core * /*orb_core*/,
                                  TAO_Target_Specification &spec,
                                  TAO_Operation_Details &opdetails,
                                  TAO_OutputCDR &output,
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->messaging_object_->generate_locate_request_header (opdetails,
                                                               spec,
                                                               output) == -1)
    ACE_THROW (CORBA::MARSHAL ());
}


CORBA::Boolean
TAO_IIOP_Transport::send_request_header (TAO_Operation_Details &opdetails,
                                         TAO_Target_Specification &spec,
                                         TAO_OutputCDR &msg)
{
  // Check whether we have a Bi Dir IIOP policy set, whether the
  // messaging objects are ready to handle bidirectional connections
  // and also make sure that we have not recd. or sent any information
  // regarding this before...
  if (this->orb_core ()->bidir_giop_policy () &&
      this->messaging_object_->is_ready_for_bidirectional () &&
      this->bidirectional_flag () < 0)
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
  if (this->messaging_object_->generate_request_header (opdetails,
                                                        spec,
                                                        msg) == -1)
    return 0;

  return 1;
}

int
TAO_IIOP_Transport::messaging_init (CORBA::Octet major,
                                    CORBA::Octet minor)
{
  this->messaging_object_->init (major,
                                 minor);
  return 1;
}

int
TAO_IIOP_Transport::tear_listen_point_list (TAO_InputCDR &cdr)
{
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return -1;

  cdr.reset_byte_order (ACE_static_cast(int,byte_order));

  IIOP::ListenPointList listen_list;
  if ((cdr >> listen_list) == 0)
    return -1;

  // As we have received a bidirectional information, set the flag to
  // 1
  this->bidirectional_flag (1);
  return this->connection_handler_->process_listen_point_list (listen_list);
}


int
TAO_IIOP_Transport::process_message (void)
{
  // Get the <message_type> that we have received
  TAO_Pluggable_Message_Type t =
    this->messaging_object_->message_type ();


  int result = 0;
  if (t == TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %p\n"),
                    ACE_TEXT ("Close Connection Message recd \n")));

      this->tms_->connection_closed ();
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_REQUEST)
    {
      if (this->messaging_object_->process_request_message (this,
                                                            this->orb_core ()) == -1)
        return -1;
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_REPLY)
    {
      TAO_Pluggable_Reply_Params params (this->orb_core ());
      if (this->messaging_object_->process_reply_message (params)  == -1)
        {

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - %p\n"),
                        ACE_TEXT ("IIOP_Transport::process_message, process_reply_message ()")));

          this->messaging_object_->reset ();
          this->tms_->connection_closed ();
          return -1;
        }


      result =
        this->tms_->dispatch_reply (params);

      // @@ Somehow it seems dangerous to reset the state *after*
      //    dispatching the request, what if another threads receives
      //    another reply in the same connection?
      //    My guess is that it works as follows:
      //    - For the exclusive case there can be no such thread.
      //    - The the muxed case each thread has its own message_state.
      //    I'm pretty sure this comment is right.  Could somebody else
      //    please look at it and confirm my guess?

      // @@ The above comment was found in the older versions of the
      //    code. The code was also written in such a way that, when
      //    the client thread on a call from handle_input () from the
      //    reactor a call would be made on the handle_client_input
      //    (). The implementation of handle_client_input () looked so
      //    flaky. It used to create a message state upon entry in to
      //    the function using the TMS and destroy that on exit. All
      //    this was fine _theoretically_ for multiple threads. But
      //    the flakiness was originating in the implementation of
      //    get_message_state () where we were creating message state
      //    only once and dishing it out for every thread till one of
      //    them destroy's it. So, it looked broken. That has been
      //    changed. Why?. To my knowledge, the reactor does not call
      //    handle_input () on two threads at the same time. So, IMHO
      //    that defeats the purpose of creating a message state for
      //    every thread. This is just my guess. If we run in to
      //    problems this place needs to be revisited. If someone else
      //    is going to take a look please contact bala@cs.wustl.edu
      //    for details on this-- Bala



      if (result == -1)
        {
          // Something really critical happened, we will forget about
          // every reply on this connection.
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) : IIOP_Client_Transport::")
                        ACE_TEXT ("handle_client_input - ")
                        ACE_TEXT ("dispatch reply failed\n")));

          this->messaging_object_->reset ();
          this->tms_->connection_closed ();
          return -1;
        }

      if (result == 0)
        {
          this->messaging_object_->reset ();

          // The reply dispatcher was no longer registered.
          // This can happened when the request/reply
          // times out.
          // To throw away all registered reply handlers is 
          // not the right thing, as there might be just one
          // old reply coming in and several valid new ones
          // pending. If we would invoke <connection_closed>
          // we would throw away also the valid ones.
          //return 0;
        }


      // This is a NOOP for the Exclusive request case, but it actually
      // destroys the stream in the muxed case.
      //this->tms_->destroy_message_state (message_state);
    }
  else if (t == TAO_PLUGGABLE_MESSAGE_MESSAGERROR)
    {
      return -1;
    }

  return this->messaging_object_->more_messages ();
}


void
TAO_IIOP_Transport::set_bidir_context_info (TAO_Operation_Details &opdetails)
{

  // Get a handle on to the acceptor registry
  TAO_Acceptor_Registry * ar =
    this->orb_core ()->acceptor_registry ();


  // Get the first acceptor in the registry
  TAO_AcceptorSetIterator acceptor = ar->begin ();

  IIOP::ListenPointList listen_point_list;

  for (;
       acceptor != ar->end ();
       acceptor++)
    {
      // Check whether it is a IIOP acceptor
      if ((*acceptor)->tag () == TAO_TAG_IIOP_PROFILE)
        {
          this->get_listen_point (listen_point_list,
                                  *acceptor);
        }
    }

  // We have the ListenPointList at this point. Create a output CDR
  // stream at this point
  TAO_OutputCDR cdr;

  // Marshall the information into the stream
  if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)== 0)
      || (cdr << listen_point_list) == 0)
    return;

  // Add this info in to the svc_list
  opdetails.service_context ().set_context (IOP::BI_DIR_IIOP,
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

  // Get the array of endpoints serviced by <iiop_acceptor>
  const ACE_INET_Addr *endpoint_addr =
    iiop_acceptor->endpoints ();

  // Get the count
  size_t count =
    iiop_acceptor->endpoint_count ();

  // Get the local address of the connection
  ACE_INET_Addr local_addr;

  if (this->connection_handler_->peer ().get_local_addr (local_addr)
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Could not resolve local host")
                         ACE_TEXT (" address in set_bidir_context_info () \n")),
                        -1);
    }


  // Note: Looks like there is no point in sending the list of
  // endpoints on interfaces on which this connection has not
  // been established. If this is wrong, please correct me.
  char *local_interface = 0;

  // Get the hostname for the local address
  if (iiop_acceptor->hostname (this->orb_core_,
                               local_addr,
                               local_interface) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Could not resolve local host")
                         ACE_TEXT (" name \n")),
                        -1);
    }

  ACE_INET_Addr *tmp_addr = ACE_const_cast (ACE_INET_Addr *,
                                            endpoint_addr);

  for (size_t index = 0;
       index <= count;
       index++)
    {
      // Get the listen point on that acceptor if it has the same
      // interface on which this connection is established
      char *acceptor_interface = 0;

      if (iiop_acceptor->hostname (this->orb_core_,
                                   tmp_addr[index],
                                   acceptor_interface) == -1)
          continue;

      // @@ This is very bad for performance, but it is a one time
      // affair
      if (ACE_OS::strcmp (local_interface,
                          acceptor_interface) == 0)
        {
          // We have the connection and the acceptor endpoint on the
          // same interface
          IIOP::ListenPoint point;
          point.host = CORBA::string_dup (local_interface);
          point.port = endpoint_addr[index].get_port_number ();

          // Get the count of the number of elements
          CORBA::ULong len = listen_point_list.length ();

          // Increase the length by 1
          listen_point_list.length (len + 1);

          // Add the new length to the list
          listen_point_list[len] = point;
        }

      // @@ This is bad....
      CORBA::string_free (acceptor_interface);
    }

  CORBA::string_free (local_interface);
  return 1;
}
