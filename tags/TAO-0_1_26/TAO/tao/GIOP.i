// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE
TAO_GIOP_LocateRequestHeader::TAO_GIOP_LocateRequestHeader (void)
  : request_id (0)
{}

ACE_INLINE void
TAO_GIOP_Invocation::get_value (CORBA::TypeCode_ptr tc,
                                void *value,
                                CORBA::Environment &env)
{
  (void) this->inp_stream_.decode (tc, value, 0, env);
}

ACE_INLINE void
TAO_GIOP_Invocation::put_param (CORBA::TypeCode_ptr tc,
                                void *value,
                                CORBA::Environment &env)
{
  (void) this->out_stream_.encode (tc, value, 0, env);
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Invocation::inp_stream (void)
{
  return this->inp_stream_;
}

ACE_INLINE TAO_OutputCDR &
TAO_GIOP_Invocation::out_stream (void)
{
  return this->out_stream_;
}

#if 0
// @@ TODO
ACE_INLINE CDR &
TAO_GIOP_Invocation::stream (void)
{
  return this->stream_;
}
#endif /* 0 */
