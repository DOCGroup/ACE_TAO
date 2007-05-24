// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_FT_Request_Duration_Policy::TAO_FT_Request_Duration_Policy (
    const TimeBase::TimeT &duration
  )
  : request_duration_ (duration)
{
}

ACE_INLINE
TAO_FT_Request_Duration_Policy::TAO_FT_Request_Duration_Policy (
    const TAO_FT_Request_Duration_Policy &rhs
  )
  : CORBA::Object (),
    CORBA::Policy (),
    CORBA::LocalObject (),
    FT::RequestDurationPolicy (),
    TAO_Local_RefCounted_Object (),
    request_duration_ (rhs.request_duration_)
{
}

ACE_INLINE
TAO_FT_Heart_Beat_Policy::TAO_FT_Heart_Beat_Policy (
    const CORBA::Boolean heartbeat,
    const TimeBase::TimeT &interval,
    const TimeBase::TimeT &timeout
  )
  : heartbeat_ (heartbeat),
    heartbeat_interval_ (interval),
    heartbeat_timeout_ (timeout)
{
}

ACE_INLINE
TAO_FT_Heart_Beat_Policy::TAO_FT_Heart_Beat_Policy (
    const TAO_FT_Heart_Beat_Policy &rhs
  )
  : CORBA::Object (),
    CORBA::Policy (),
    CORBA::LocalObject (),
    FT::HeartbeatPolicy (),
    TAO_Local_RefCounted_Object (),
    heartbeat_ (rhs.heartbeat_),
    heartbeat_interval_ (rhs.heartbeat_interval_),
    heartbeat_timeout_ (rhs.heartbeat_timeout_)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
