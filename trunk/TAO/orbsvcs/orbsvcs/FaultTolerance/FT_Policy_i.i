//$Id$
ACE_INLINE
TAO_FT_Request_Duration_Policy::TAO_FT_Request_Duration_Policy (const TimeBase::TimeT &duration)
  : request_duration_ (duration)
{
}

ACE_INLINE
TAO_FT_Request_Duration_Policy::TAO_FT_Request_Duration_Policy (const TAO_FT_Request_Duration_Policy &rhs)
  : FT::RequestDurationPolicy (),
    TAO_Local_RefCounted_Object (),
    request_duration_ (rhs.request_duration_)
{
}
