// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/IIOP_Transport.h"
#include "tao/IIOP_Connect.h"
#include "tao/IIOP_Profile.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Sync_Strategies.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#include "tao/Pluggable_Messaging.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Transport_Timeprobe_Description[] =
  {
    "IIOP_Transport::send - start",
    "IIOP_Transport::send - end",

    "IIOP_Transport::receive - start",
    "IIOP_Transport::receive - end",

    "IIOP_Client_Transport::start_request - start",
    "IIOP_Client_Transport::start_request - end"
  };

enum
  {
    TAO_IIOP_TRANSPORT_SEND_START = 1200,
    TAO_IIOP_TRANSPORT_SEND_END,

    TAO_IIOP_TRANSPORT_RECEIVE_START,
    TAO_IIOP_TRANSPORT_RECEIVE_END,

    TAO_IIOP_CLIENT_TRANSPORT_START_REQUEST_START,
    TAO_IIOP_CLIENT_TRANSPORT_START_REQUEST_END
  };


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Transport_Timeprobe_Description,
                                  TAO_IIOP_TRANSPORT_SEND_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_IIOP_Transport::TAO_IIOP_Transport (TAO_IIOP_Handler_Base *handler,
                                        TAO_ORB_Core *orb_core)
  : TAO_Transport (TAO_TAG_IIOP_PROFILE,
                   orb_core),
    handler_ (handler)
{
}

TAO_IIOP_Transport::~TAO_IIOP_Transport (void)
{
  // If the socket has not already been closed.
  if (this->handle () != ACE_INVALID_HANDLE)
    {
      // Cannot deal with errors, and therefore they are ignored.
      this->send_buffered_messages ();
    }
  else
    {
      // Dequeue messages and delete message blocks.
      this->dequeue_all ();
    }
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

ACE_Event_Handler *
TAO_IIOP_Transport::event_handler (void)
{
  return this->handler_;
}


// ****************************************************************

TAO_IIOP_Server_Transport::
    TAO_IIOP_Server_Transport (TAO_IIOP_Server_Connection_Handler *handler,
                               TAO_ORB_Core* orb_core)
  : TAO_IIOP_Transport (handler, orb_core),
    server_handler_ (handler),
    message_state_ (orb_core)
{
}

TAO_IIOP_Server_Transport::~TAO_IIOP_Server_Transport (void)
{
}

void
TAO_IIOP_Server_Transport::messaging_init (TAO_Pluggable_Messaging_Interface *mesg)
{
  this->server_mesg_factory_ = mesg;
}


// ****************************************************************

TAO_IIOP_Client_Transport::
    TAO_IIOP_Client_Transport (TAO_IIOP_Client_Connection_Handler *handler,
                               TAO_ORB_Core *orb_core)
  :  TAO_IIOP_Transport (handler,
                         orb_core),
     client_handler_ (handler)
{
}

TAO_IIOP_Client_Transport::~TAO_IIOP_Client_Transport (void)
{
}



TAO_IIOP_Client_Connection_Handler *
TAO_IIOP_Client_Transport::client_handler (void)
{
  return this->client_handler_;
}

void
TAO_IIOP_Client_Transport::start_request (TAO_ORB_Core */*orb_core*/,
                                          TAO_Target_Specification & /*spec */,
                                          TAO_OutputCDR &output,
                                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_CLIENT_TRANSPORT_START_REQUEST_START);

  /*const TAO_IIOP_Profile* profile =
    ACE_dynamic_cast(const TAO_IIOP_Profile*, pfile);

  // @@ This should be implemented in the transport object, which
  //    would query the profile to obtain the version...
  if (TAO_GIOP::start_message (profile->version (),
                               TAO_GIOP::Request,
                               output,
                               orb_core) == 0)*/
  if (this->client_mesg_factory_->write_protocol_header
      (TAO_PLUGGABLE_MESSAGE_REQUEST, 
       output) == 0)
    ACE_THROW (CORBA::MARSHAL ());
}

void
TAO_IIOP_Client_Transport::start_locate (TAO_ORB_Core */*orb_core*/,
                                         TAO_Target_Specification &spec,
                                         CORBA::ULong request_id,
                                         TAO_OutputCDR &output,
                                         CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  /*  const TAO_IIOP_Profile* profile =
      ACE_dynamic_cast(const TAO_IIOP_Profile*, pfile); */

  // Obtain object key.
  //const TAO_ObjectKey& key = profile->object_key ();

  // @@ This should be implemented in the transport object, which
  //    would query the profile to obtain the version...
  /*if (TAO_GIOP::start_message (profile->version (),
                               TAO_GIOP::LocateRequest,
                               output,
                               orb_core) == 0)*/

  // See this is GIOP way of doing this..But anyway IIOP will be tied
  // up with GIOP. 
  if (this->client_mesg_factory_->write_protocol_header
      (TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST, 
       output) == 0)
    ACE_THROW (CORBA::MARSHAL ());
  
  TAO_Pluggable_Connector_Params params;
  params.request_id = request_id;
  if (this->client_mesg_factory_->write_message_header (params,
                                                        TAO_PLUGGABLE_MESSAGE_LOCATE_REQUEST_HEADER,   
                                                        spec,
                                                        output) == 0)
    ACE_THROW (CORBA::MARSHAL ());
}

