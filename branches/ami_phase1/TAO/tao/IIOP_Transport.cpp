// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/IIOP_Transport.h"
#include "tao/GIOP.h"
#include "tao/Connect.h"
#include "tao/Timeprobe.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Transport_Timeprobe_Description[] =
  {
    "IIOP_Transport::send - start",
    "IIOP_Transport::send - end",

    "IIOP_Transport::receive - start",
    "IIOP_Transport::recieve - end",

    "IIOP_Client_Transport::send_request - start",
    "IIOP_Client_Transport::send_request - end"
  };

enum
  {
    TAO_IIOP_TRANSPORT_SEND_START = 1200,
    TAO_IIOP_TRANSPORT_SEND_END,

    TAO_IIOP_TRANSPORT_RECEIVE_START,
    TAO_IIOP_TRANSPORT_RECEIVE_END,

    TAO_IIOP_CLIENT_TRANSPORT_SEND_REQUEST_START,
    TAO_IIOP_CLIENT_TRANSPORT_SEND_REQUEST_END
  };


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Transport_Timeprobe_Description,
                                  TAO_IIOP_TRANSPORT_SEND_START);

#endif /* ACE_ENABLE_TIMEPROBES */


TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Handler_Base* handler,
                                        TAO_IIOP_Request_Multiplexing_Strategy *rms,
                                        TAO_IIOP_Wait_Strategy *ws)
  : handler_(handler),
    tag_(TAO_IOP_TAG_INTERNET_IOP),
    rms_ (rs),
    ws_ (ws)
{
  // @@ I am creating the RMS strategy here. (alex)
  ACE_NEW (rms_,
           TAO_IIOP_Exclusive_RMS);

  // @@ Creating the WS here. (alex)
  ACE_NEW (ws_,
           TAO_Wait_On_Read);
}

TAO_IIOP_Transport::~TAO_IIOP_Transport (void)
{
}

TAO_IIOP_Server_Transport::TAO_IIOP_Server_Transport (TAO_Server_Connection_Handler *handler)
  : TAO_IIOP_Transport(handler),
    server_handler_ (0)
{
  server_handler_ = handler;
}

TAO_IIOP_Client_Transport::TAO_IIOP_Client_Transport (TAO_Client_Connection_Handler *handler)
  :  TAO_IIOP_Transport(handler),
     client_handler_ (0)
{
  client_handler_ = handler;
}

TAO_IIOP_Server_Transport::~TAO_IIOP_Server_Transport (void)
{
}

TAO_IIOP_Client_Transport::~TAO_IIOP_Client_Transport (void)
{
}

CORBA::ULong
TAO_IIOP_Transport::tag (void)
{
  return this->tag_;
}

TAO_Client_Connection_Handler *
TAO_IIOP_Client_Transport::client_handler (void)
{
  return this->client_handler_;
}

TAO_Server_Connection_Handler *
TAO_IIOP_Server_Transport::server_handler (void)
{
  return this->server_handler_;
}

TAO_IIOP_Handler_Base *&
TAO_IIOP_Transport::handler (void)
{
  return this->handler_;
}

int
TAO_IIOP_Transport::idle (void)
{
  return this->handler_->idle();
}

int
TAO_IIOP_Transport::is_nil (TAO_Transport *obj)
{
  return obj == 0;
}

TAO_Transport *
TAO_IIOP_Transport::_nil (void)
{
  return (TAO_IIOP_Transport *)0;
}

void
TAO_IIOP_Transport::resume_connection (ACE_Reactor *reactor)
{
  this->handler_->resume_handler (reactor);
}

void
TAO_IIOP_Transport::close_connection (void)
{
  this->handler_->handle_close ();
}

ACE_HANDLE
TAO_IIOP_Transport::handle (void)
{
  return this->handler_->get_handle ();
}

int
TAO_IIOP_Client_Transport::send_request (TAO_ORB_Core *orb_core,
                                         TAO_OutputCDR &stream,
                                         int twoway)
{
  ACE_FUNCTION_TIMEPROBE (TAO_IIOP_CLIENT_TRANSPORT_SEND_REQUEST_START);

  return this->client_handler_->send_request (orb_core, stream, twoway);
}

