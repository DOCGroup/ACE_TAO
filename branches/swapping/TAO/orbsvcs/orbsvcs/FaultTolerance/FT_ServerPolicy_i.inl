//$Id$

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
  : ACE_NESTED_CLASS (CORBA, Object) (),
    ACE_NESTED_CLASS (CORBA, Policy) (),
    ACE_NESTED_CLASS (CORBA, LocalObject) (),
    FT::HeartbeatEnabledPolicy (),
    TAO_Local_RefCounted_Object (),
    heartbeat_enabled_value_ (rhs.heartbeat_enabled_value_)
{
}
