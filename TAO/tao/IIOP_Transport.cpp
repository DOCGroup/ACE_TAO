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
#include "tao/debug.h"

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


TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Handler_Base *handler,
                                        TAO_ORB_Core *orb_core)
  : TAO_Transport (TAO_IOP_TAG_INTERNET_IOP,
                   orb_core),
    handler_(handler)
{
}

TAO_IIOP_Transport::~TAO_IIOP_Transport (void)
{
}

TAO_IIOP_Server_Transport::TAO_IIOP_Server_Transport (TAO_Server_Connection_Handler *handler)
  : TAO_IIOP_Transport(handler,
                       TAO_ORB_Core_instance ()),
    server_handler_ (handler)
{
}

TAO_IIOP_Client_Transport::TAO_IIOP_Client_Transport (TAO_Client_Connection_Handler *handler,
                                                      TAO_ORB_Core *orb_core)
  :  TAO_IIOP_Transport (handler,
                         orb_core),
     client_handler_ (handler)
{
  client_handler_ = handler;
}

TAO_IIOP_Server_Transport::~TAO_IIOP_Server_Transport (void)
{
}

TAO_IIOP_Client_Transport::~TAO_IIOP_Client_Transport (void)
{
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
                                         int two_way)
{
  ACE_FUNCTION_TIMEPROBE (TAO_IIOP_CLIENT_TRANSPORT_SEND_REQUEST_START);

  return this->ws_->send_request (orb_core,
                                  stream,
                                  two_way);
}

// Return 0, when the reply is not read fully, 1 if it is read fully.
// @@ This code should go in the TAO_Transport class is repeated for
//    each transport!!
int
TAO_IIOP_Client_Transport::handle_client_input (int block)
{
  // @@ Alex: it should be possible to make this code generic and move
  //    it to the GIOP class or something similar....

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
  // @@ Alex: I thought some more about this, and here is how i would
  //    like to do it: this class keeps a CDR stream for the "current"
  //    message beign received. Initially the CDR is 0, when the
  //    handle_client_input() is called the first time then we go to
  //    the muxer to obtain the CDR stream.
  //    - The exclusive Muxer returns the CDR stream pre-allocated by
  //      the invocation.
  //    - The shared Muxer returns a new CDR stream.
  //    Once all the data has been received the reply handler takes
  //    charge of the CDR stream, or actually of its message block,
  //    which is referenced counted and thus can be efficiently
  //    removed.
  //    Do I make any sense?

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

  // @@ Exclsive RMS instead of giving the CDR given by the Invocation
  //    class, it should give the preallocated CDR so that it can give
  //    that CDR to the invocation back, if there is a valid reply or
  //    it can just forget it, for example, if there was a close
  //    connection message or something. (Alex)

  // If RMS not expecting any message, handle the unexpected data.
  if (cdr == 0)
    return this->check_unexpected_data ();

  TAO_GIOP_Version version;

  TAO_GIOP::Message_Type message_type =
    TAO_GIOP::recv_message (this,
                            *cdr,
                            this->orb_core_,
                            version,
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
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l handle_client_input: "
                         "error on stream.\n"),
                        -1);

    case TAO_GIOP::Fragment:
      // Handle this.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l handle_client_input: "
                         "fragment.\n"),
                        -1);

    case TAO_GIOP::Request:
      // In GIOP 1.0 and GIOP 1.1 this is an error, but it is
      // *possible* to receive requests in GIOP 1.2.  Don't handle this
      // on the firt iteration, leave it for the nearby future...
      // ERROR too.
      // @@ this->reply_handler_->error ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l handle_client_input: "
                         "request.\n"),
                        -1);

    case TAO_GIOP::CancelRequest:
    case TAO_GIOP::LocateRequest:
    case TAO_GIOP::CloseConnection:
    default:
      // @@ Errors for the time being.
      // @@ this->reply_handler_->error ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l handle_client_input: "
                         "wrong message.\n"),
                        -1);

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
                       "TAO (%P|%t) : IIOP_Client_Transport::"
                       "handle_client_input - error while "
                       "reading request_id\n"),
                      -1);

  if (!cdr->read_ulong (reply_status))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO (%P|%t) : IIOP_Client_Transport::"
                       "handle_client_input - error while "
                       "reading reply status\n"),
                      -1);

  // @@ Alex: for some reason this was causing a crash with the
  //    leader-follower wait strategy.  Somehow it seems like the rms
  //    still has a pointer to an object that was already destroyed
  //    (i.e. the stack was unrolled on the thread waiting for this
  //     event), since this is only needed for *true* asynchronous
  //     messaging.
  //ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - dispatching reply <%x>\n", this));
  if (this->rms_->dispatch_reply (request_id,
                                  reply_status,
                                  version,
                                  reply_ctx,
                                  cdr) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) : IIOP_Client_Transport::"
                         "handle_client_input - "
                         "dispatch reply failed\n"),
                        -1);
    }

  // This is a NOOP for the Exclusive request case, but it actually
  // destroys the stream in the muxed case.
  this->destroy_cdr_stream (cdr);

  // Return something to indicate the reply is received.
  return 1;
}