int
TAO_IIOP_Client_Transport::send_request (TAO_Stub *stub,
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
TAO_IIOP_Client_Transport::handle_client_input (int /* block */,
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
                    ASYS_TEXT ("TAO (%P|%t) IIOP_Transport::handle_client_input -")
                    ASYS_TEXT (" nil message state\n")));
      return -1;
    }

  int result = this->client_mesg_factory_->handle_input (this,
                                                         this->orb_core_,
                                                         *message_state,
                                                         max_wait_time);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - %p\n"),
                    ASYS_TEXT ("IIOP_Transport::handle_client_input, handle_input")));
      return -1;
    }
  if (result == 0)
    return result;

  // OK, the complete message is here...

  TAO_Pluggable_Connector_Params params;
  CORBA::ULong reply_status;

  result = this->client_mesg_factory_->parse_reply (*message_state,
                                                    params,
                                                    reply_status);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - %p\n"),
                    ASYS_TEXT ("IIOP_Transport::handle_client_input, parse reply")));
      message_state->reset ();
      return -1;
    }

  result =
    this->tms_->dispatch_reply (params.request_id,
                                reply_status,
                                message_state->giop_version,
                                params.svc_ctx,
                                message_state);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("TAO (%P|%t) : IIOP_Client_Transport::")
                    ASYS_TEXT ("handle_client_input - ")
                    ASYS_TEXT ("dispatch reply failed\n")));
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
TAO_IIOP_Client_Transport::register_handler (void)
{
  // @@ It seems like this method should go away, the right reactor is
  //    picked at object creation time.
  ACE_Reactor *r = this->orb_core ()->reactor ();
  if (r == this->client_handler ()->reactor ())
    return 0;

  return r->register_handler (this->client_handler (),
                              ACE_Event_Handler::READ_MASK);
}

void
TAO_IIOP_Client_Transport::messaging_init (TAO_Pluggable_Messaging_Interface *mesg)
{
  this->client_mesg_factory_ = mesg;
}

CORBA::Boolean
TAO_IIOP_Client_Transport::send_request_header (const IOP::ServiceContextList & svc_ctx,  
                                                CORBA::ULong request_id,
                                                CORBA::Octet response_flags,
                                                TAO_Target_Specification &spec,
                                                const char* opname,
                                                TAO_OutputCDR & msg)
{
  TAO_Pluggable_Connector_Params params;
  params.svc_ctx = svc_ctx;
  params.request_id = request_id;
  params.response_flags = response_flags;
  params.operation_name = opname;
  // We are going to pass on this request to the underlying messaging
  // layer. It should take care of this request
    CORBA::Boolean retval = 
      this->client_mesg_factory_->write_message_header (params,
                                                        TAO_PLUGGABLE_MESSAGE_REQUEST_HEADER,  
                                                        spec,
                                                        msg);
  
  return retval;
}


// *********************************************************************

ssize_t
TAO_IIOP_Transport::send (TAO_Stub *stub,
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
TAO_IIOP_Transport::send (const ACE_Message_Block *message_block,
                          const ACE_Time_Value *max_wait_time)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_SEND_START);

  return ACE::send_n (this->handle (),
                      message_block,
                      max_wait_time);
}

ssize_t
TAO_IIOP_Transport::send (const u_char *buf,
                          size_t len,
                          const ACE_Time_Value *max_wait_time)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_SEND_START);

  return this->handler_->peer ().send_n (buf,
                                         len,
                                         max_wait_time);
}

ssize_t
TAO_IIOP_Transport::recv (char *buf,
                          size_t len,
                          const ACE_Time_Value *max_wait_time)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_IIOP_TRANSPORT_RECEIVE_START);

  return this->handler_->peer ().recv_n (buf,
                                         len,
                                         max_wait_time);
}

// Default action to be taken for send request.
int
TAO_IIOP_Transport::send_request (TAO_Stub *,
                                  TAO_ORB_Core *  /* orb_core */,
                                  TAO_OutputCDR & /* stream   */,
                                  int             /* twoway   */,
                                  ACE_Time_Value * /* max_wait_time */)
{
  return -1;
}


                                         
CORBA::Boolean
TAO_IIOP_Transport::send_request_header (const IOP::ServiceContextList & /*svc_ctx*/,  
                                         CORBA::ULong /*request_id*/,
                                         CORBA::Octet /*response_flags*/,
                                         TAO_Target_Specification & /*spec */ ,
                                         const char* /*opname*/,
                                         TAO_OutputCDR & /*msg*/)
{
  // We should never be here. So return an error.
  return 0;
}
