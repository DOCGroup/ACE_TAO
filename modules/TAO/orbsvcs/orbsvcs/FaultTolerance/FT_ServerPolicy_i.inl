// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_FT_Heart_Beat_Enabled_Policy::TAO_FT_Heart_Beat_Enabled_Policy (
    const CORBA::Boolean heartbeat
  )
  : heartbeat_enabled_value_ (heartbeat)
{
}

ACE_INLINE
TAO_FT_Heart_Beat_Enabled_Policy::TAO_FT_Heart_Beat_Enabled_Policy (
    const TAO_FT_Heart_Beat_Enabled_Policy &rhs
  )
  : CORBA::Object (),
    CORBA::Policy (),
    FT::HeartbeatEnabledPolicy (),
    CORBA::LocalObject (),
    heartbeat_enabled_value_ (rhs.heartbeat_enabled_value_)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
