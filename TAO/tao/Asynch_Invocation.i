// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

#if (TAO_HAS_CORBA_MESSAGING == 1)

#  if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

ACE_INLINE
TAO_GIOP_Twoway_Asynch_Invocation::
TAO_GIOP_Twoway_Asynch_Invocation (TAO_Stub *stub,
                                   const char *operation,
                                   TAO_ORB_Core *orb_core,
                                   const TAO_Reply_Handler_Skeleton &reply_handler_skel,
                                   Messaging::ReplyHandler_ptr reply_handler_ptr)
  : TAO_GIOP_Invocation (stub,
                         operation,
                         orb_core),
    rd_ (0)
{
  // New reply dispatcher on the heap, because
  // we will go out of scope and hand over the
  // reply dispatcher to the ORB.
  // So this->rd_ is 0, because we do not need to
  // hold a pointer to it.
  ACE_NEW (rd_,
           TAO_Asynch_Reply_Dispatcher (reply_handler_skel,
                                        reply_handler_ptr));
}

#  endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

//****************************************************************************

#if !defined (TAO_HAS_MINIMUM_CORBA)

ACE_INLINE
TAO_GIOP_DII_Deferred_Invocation::
TAO_GIOP_DII_Deferred_Invocation (TAO_Stub *stub,
                                  TAO_ORB_Core *orb_core,
                                  const CORBA::Request_ptr req)
  : TAO_GIOP_Invocation (stub,
                         req->operation (),
                         orb_core),
    rd_ (0)
{
  // New reply dispatcher on the heap, because
  // we will go out of scope and hand over the
  // reply dispatcher to the ORB.
  // So this->rd_ is 0, because we do not need to
  // hold a pointer to it.
  ACE_NEW (rd_,
           TAO_DII_Deferred_Reply_Dispatcher (req));
}

#endif /* TAO_HAS_MINIMUM_CORBA */