// int
// TAO_IIOP_Server_Transport::send_response (TAO_OutputCDR &response)
// {
//   this->server_handler_->send_response (response);
//   return 1;
// }

int
TAO_IIOP_Client_Transport::handle_client_input (int read_header)
{
  // When we multiplex several invocations over a connection we need
  // to allocate the CDR stream *here*, but when there is a single
  // request over a connection the CDR stream can be pre-allocated on
  // the stack of the thread that sent the request!
  // Can we preserve this optimization on the new architecture?
  //
  // here is how:
  //
  // Use an "factory" to obtain the CDR stream, in the Muxed case the
  // factory simply allocates a new one, in the Exclusive case the
  // factory returns a pointer to the pre-allocated CDR.
  //
  // Receive the message. Get also the GIOP version number. 
  // <recv_message> is non-blocking!!!!
  // 
  //   + In <recv_message>, Don't worry about blocking on the GIOP
  //     header, it's only 12 bytes, use read_n() for the header but
  //     non-blocking for the rest.
  // 
  //   + After reading the header you can allocate memory for the
  //      complete buffer [this is there already, look at how they
  //      do it!]
  //

  TAO_InputCDR* cdr =
    this->request_reply_strategy->get_cdr_stream ();
  
  TAO_GIOP_Version version;
  
  int message_type = 
    GIOP::recv_message (this,
                        cdr,
                        this->orb_core_,
                        version);

  switch (message_type)
    {
    case TAO_GIOP::ShortRead:
      // Return a value so that this we will get called again to
      // handle the input.  
      return 0;
      // NOT REACHED.

    case TAO_GIOP::EndOfFile:
    case TAO_GIOP::CommunicatioError:
    case TAO_GIOP::MessageError:
      // Handle errors like those...
      this->reply_handler_->error ();
      return 1;

    case TAO_GIOP::Request:
      // In GIOP 1.0 and GIOP 1.1 this is an error, but it is
      // *possible* to receive requests in GIOP 1.2.  Don't handle this 
      // on the firt iteration, leave it for the nearby future...
      // ERROR too.
      this->reply_handler_->error ();
      return 1;
      
    case TAO_GIOP::LocateReply:
    case TAO_GIOP::Reply:
      // Handle after the switch.
      break;
    }

  // For GIOP 1.0 and 1.1 the reply_ctx comes first:
  TAO_GIOP_ServiceContextList reply_ctx;
  this->inp_stream_ >> reply_ctx;
  
  // We should pass that reply_ctx to the invocation, interceptors
  // will want to read it!
  
  // Read the request id and the reply status type.
  // status can be NO_EXCEPTION, SYSTEM_EXCEPTION, USER_EXCEPTION,
  // LOCATION_FORWARD or (on GIOP 1.2) LOCATION_FORWARD_PERM

  CORBA::ULong request_id;
  CORBA::ULong reply_status;
  
  if (!this->inp_stream_.read_ulong (request_id))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):TAO_IIOP_Client_Transport::handle_client_input: "
                       "Failed to read request_id.\n"),
                      -1);
  
  if (!this->inp_stream_.read_ulong (reply_status))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):TAO_IIOP_Client_Transport::handle_client_input: "
                       "Failed to read request_status type.\n"),
                      -1);
  
  // Find the TAO_Reply_Handler for that request ID!
  TAO_Reply_Handler* reply_handler =
    this->request_reply_strategy->find_handler (request_id);
  if (reply_handler == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):TAO_IIOP_Client_Transport::handle_client_input: "
                       "Failed to find Reply Handler.\n"),
                      -1);
  
  // Handle the reply.
  if (reply_handler->handle_reply (reply_status, cdr) == -1)
    return -1;
  
  // This is a NOOP for the Exclusive request case, but it actually 
  // destroys the stream in the muxed case.
  this->request_multiplexing_stratetgy_->destroy_cdr_stream (cdr);
  
  // Return something to indicate the reply is received.
  return 1;
}


