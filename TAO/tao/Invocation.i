// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE void
TAO_GIOP_Invocation::put_param (CORBA::TypeCode_ptr tc,
                                void *value,
                                CORBA::Environment &env)
{
  (void) this->out_stream_.encode (tc, value, 0, env);
}

ACE_INLINE TAO_OutputCDR &
TAO_GIOP_Invocation::out_stream (void)
{
  return this->out_stream_;
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Twoway_Invocation::
TAO_GIOP_Twoway_Invocation (IIOP_Object *data,
			    const char *operation)
  : TAO_GIOP_Invocation (data, operation),
    inp_stream_ (CDR::DEFAULT_BUFSIZE)
{
}

ACE_INLINE void
TAO_GIOP_Twoway_Invocation::start (CORBA::Environment &env)
{
  TAO_GIOP_Invocation::start (CORBA::B_TRUE, env);
}

ACE_INLINE void
TAO_GIOP_Twoway_Invocation::get_value (CORBA::TypeCode_ptr tc,
                                void *value,
                                CORBA::Environment &env)
{
  (void) this->inp_stream_.decode (tc, value, 0, env);
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Twoway_Invocation::inp_stream (void)
{
  return this->inp_stream_;
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Oneway_Invocation::
TAO_GIOP_Oneway_Invocation (IIOP_Object *data,
			    const char *operation)
  : TAO_GIOP_Invocation (data, operation)
{
}

ACE_INLINE void
TAO_GIOP_Oneway_Invocation::start (CORBA::Environment &env)
{
  TAO_GIOP_Invocation::start (CORBA::B_FALSE, env);
}

ACE_INLINE TAO_GIOP_ReplyStatusType
TAO_GIOP_Oneway_Invocation::invoke (CORBA::Environment &env)
{
  return TAO_GIOP_Invocation::invoke (CORBA::B_FALSE, env);
}
