// $Id$

ACE_INLINE
TAO_RelativeRoundtripTimeoutPolicy_i::TAO_RelativeRoundtripTimeoutPolicy_i (
    PortableServer::POA_ptr poa,
    const TimeBase::TimeT& relative_expiry)
  :  poa_ (PortableServer::POA::_duplicate (poa)),
     relative_expiry_ (relative_expiry)
{
}
