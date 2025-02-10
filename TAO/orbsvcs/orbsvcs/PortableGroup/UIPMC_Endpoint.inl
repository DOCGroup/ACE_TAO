// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const ACE_INET_Addr &
TAO_UIPMC_Endpoint::object_addr () const
{
  return this->object_addr_;
}

ACE_INLINE CORBA::UShort
TAO_UIPMC_Endpoint::port () const
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO_UIPMC_Endpoint::port (CORBA::UShort p)
{
  return this->port_ = p;
}

ACE_INLINE const char*
TAO_UIPMC_Endpoint::preferred_if () const
{
  return this->preferred_if_.in();
}

TAO_END_VERSIONED_NAMESPACE_DECL
