// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE
TAO_GIOP_Twoway_Asynch_Invocation::
TAO_GIOP_Twoway_Asynch_Invocation (TAO_Stub *stub,
                                   const char *operation,
                                   TAO_ORB_Core *orb_core,
                                   const TAO_Reply_Handler_Skeleton &reply_handler_skel,
                                   Messaging::ReplyHandler_ptr reply_handler_ptr)
  : TAO_GIOP_Invocation (stub, operation, orb_core),
    message_state_ (0),
    rd_ (0)
{
  // Make a new message state.
  ACE_NEW (message_state_,
           TAO_GIOP_Message_State (orb_core));
  
  // New Reply Dispatcher.
  ACE_NEW (rd_,
           TAO_Asynch_Reply_Dispatcher (message_state_,
                                        reply_handler_skel,
                                        reply_handler_ptr));
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Twoway_Asynch_Invocation::inp_stream (void)
{
  return this->message_state_->cdr;
}

ACE_INLINE void
TAO_GIOP_Twoway_Asynch_Invocation::get_value (CORBA::TypeCode_ptr tc,
                                              void *value,
                                              CORBA::Environment &ACE_TRY_ENV)
{
  (void) this->inp_stream ().decode (tc, value, 0, ACE_TRY_ENV);
}
