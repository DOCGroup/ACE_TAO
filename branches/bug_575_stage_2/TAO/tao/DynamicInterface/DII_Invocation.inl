// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE
TAO_GIOP_DII_Invocation::TAO_GIOP_DII_Invocation (TAO_Stub *data,
                                                  const char *operation,
                                                  CORBA::ULong opname_len,
                                                  CORBA::Boolean argument_flag,
                                                  TAO_ORB_Core *orb_core,
                                                  CORBA::Request_ptr host,
                                                  int byte_order)
  : TAO_GIOP_Twoway_Invocation (data,
                                operation,
                                opname_len,
                                argument_flag,
                                orb_core,
                                byte_order),
    host_ (host)
{
}

ACE_INLINE
TAO_GIOP_DII_Deferred_Invocation::TAO_GIOP_DII_Deferred_Invocation (
    TAO_Stub *stub,
    TAO_ORB_Core *orb_core,
    CORBA::Boolean argument_flag,
    const CORBA::Request_ptr req,
    int byte_order
  )
  : TAO_GIOP_Asynch_Invocation (stub,
                                req->operation (),
                                ACE_OS::strlen (req->operation ()),
                                argument_flag,
                                orb_core,
                                byte_order)
{
  // New reply dispatcher on the heap, because
  // we will go out of scope and hand over the
  // reply dispatcher to the ORB.
  // So this->rd_ is 0, because we do not need to
  // hold a pointer to it.
  ACE_NEW (this->rd_,
           TAO_DII_Deferred_Reply_Dispatcher (req,
                                              orb_core));
}
