// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

#if (TAO_HAS_AMI_CALLBACK == 1)

ACE_INLINE
TAO_GIOP_Twoway_Asynch_Invocation::TAO_GIOP_Twoway_Asynch_Invocation (
    TAO_Stub *stub,
    const char *operation,
    CORBA::ULong opname_len,
    CORBA::Boolean argument_flag,
    TAO_ORB_Core *orb_core,
    const TAO_Reply_Handler_Skeleton &reply_handler_skel,
    Messaging::ReplyHandler_ptr reply_handler_ptr
  )
  : TAO_GIOP_Asynch_Invocation (stub,
                                operation,
                                opname_len,
                                argument_flag,
                                orb_core)
{
  // If the reply handler is nil, we do not create a reply dispatcher.
  // The ORB will drop replies to which it cannot associate a reply
  // dispatcher.
  if (!CORBA::is_nil (reply_handler_ptr))
    {
			// New reply dispatcher on the heap, because
			// we will go out of scope and hand over the
			// reply dispatcher to the ORB.

      // @@Michael: What does following comment mean?
			// So this->rd_ is 0, because we do not need to
			// hold a pointer to it.
			ACE_NEW (this->rd_,
               TAO_Asynch_Reply_Dispatcher (reply_handler_skel,
                                            reply_handler_ptr,
                                            orb_core));
    }
}

#endif /* TAO_HAS_AMI_CALLBACK == 1*/
