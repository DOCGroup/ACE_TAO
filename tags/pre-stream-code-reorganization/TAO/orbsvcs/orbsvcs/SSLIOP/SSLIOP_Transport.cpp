// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "SSLIOP_Transport.h"
#include "SSLIOP_Connect.h"
#include "SSLIOP_Profile.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Sync_Strategies.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#include "tao/GIOP_Message_Connectors.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Transport_Timeprobe_Description[] =
  {
    "SSLIOP_Transport::send - start",
    "SSLIOP_Transport::send - end",

    "SSLIOP_Transport::receive - start",
    "SSLIOP_Transport::receive - end",

    "SSLIOP_Client_Transport::start_request - start",
    "SSLIOP_Client_Transport::start_request - end"
  };

enum
  {
    TAO_SSLIOP_TRANSPORT_SEND_START = 2200,
    TAO_SSLIOP_TRANSPORT_SEND_END,

    TAO_SSLIOP_TRANSPORT_RECEIVE_START,
    TAO_SSLIOP_TRANSPORT_RECEIVE_END,

    TAO_SSLIOP_CLIENT_TRANSPORT_START_REQUEST_START,
    TAO_SSLIOP_CLIENT_TRANSPORT_START_REQUEST_END
  };


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Transport_Timeprobe_Description,
                                  TAO_SSLIOP_TRANSPORT_SEND_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_SSLIOP_Transport::TAO_SSLIOP_Transport (TAO_SSLIOP_Handler_Base *handler,
                                        TAO_ORB_Core *orb_core)
  : TAO_Transport (TAO_TAG_IIOP_PROFILE,
                   orb_core),
    handler_ (handler)
{
}

TAO_SSLIOP_Transport::~TAO_SSLIOP_Transport (void)
{
}

TAO_SSLIOP_Handler_Base *&
TAO_SSLIOP_Transport::handler (void)
{
  return this->handler_;
}

int
TAO_SSLIOP_Transport::idle (void)
{
  return this->handler_->idle();
}

void
TAO_SSLIOP_Transport::close_connection (void)
{
  this->handler_->handle_close ();
}

ACE_HANDLE
TAO_SSLIOP_Transport::handle (void)
{
  return this->handler_->get_handle ();
}

ACE_Event_Handler *
TAO_SSLIOP_Transport::event_handler (void)
{
  return this->handler_;
}

// ****************************************************************

TAO_SSLIOP_Server_Transport::
    TAO_SSLIOP_Server_Transport (TAO_SSLIOP_Server_Connection_Handler *handler,
                               TAO_ORB_Core* orb_core)
  : TAO_SSLIOP_Transport (handler, orb_core),
    server_handler_ (handler),
    message_state_ (orb_core)
{
}

TAO_SSLIOP_Server_Transport::~TAO_SSLIOP_Server_Transport (void)
{
}

// ****************************************************************

TAO_SSLIOP_Client_Transport::
    TAO_SSLIOP_Client_Transport (TAO_SSLIOP_Client_Connection_Handler *handler,
                               TAO_ORB_Core *orb_core)
  :  TAO_SSLIOP_Transport (handler,
                         orb_core),
     client_handler_ (handler),
     client_mesg_factory_ (0),
     orb_core_ (orb_core),
     params_ ()
{
}

TAO_SSLIOP_Client_Transport::~TAO_SSLIOP_Client_Transport (void)
{
}

TAO_SSLIOP_Client_Connection_Handler *
TAO_SSLIOP_Client_Transport::client_handler (void)
{
  return this->client_handler_;
}

void
TAO_SSLIOP_Client_Transport::start_request (TAO_ORB_Core * /* orb_core */,
					    TAO_Target_Specification & /* spec */,
					    TAO_OutputCDR &output,
					    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_SSLIOP_CLIENT_TRANSPORT_START_REQUEST_START);

//   const TAO_SSLIOP_Profile* profile =
//     ACE_dynamic_cast(const TAO_SSLIOP_Profile*, pfile);