int
TAO_IIOP_Client_Transport::register_handler (void)
{
  ACE_Reactor *r = this->orb_core ()->reactor ();
  if (r == this->client_handler ()->reactor ())
    return 0;

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

int
TAO_IIOP_Client_Transport::handle_close (void)
{
  this->wait_strategy ()->handle_close ();
  // @@ Should we? : this->rms_->handle_close ();
  return 0;
}

int
TAO_IIOP_Client_Transport::check_unexpected_data (void)
{
  // @@ Alex: This should *not* be part of the client connection
  //    handler, we should treat any incoming data as a GIOP message.
  //    The server can always send the "CloseConnection" message and
  //    we should act accordingly.
  //    Finally: in the future the server may send requests though
  //    GIOP 1.2 connections, we shouldn't hard-code the current GIOP
  //    state machine at this level...

  // We're a client, so we're not expecting to see input.  Still we
  // better check what it is!
  char ignored;
  ssize_t ret = this->client_handler ()->peer().recv (&ignored,
                                                      sizeof ignored,
                                                      MSG_PEEK);
  switch (ret)
    {
    case 0:
    case -1:
      // 0 is a graceful shutdown
      // -1 is a somewhat ugly shutdown
      //
      // Both will result in us returning -1 and this connection
      // getting closed
      //
      // if (errno == EWOULDBLOCK)
      // return 0;

      if (TAO_debug_level)
        ACE_DEBUG ((LM_WARNING,
                    "TAO_IIOP_Client_Transport::check_unexpected_data: "
                    "closing connection on fd %d\n",
                    this->client_handler ()->peer().get_handle ()));
      break;

    case 1:
      //
      // @@ Fix me!!
      //
      // This should be the close connection message.  Since we don't
      // handle this yet, log an error, and close the connection.
      ACE_ERROR ((LM_WARNING,
                  "TAO_IIOP_Client_Transport::check_unexpected_data: "
                  "input while not expecting a response; "
                  "closing connection on fd %d\n",
                  this->client_handler ()->peer().get_handle ()));
      break;
    }

  // We're not expecting input at this time, so we'll always
  // return -1 for now.
  return -1;
}

// *********************************************************************

ssize_t
TAO_IIOP_Transport::send (const ACE_Message_Block *mblk, ACE_Time_Value *)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_SEND_START);

  // For the most part this was copied from GIOP::send_request and
  // friends.

  iovec iov[IOV_MAX];
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
          // we should set IOV_MAX to that limit.
          if (iovcnt == IOV_MAX)
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
                          ACE_Time_Value *)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_SEND_START);

  return this->handler_->peer ().send_n (buf, len);
}

ssize_t
TAO_IIOP_Transport::send (const iovec *iov,
                          int iovcnt,
                          ACE_Time_Value *)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_SEND_START);

  return this->handler_->peer ().sendv_n ((const iovec *) iov,
                                          iovcnt);
}

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          ACE_Time_Value *)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_RECEIVE_START);

  return this->handler_->peer ().recv_n (buf, len);
}

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          int flags,
                          ACE_Time_Value *)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_RECEIVE_START);

  return this->handler_->peer ().recv_n (buf,
                                         len,
                                         flags);
}

ssize_t
TAO_IIOP_Transport::recv (iovec *iov,
                          int iovcnt,
                          ACE_Time_Value *)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_RECEIVE_START);

  return handler_->peer ().recvv_n (iov, iovcnt);
}

// Default action to be taken for send request.
int
TAO_IIOP_Transport::send_request (TAO_ORB_Core *  /* orb_core */,
                                  TAO_OutputCDR & /* stream   */,
                                  int             /* twoway   */)
{
  return -1;
}

