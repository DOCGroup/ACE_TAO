// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE
TAO_GIOP_Asynch_Invocation::TAO_GIOP_Asynch_Invocation (
    TAO_Stub *stub,
    const char *operation,
    CORBA::ULong opname_len,
    CORBA::Boolean argument_flag,
    TAO_ORB_Core *orb_core,
    int byte_order
  )
  : TAO_GIOP_Invocation (stub,
                         operation,
                         opname_len,
                         argument_flag,
                         orb_core,
                         byte_order),
    rd_ (0)
{
}

