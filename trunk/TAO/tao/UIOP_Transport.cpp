// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/UIOP_Transport.h"

#if defined (TAO_HAS_UIOP)

#include "tao/UIOP_Connect.h"
#include "tao/UIOP_Profile.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_UIOP_Transport_Timeprobe_Description[] =
  {
    "UIOP_Transport::send - start",
    "UIOP_Transport::send - end",

    "UIOP_Transport::receive - start",
    "UIOP_Transport::receive - end",

    "UIOP_Client_Transport::start_request - start",
    "UIOP_Client_Transport::start_request - end"
  };

enum
  {
    TAO_UIOP_TRANSPORT_SEND_START = 1300,
    TAO_UIOP_TRANSPORT_SEND_END,

    TAO_UIOP_TRANSPORT_RECEIVE_START,
    TAO_UIOP_TRANSPORT_RECEIVE_END,

    TAO_UIOP_CLIENT_TRANSPORT_START_REQUEST_START,
    TAO_UIOP_CLIENT_TRANSPORT_START_REQUEST_END
  };


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_UIOP_Transport_Timeprobe_Description,
                                  TAO_UIOP_TRANSPORT_SEND_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_UIOP_Transport::TAO_UIOP_Transport (TAO_UIOP_Handler_Base *handler,
                                        TAO_ORB_Core *orb_core)
  : TAO_Transport (TAO_IOP_TAG_UNIX_IOP,
                   orb_core),
    handler_ (handler)
{
}

TAO_UIOP_Transport::~TAO_UIOP_Transport (void)
{
}

TAO_UIOP_Handler_Base *&
TAO_UIOP_Transport::handler (void)
{
  return this->handler_;
}

int
TAO_UIOP_Transport::idle (void)
{
  return this->handler_->idle();
}

void
TAO_UIOP_Transport::close_connection (void)
{
  this->handler_->handle_close ();
}

ACE_HANDLE
TAO_UIOP_Transport::handle (void)
{
  return this->handler_->get_handle ();
}

// ****************************************************************

TAO_UIOP_Server_Transport::
    TAO_UIOP_Server_Transport (TAO_UIOP_Server_Connection_Handler *handler,
                               TAO_ORB_Core* orb_core)
  : TAO_UIOP_Transport (handler,
                        orb_core),
    server_handler_ (handler),
    message_state_ (orb_core)
{
}

TAO_UIOP_Server_Transport::~TAO_UIOP_Server_Transport (void)
{
}

// ****************************************************************

TAO_UIOP_Client_Transport::
    TAO_UIOP_Client_Transport (TAO_UIOP_Client_Connection_Handler *handler,
                               TAO_ORB_Core *orb_core)
  :  TAO_UIOP_Transport (handler, orb_core),
     client_handler_ (handler)
{
}

TAO_UIOP_Client_Transport::~TAO_UIOP_Client_Transport (void)
{
}

TAO_UIOP_Client_Connection_Handler *
TAO_UIOP_Client_Transport::client_handler (void)
{
  return this->client_handler_;
}

void
TAO_UIOP_Client_Transport::start_request (TAO_ORB_Core *orb_core,
                                          const TAO_Profile* pfile,
                                          TAO_OutputCDR &output,
                                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_UIOP_CLIENT_TRANSPORT_START_REQUEST_START);

  const TAO_UIOP_Profile* profile =
    ACE_dynamic_cast(const TAO_UIOP_Profile*, pfile);

  // @@ This should be implemented in the transport object, which
  //    would query the profile to obtain the version...
  if (TAO_GIOP::start_message (profile->version (),
                               TAO_GIOP::Request,
                               output,
                               orb_core) == 0)
    ACE_THROW (CORBA::MARSHAL ());
}

void
TAO_UIOP_Client_Transport::start_locate (TAO_ORB_Core *orb_core,
                                         const TAO_Profile* pfile,
                                         CORBA::ULong request_id,
                                         TAO_OutputCDR &output,
                                         CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  const TAO_UIOP_Profile* profile =
    ACE_dynamic_cast(const TAO_UIOP_Profile*, pfile);

  // Obtain object key.
  const TAO_ObjectKey& key = profile->object_key ();

  // @@ This should be implemented in the transport object, which
  //    would query the profile to obtain the version...
  if (TAO_GIOP::start_message (profile->version (),
                               TAO_GIOP::LocateRequest,
                               output,
                               orb_core) == 0)
    ACE_THROW (CORBA::MARSHAL ());


  if (TAO_GIOP::write_locate_request_header (request_id,
                                             key,
                                             output) == 0)
    ACE_THROW (CORBA::MARSHAL ());
}

