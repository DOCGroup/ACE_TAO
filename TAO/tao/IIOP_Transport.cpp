// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/IIOP_Transport.h"
#include "tao/Connect.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Request_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/ORB_Core.h"

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
                                        TAO_ORB_Core *orb_core)
  : TAO_Transport (orb_core),
    handler_(handler),
    tag_(TAO_IOP_TAG_INTERNET_IOP)
{
}

TAO_IIOP_Transport::~TAO_IIOP_Transport (void)
{
}

TAO_IIOP_Server_Transport::TAO_IIOP_Server_Transport (TAO_Server_Connection_Handler *handler)
  : TAO_IIOP_Transport(handler,
                       TAO_ORB_Core_instance ()),
    server_handler_ (0)
{
  server_handler_ = handler;
}

TAO_IIOP_Client_Transport::TAO_IIOP_Client_Transport (TAO_Client_Connection_Handler *handler,
                                                      TAO_ORB_Core *orb_core)
  :  TAO_IIOP_Transport (handler,
                         orb_core),
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
  this->ws_->resume_handler (reactor);
  // this->handler_->resume_handler (reactor);
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
                                         int /* twoway */)
{
  ACE_FUNCTION_TIMEPROBE (TAO_IIOP_CLIENT_TRANSPORT_SEND_REQUEST_START);
  
  return TAO_GIOP::send_request (this, stream, orb_core);
}

// int
// TAO_IIOP_Server_Transport::send_response (TAO_OutputCDR &response)
// {
//   this->server_handler_->send_response (response);
//   return 1;
// }

// Return 0, when the reply is not read fully, 1 if it is read fully.
int
TAO_IIOP_Client_Transport::handle_client_input (int block)
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

  //  if (!this->message_size_)
  // {
  // Reading the header.

  // @@ Where do I keep this CDR? (alex)
  // }

  // Get the CDR stream for reading the input.
  TAO_InputCDR* cdr = this->input_cdr_stream ();

  TAO_GIOP::Message_Type message_type =
    TAO_GIOP::recv_message (this,
                            *cdr,
                            this->orb_core_,
                            this->version_,
                            block);
  switch (message_type)
    {
    case TAO_GIOP::ShortRead:
      // Return a value so that this we will get called again to
      // handle the input.
      return 0;
      // NOT REACHED.

    case TAO_GIOP::EndOfFile:
    case TAO_GIOP::CommunicationError:
    case TAO_GIOP::MessageError:
      // Handle errors like these.
      // @@ this->reply_handler_->error ();
      return 1;

    case TAO_GIOP::Fragment:
      // Handle this.
      return 1;

    case TAO_GIOP::Request:
      // In GIOP 1.0 and GIOP 1.1 this is an error, but it is
      // *possible* to receive requests in GIOP 1.2.  Don't handle this
      // on the firt iteration, leave it for the nearby future...
      // ERROR too.
      // @@ this->reply_handler_->error ();
      return 1;

    case TAO_GIOP::CancelRequest:
    case TAO_GIOP::LocateRequest:
    case TAO_GIOP::CloseConnection:
      // @@ Errors for the time being.
      // @@ this->reply_handler_->error ();
      return 1;

    case TAO_GIOP::LocateReply:
    case TAO_GIOP::Reply:
      // Handle after the switch.
      break;
    }

  // For GIOP 1.0 and 1.1 the reply_ctx comes first:
  // @@ Put this reply ctx into the reply dispatcher. so that
  // invocation can read it.
  // We should pass that reply_ctx to the invocation, interceptors
  // will want to read it!

  TAO_GIOP_ServiceContextList reply_ctx;
  *cdr >> reply_ctx;

  // Read the request id and the reply status type.
  // status can be NO_EXCEPTION, SYSTEM_EXCEPTION, USER_EXCEPTION,
  // LOCATION_FORWARD or (on GIOP 1.2) LOCATION_FORWARD_PERM

  CORBA::ULong request_id;
  CORBA::ULong reply_status;

  if (!cdr->read_ulong (request_id))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):TAO_IIOP_Client_Transport::handle_client_input: "
                       "Failed to read request_id.\n"),
                      -1);

  if (!cdr->read_ulong (reply_status))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):TAO_IIOP_Client_Transport::handle_client_input: "
                       "Failed to read request_status type.\n"),
                      -1);

  // Find the TAO_Reply_Handler for that request ID!
  TAO_Reply_Dispatcher* reply_dispatcher =
    this->rms_->find_dispatcher (request_id);
  if (reply_dispatcher == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):TAO_IIOP_Client_Transport::handle_client_input: "
                       "Failed to find Reply Handler.\n"),
                      -1);

  // Init the Reply dispatcher with all the reply info.
  reply_dispatcher->reply_status (reply_status);
  // @@ reply_dispatcher->version (this->version ());
  // @@ reply->dispatcher->reply_context (reply_ctx);
  reply_dispatcher->cdr (cdr);

  // @@ Alex: I think that a better interface is:
  //  reply_dispatcher->dispatch_reply (reply_status, version,
  //                                    reply_ctx, cdr);
  // That way we don't need to keep state in the dispatch_reply
  // object, careful about the lifetime of the reply_ctx and CDR
  // objects because they allocate memory....
  
  // Handle the reply.
  if (reply_dispatcher->dispatch_reply () == -1)
    return -1;

  // This is a NOOP for the Exclusive request case, but it actually
  // destroys the stream in the muxed case.
  this->rms_->destroy_cdr_stream ();

  // Return something to indicate the reply is received.
  return 1;
}

int
TAO_IIOP_Client_Transport::register_handler (void)
{
  ACE_Reactor *r = this->orb_core ()->reactor ();
  return r->register_handler (this->client_handler (),
                              ACE_Event_Handler::READ_MASK);
}

int
TAO_IIOP_Client_Transport::suspend_handler (void)
{
  return this->orb_core ()->reactor ()->suspend_handler
    (this->client_handler ()); 
}

int
TAO_IIOP_Client_Transport::resume_handler (void)
{
  return this->orb_core ()->reactor ()->resume_handler
    (this->client_handler ()); 
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

// Default action to be taken for send request.
int
TAO_IIOP_Transport::send_request (TAO_ORB_Core *  /* orb_core */,
                                  TAO_OutputCDR & /* stream   */,
                                  int             /* twoway   */)
{
  return -1;
};
