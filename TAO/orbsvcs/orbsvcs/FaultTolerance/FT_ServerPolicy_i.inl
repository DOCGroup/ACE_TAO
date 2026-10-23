// -*- C++ -*-
//
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_FT_Heart_Beat_Enabled_Policy::TAO_FT_Heart_Beat_Enabled_Policy (
    CORBA::Boolean const heartbeat
  )
  : heartbeat_enabled_value_ (heartbeat)
{
}

ACE_INLINE
TAO_FT_Heart_Beat_Enabled_Policy::TAO_FT_Heart_Beat_Enabled_Policy (
    TAO_FT_Heart_Beat_Enabled_Policy const &rhs
  )
  : CORBA::Object (),
    CORBA::Policy (),
    FT::HeartbeatEnabledPolicy (),
    CORBA::LocalObject (),
    heartbeat_enabled_value_ (rhs.heartbeat_enabled_value_)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