int
TAO_UIOP_Client_Transport::send_request (TAO_ORB_Core *orb_core,
                                         TAO_OutputCDR &stream,
                                         int two_way,
                                         ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  two_way) == -1)
    return -1;

  if (TAO_GIOP::send_message (this,
                              stream,
                              orb_core,
                              max_wait_time) == -1)
    return -1;

  return this->idle_after_send ();
}

// Return 0, when the reply is not read fully, 1 if it is read fully.
// @@ This code should go in the TAO_Transport class is repeated for
//    each transport!!
int
TAO_UIOP_Client_Transport::handle_client_input (int /* block */,
                                                ACE_Time_Value *max_wait_time)
{

  // Notice that the message_state is only modified in one thread at a
  // time because the reactor does not call handle_input() for the
  // same Event_Handler in two threads at the same time.

  // Get the message state from the Transport Mux Strategy.
  TAO_GIOP_Message_State* message_state =
    this->tms_->get_message_state ();

  if (message_state == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) UIOP_Transport::handle_client_input -"
                    " nil message state\n"));
      return -1;
    }

  int result = TAO_GIOP::handle_input (this,
                                       this->orb_core_,
                                       *message_state,
                                       max_wait_time);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "UIOP_Transport::handle_client_input, handle_input"));
      return -1;
    }
  if (result == 0)
    return result;

  // OK, the complete message is here...

  IOP::ServiceContextList reply_ctx;
  CORBA::ULong request_id;
  CORBA::ULong reply_status;

  result = TAO_GIOP::parse_reply (this,
                                  this->orb_core_,
                                  *message_state,
                                  reply_ctx,
                                  request_id,
                                  reply_status);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "UIOP_Transport::handle_client_input, parse reply"));
      message_state->reset ();
      return -1;
    }

  result =
    this->tms_->dispatch_reply (request_id,
                                reply_status,
                                message_state->giop_version,
                                reply_ctx,
                                message_state);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) : UIOP_Client_Transport::"
                    "handle_client_input - "
                    "dispatch reply failed\n"));
      message_state->reset ();
      return -1;
    }

  if (result == 0)
    {
      message_state->reset ();
      return 0;
    }

  // This is a NOOP for the Exclusive request case, but it actually
  // destroys the stream in the muxed case.
  this->tms_->destroy_message_state (message_state);

  // Return something to indicate the reply is received.
  return result;
}

int
TAO_UIOP_Client_Transport::register_handler (void)
{
  // @@ It seems like this method should go away, the right reactor is
  //    picked at object creation time.
  ACE_Reactor *r = this->orb_core ()->reactor ();
  if (r == this->client_handler ()->reactor ())
    return 0;

  return r->register_handler (this->client_handler (),
                              ACE_Event_Handler::READ_MASK);
}

// ****************************************************************

ssize_t
TAO_UIOP_Transport::send (const ACE_Message_Block *mblk,
                          ACE_Time_Value *max_time_wait)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_UIOP_TRANSPORT_SEND_START);

  // @@ This code should be refactored into ACE.cpp or something
  // similar!

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
              if (max_time_wait == 0)
                n = this->handler_->peer ().sendv_n ((const iovec *) iov,
                                                     iovcnt);
              else
                n = ACE::writev (this->handler_->peer ().get_handle (),
                                 (const iovec*) iov,
                                 iovcnt,
                                 max_time_wait);
              if (n <= 0)
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
TAO_UIOP_Transport::send (const u_char *buf,
                          size_t len,
                          ACE_Time_Value *)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_BIOP_TRANSPORT_SEND_START);

  return this->handler_->peer ().send_n (buf, len);
}

ssize_t
TAO_UIOP_Transport::recv (char *buf,
                          size_t len,
                          ACE_Time_Value *max_wait_time)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_UIOP_TRANSPORT_RECEIVE_START);

  return ACE::recv_n (this->handler_->peer ().get_handle (),
                      buf,
                      len,
                      max_wait_time);
}

// Default action to be taken for send request.
int
TAO_UIOP_Transport::send_request (TAO_ORB_Core *  /* orb_core */,
                                  TAO_OutputCDR & /* stream   */,
                                  int             /* twoway   */,
                                  ACE_Time_Value * /* max_wait_time */)
{
  return -1;
}

#endif  /* TAO_HAS_UIOP */