//   // @@ This should be implemented in the transport object, which
//   //    would query the profile to obtain the version...
//   if (TAO_GIOP::start_message (profile->version (),
//                                TAO_GIOP::Request,
//                                output,
//                                orb_core) == 0)
  if (this->client_mesg_factory_->write_protocol_header
      (TAO_PLUGGABLE_MESSAGE_REQUEST,
       output) == 0)
    ACE_THROW (CORBA::MARSHAL ());
}

void
TAO_SSLIOP_Client_Transport::start_locate (TAO_ORB_Core * /* orb_core */,
					   TAO_Target_Specification & spec,
					   TAO_Operation_Details & opdetails,
					   TAO_OutputCDR &output,
					   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // See this is GIOP way of doing this..But anyway IIOP will be tied
  // up with GIOP.  (comment from Bala?)
  if (this->client_mesg_factory_->write_protocol_header
      (TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST,
       output) == 0)
    ACE_THROW (CORBA::MARSHAL ());

  if (this->client_mesg_factory_->write_message_header (
                                   opdetails,
                                   TAO_PLUGGABLE_MESSAGE_LOCATE_REQUEST_HEADER,
                                   spec,
                                   output) == 0)
    ACE_THROW (CORBA::MARSHAL ());
}

int
TAO_SSLIOP_Client_Transport::send_request (TAO_Stub *stub,
					   TAO_ORB_Core *orb_core,
					   TAO_OutputCDR &stream,
					   int two_way,
					   ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  two_way) == -1)
    return -1;

  if (this->client_mesg_factory_->send_message (this,
                                                stream,
                                                max_wait_time,
                                                stub,
                                                two_way) == -1)
    return -1;

  return this->idle_after_send ();
}

// Return 0, when the reply is not read fully, 1 if it is read fully.
// @@ This code should go in the TAO_Transport class is repeated for
//    each transport!!
int
TAO_SSLIOP_Client_Transport::handle_client_input (int /* block */,
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
                    "TAO (%P|%t) SSLIOP_Transport::handle_client_input -"
                    " nil message state\n"));
      return -1;
    }

  int result =
    this->client_mesg_factory_->handle_input (this,
                                              this->orb_core_,
                                              *message_state,
                                              max_wait_time);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "SSLIOP_Transport::handle_client_input, handle_input"));
      return -1;
    }
  if (result == 0)
    return result;

  // OK, the complete message is here...

  result = this->client_mesg_factory_->parse_reply (*message_state,
                                                    this->params_);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "SSLIOP_Transport::handle_client_input, parse reply"));
      message_state->reset ();
      return -1;
    }

  result =
    this->tms_->dispatch_reply (this->params_.request_id_,
                                this->params_.reply_status_,
                                message_state->giop_version,
                                this->params_.svc_ctx_,
                                message_state);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) : SSLIOP_Client_Transport::"
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

  return result;
}

int
TAO_SSLIOP_Client_Transport::register_handler (void)
{
  // @@ It seems like this method should go away, the right reactor is
  //    picked at object creation time.
  ACE_Reactor *r = this->orb_core ()->reactor ();
  if (r == this->client_handler ()->reactor ())
    return 0;

  return r->register_handler (this->client_handler (),
                              ACE_Event_Handler::READ_MASK);
}

int
TAO_SSLIOP_Client_Transport::messaging_init (CORBA::Octet major,
                                             CORBA::Octet minor)
{
  // DO NOT add support for GIOPlite to SSLIOP.  It introduces
  // security holes!

  if (this->client_mesg_factory_ == 0)
    {
      if (major == TAO_DEF_GIOP_MAJOR)
        {
          if (minor > TAO_DEF_GIOP_MINOR)
            minor = TAO_DEF_GIOP_MINOR;

          switch (minor)
            {
            case 0:
              ACE_NEW_RETURN  (this->client_mesg_factory_,
                               TAO_GIOP_Message_Connector_10,
                               0);
              break;
            case 1:
              ACE_NEW_RETURN  (this->client_mesg_factory_,
                               TAO_GIOP_Message_Connector_11,
                               0);
              break;
            default:
              if (TAO_debug_level > 0)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ASYS_TEXT ("(%N|%l|%p|%t) ")
                                     ASYS_TEXT ("No matching minor ")
                                     ASYS_TEXT ("version number \n")),
                                    0);
                }
            }
        }
      else
        {
          if (TAO_debug_level > 0)
            {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ASYS_TEXT ("(%N|%l|%p|%t) ")
                                     ASYS_TEXT ("No matching minor ")
                                     ASYS_TEXT ("version number \n")),
                                    0);
            }
        }
    }

  return 1;
}

