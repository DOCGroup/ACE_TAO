// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE void
TAO_GIOP_Invocation::put_param (CORBA::TypeCode_ptr tc,
                                void *value,
                                CORBA::Environment &ACE_TRY_ENV)
{
  (void) this->out_stream_.encode (tc, value, 0, ACE_TRY_ENV);
}

ACE_INLINE TAO_OutputCDR &
TAO_GIOP_Invocation::out_stream (void)
{
  return this->out_stream_;
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Twoway_Invocation::
TAO_GIOP_Twoway_Invocation (TAO_Stub *stub,
                            const char *operation,
                            TAO_ORB_Core *orb_core)
  : TAO_GIOP_Invocation (stub, operation, orb_core),
    message_state_ (orb_core),
    rd_ (&message_state_)
{
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Twoway_Invocation::inp_stream (void)
{
  return this->message_state_.cdr;
}

ACE_INLINE void
TAO_GIOP_Twoway_Invocation::get_value (CORBA::TypeCode_ptr tc,
                                       void *value,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  (void) this->inp_stream ().decode (tc, value, 0, ACE_TRY_ENV);
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Oneway_Invocation::
TAO_GIOP_Oneway_Invocation (TAO_Stub *stub,
                            const char *operation,
                            TAO_ORB_Core *orb_core)
  : TAO_GIOP_Invocation (stub, operation, orb_core)
{
}

ACE_INLINE int
TAO_GIOP_Oneway_Invocation::invoke (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return TAO_GIOP_Invocation::invoke (0, ACE_TRY_ENV);
}

// *********************************************************************

ACE_INLINE
TAO_GIOP_Locate_Request_Invocation::
TAO_GIOP_Locate_Request_Invocation (TAO_Stub *stub,
                                    TAO_ORB_Core *orb_core)
  : TAO_GIOP_Invocation (stub, 0, orb_core),
    message_state_ (orb_core),
    rd_ (&message_state_)
{
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Locate_Request_Invocation::inp_stream (void)
{
  return this->message_state_.cdr;
}
