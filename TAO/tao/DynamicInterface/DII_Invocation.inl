// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE
TAO_GIOP_DII_Deferred_Invocation::
TAO_GIOP_DII_Deferred_Invocation (TAO_Stub *stub,
                                  TAO_ORB_Core *orb_core,
                                  const CORBA::Request_ptr req)
  : TAO_GIOP_Invocation (stub,
                         req->operation (),
                         ACE_OS::strlen (req->operation ()),
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