CORBA::Boolean
TAO_SSLIOP_Client_Transport::send_request_header (
       TAO_Operation_Details &opdetails,
       TAO_Target_Specification &spec,
       TAO_OutputCDR & msg)
{
  // We are going to pass on this request to the underlying messaging
  // layer. It should take care of this request.
  CORBA::Boolean retval =
    this->client_mesg_factory_->write_message_header (
                                   opdetails,
                                   TAO_PLUGGABLE_MESSAGE_REQUEST_HEADER,
                                   spec,
                                   msg);

  return retval;
}

// *********************************************************************

ssize_t
TAO_SSLIOP_Transport::send (TAO_Stub *stub,
			    int two_way,
			    const ACE_Message_Block *message_block,
			    const ACE_Time_Value *max_wait_time)
{
  if (stub == 0 || two_way)
    {
      return this->send (message_block,
                         max_wait_time);
    }
  else
    {
      TAO_Sync_Strategy &sync_strategy = stub->sync_strategy ();

      return sync_strategy.send (*this,
                                 *stub,
                                 message_block,
                                 max_wait_time);
    }
}

ssize_t
TAO_SSLIOP_Transport::send (const ACE_Message_Block *message_block,
			    const ACE_Time_Value *max_wait_time)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_SSLIOP_TRANSPORT_SEND_START);

  // @@ This code should be refactored into ACE.cpp or something
  // similar!

  // For the most part this was copied from GIOP::send_request and
  // friends.

  iovec iov[IOV_MAX];
  int iovcnt = 0;
  ssize_t n = 0;
  ssize_t nbytes = 0;

  for (const ACE_Message_Block *i = message_block;
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
              if (max_wait_time == 0)
                n = this->handler_->peer ().sendv_n (iov,
                                                     iovcnt);
              else
                // @@ No timeouts!!!
                n = this->handler_->peer ().sendv_n (iov,
                                                     iovcnt
                                                     /*, max_wait_time */);

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
      n = this->handler_->peer ().sendv_n (iov,
                                           iovcnt);
      if (n < 1)
        return n;

      nbytes += n;
    }

  return nbytes;
}

ssize_t
TAO_SSLIOP_Transport::send (const u_char *buf,
			    size_t len,
			    const ACE_Time_Value *)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_SSLIOP_TRANSPORT_SEND_START);

  return this->handler_->peer ().send_n (buf, len);
}

ssize_t
TAO_SSLIOP_Transport::recv (char *buf,
			    size_t len,
			    const ACE_Time_Value * /* max_wait_time */)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_SSLIOP_TRANSPORT_RECEIVE_START);

  // @@ No timeouts!
  return this->handler_->peer ().recv_n (buf,
                                         len
                                         /* , max_wait_time */);
}

// Default action to be taken for send request.
int
TAO_SSLIOP_Transport::send_request (TAO_Stub *,
				    TAO_ORB_Core *  /* orb_core */,
				    TAO_OutputCDR & /* stream   */,
				    int             /* twoway   */,
				    ACE_Time_Value * /* max_wait_time */)
{
  return -1;
}

CORBA::Boolean
TAO_SSLIOP_Transport::send_request_header (TAO_Operation_Details &,
                                           TAO_Target_Specification &,
                                           TAO_OutputCDR &)
{
  // We should never be here. So return an error.
  return 0;
}