ssize_t
TAO_IIOP_Transport::send (const ACE_Message_Block *mblk, ACE_Time_Value *s)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_SEND_START);

  ACE_UNUSED_ARG (s);

  // For the most part this was copied from GIOP::send_request and
  // friends.

  // @@ Fred, this should NOT be a local constant...  It should use a
  // macro defined in OS.h...
  const int TAO_WRITEV_MAX = 16;
  iovec iov[TAO_WRITEV_MAX];
  int iovcnt = 0;
  ssize_t n = 0;
  ssize_t nbytes = 0;

  for (const ACE_Message_Block *i = mblk;
       i != 0;
       i = i->cont ())
    {
      // Make sure there is something to send!
      if (i->length () > 0)
        {
          iov[iovcnt].iov_base = i->rd_ptr ();
          iov[iovcnt].iov_len  = i->length ();
          iovcnt++;

          // The buffer is full make a OS call.  @@ TODO this should
          // be optimized on a per-platform basis, for instance, some
          // platforms do not implement writev() there we should copy
          // the data into a buffer and call send_n(). In other cases
          // there may be some limits on the size of the iovec, there
          // we should set TAO_WRITEV_MAX to that limit.
          if (iovcnt == TAO_WRITEV_MAX)
            {
              n = this->handler_->peer ().sendv_n ((const iovec *) iov,
                                                   iovcnt);
              if (n < 1)
                return n;

              nbytes += n;
              iovcnt = 0;
            }
        }
    }

  // Check for remaining buffers to be sent!
  if (iovcnt != 0)
    {
      n = this->handler_->peer ().sendv_n ((const iovec *) iov,
                                           iovcnt);
      if (n < 1)
        return n;

      nbytes += n;
    }

  return nbytes;
}

ssize_t
TAO_IIOP_Transport::send (const u_char *buf,
                          size_t len,
                          ACE_Time_Value *s)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_SEND_START);

  ACE_UNUSED_ARG (s);
  return this->handler_->peer ().send_n (buf, len);
}

ssize_t
TAO_IIOP_Transport::send (const iovec *iov,
                          int iovcnt,
                          ACE_Time_Value *s)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_SEND_START);

  ACE_UNUSED_ARG (s);
  return this->handler_->peer ().sendv_n ((const iovec *) iov,
                                          iovcnt);
}

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          ACE_Time_Value *s)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_RECEIVE_START);

  ACE_UNUSED_ARG (s);
  return this->handler_->peer ().recv_n (buf, len);
}

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          int flags,
                          ACE_Time_Value *s)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_RECEIVE_START);

  ACE_UNUSED_ARG (s);
  return this->handler_->peer ().recv_n (buf,
                                         len,
                                         flags);
}

ssize_t
TAO_IIOP_Transport::recv (iovec *iov,
                          int iovcnt,
                          ACE_Time_Value *s)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_RECEIVE_START);

  ACE_UNUSED_ARG (s);
  return handler_->peer ().recvv_n (iov, iovcnt);
}

TAO_GIOP_ReplyStatusType
TAO_IIOP_Transport::wait_for_reply (CORBA::ULong request_id)
{
  return this->ws_->wait (request_id,
                          this->rms_);
}

// *********************************************************************

TAO_IIOP_Request_Multiplexing_Strategy::TAO_IIOP_Request_Multiplexing_Strategy (void)
{
}

TAO_IIOP_Request_Multiplexing_Strategy::~TAO_IIOP_Request_Multiplexing_Strategy (void)
{
}

// *********************************************************************

TAO_IIOP_Muxed_RMS::TAO_IIOP_Muxed_RMS (void)
{
}

TAO_IIOP_Muxed_RMS::~TAO_IIOP_Muxed_RMS (void)
{
}

TAO_InputCDR *
TAO_IIOP_Muxed_RMS::get_cdr_stream (void)
{
  // @@ 
}  

void
TAO_IIOP_Muxed_RMS::destroy_cdr_stream (TAO_InputCDR *cdr)
{
  delete cdr;
  cdr = 0;
}

// *********************************************************************

TAO_IIOP_Exclusive_RMS::TAO_IIOP_Exclusive_RMS (void)
  : request_id_ (0),
    rh_ (0)
{
}

TAO_IIOP_Exclusive_RMS::~TAO_IIOP_Exclusive_RMS (void)
{
}

// Bind the handler with the request id.
int
TAO_IIOP_Exclusive_RMS::bind_handler (CORBA::ULong request_id,
                                      TAO_Reply_Handlern *rh)
{
  if (rh_ != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:TAO_IIOP_Exclusive_RMS::bind_handler: "
                       "Failed to bind the handler\n"),
                      -1);
  this->request_id_ = request_id;
  this->rh_ = rh;
  return 0;
}

