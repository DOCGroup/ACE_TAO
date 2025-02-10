// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_ORB_Core *
TAO_Connector::orb_core ()
{
  return this->orb_core_;
}

ACE_INLINE void
TAO_Connector::orb_core (TAO_ORB_Core *orb_core)
{
  this->orb_core_ = orb_core;
}

ACE_INLINE CORBA::ULong
TAO_Connector::tag () const
{
  return this->tag_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
