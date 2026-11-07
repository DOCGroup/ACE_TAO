// -*- C++ -*-
//
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_FT_Request_Duration_Policy::TAO_FT_Request_Duration_Policy (
    TimeBase::TimeT const &duration
  )
  : request_duration_ (duration)
{
}

ACE_INLINE
TAO_FT_Request_Duration_Policy::TAO_FT_Request_Duration_Policy (
    TAO_FT_Request_Duration_Policy const &rhs
  )
  : CORBA::Object (),
    CORBA::Policy (),
    FT::RequestDurationPolicy (),
    CORBA::LocalObject (),
    request_duration_ (rhs.request_duration_)
{
}

ACE_INLINE
TAO_FT_Heart_Beat_Policy::TAO_FT_Heart_Beat_Policy (
    CORBA::Boolean const heartbeat,
    TimeBase::TimeT const &interval,
    TimeBase::TimeT const &timeout
  )
  : heartbeat_ (heartbeat),
    heartbeat_interval_ (interval),
    heartbeat_timeout_ (timeout)
{
}

ACE_INLINE
TAO_FT_Heart_Beat_Policy::TAO_FT_Heart_Beat_Policy (
    TAO_FT_Heart_Beat_Policy const &rhs
  )
  : CORBA::Object (),
    CORBA::Policy (),
    FT::HeartbeatPolicy (),
    CORBA::LocalObject (),
    heartbeat_ (rhs.heartbeat_),
    heartbeat_interval_ (rhs.heartbeat_interval_),
    heartbeat_timeout_ (rhs.heartbeat_timeout_)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