// Find the Reply Handler.  
TAO_Reply_Handler*
TAO_IIOP_Exclusive_RMS::find_handler (CORBA::ULong request_id)
{
  if (this->request_id_ != request_id)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:TAO_IIOP_Exclusive_RMS::find_handler: ",
                       "Failed to find the handler\n"),
                      0);
  
  return this->rh_;
}

// Return the preallocated CDR stream.
TAO_InputCDR *
TAO_IIOP_Exclusive_RMS::get_cdr_stream (void)
{  
  return this->cdr_;
}

// NOOP function.
void
TAO_IIOP_Exclusive_RMS::destroy_cdr_stream (TAO_InputCDR *)
{
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Reactor::TAO_Wait_On_Reactor (void)
{
}

// Destructor.  
TAO_Wait_On_Reactor::~TAO_Wait_On_Reactor (void)
{
}

TAO_GIOP_ReplyStatusType
TAO_Wait_On_Reactor::wait (CORBA::ULong request_id,
                           TAO_IIOP_Request_Multiplexing_Strategy *rms)
{
  int end_loop_flag = 0;

  TAO_Reactor_Reply_Handler rh (end_loop_flag);
  
  if (rrs->bind (request_id, &rh) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):TAO_Wait_On_Reactor::wait: "
                       "Failed to bind reply handler.\n"),
                      -1);
  int result = 0;
  while ((result != -1) && (end_loop_flag == 0))
    result = this->orb_core_->reactor ()->handle_events (/* timeout */);
  
  return 0;
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Leader_Follower::TAO_Wait_On_Leader_Follower (void)
{
}

// Destructor.  
TAO_Wait_On_Leader_Follower::~TAO_Wait_On_Leader_Follower (void)
{
}

  
TAO_GIOP_ReplyStatusType
TAO_Wait_On_Leader_Follower::wait (CORBA::ULong request_id,
                                   TAO_IIOP_Request_Multiplexing_Strategy *rms)
{
  // Grab leader follower lock.
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                            ace_mon,
                            this->orb_core_->leader_follower_lock (),
                            -1));

  // Bind the reply handler.

  TAO_Leader_Follower_Reply_Handler rh (..);

  rrs->bind (this->request_id, &rh);

  // Check if we need to become the leader.
  if (!this->orb_core_->leader_available ())
    {
      // This might increase the refcount of the leader. 
      this->orb_core_->set_leader_thread ();

      // Do the reactor event loop.
      this->orb_core_->reactor ()->owner (ACE_Thread::self ());

      int result = 0;
      while (result != -1)
        result = this->orb_core_->reactor ()->handle_events ();
    
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%N:%l:(%P | %t):TAO_Wait_On_Leader_Follower::wait: "
                           "Reactor::handle_events failed.\n"),
                          -1);
    }
  else
    {
      // Block on condition variable.
      ACE_SYNCH_CONDITION* cond =
        this->cond_response_available (orb_core);
      if (this->orb_core_->add_follower (cond) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%N:%l:(%P|%t) TAO_Wait_On_Leader_Follower::wait: " 
                    "Failed to add a follower thread\n"));
      cond->wait ();
    }
  return 0;
}

// *********************************************************************

// Constructor.
TAO_Wait_On_Read::TAO_Wait_On_Read (TAO_IIOP_Transport *transport)
  : transport_ (transport)
{
}

// Destructor.
TAO_Wait_On_Read::~TAO_Wait_On_Read (void)
{
}

// Wait on the read operation.
int
TAO_Wait_On_Read::wait (CORBA::ULong request_id,
                        TAO_IIOP_Request_Multiplexing_Strategy *rms)
{
  TAO_Wait_On_Read_RH rh;
  
  // Bind the <request_id, handler> pair with the strategy.  
  if (rms->bind (request_id, &rh) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:TAO_Wait_On_Read::wait: "
                       "Failed to bind the Reply Handler"),
                      -1);
  
  int received_reply = 0;
  while (!received_reply)
    {
      // In this case sockets *must* be blocking.
      // We need to control how they are set!
      received_reply = this->transport_->handle_client_input ();
    }
  
  return 0;
} 

 
