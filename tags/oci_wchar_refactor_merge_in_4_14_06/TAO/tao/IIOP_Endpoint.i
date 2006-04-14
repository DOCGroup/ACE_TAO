// -*- C++ -*-
//
// $Id$



ACE_INLINE const char *
TAO_IIOP_Endpoint::host (void) const
{
  return this->host_.in ();
}

ACE_INLINE CORBA::UShort
TAO_IIOP_Endpoint::port (void) const
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO_IIOP_Endpoint::port (CORBA::UShort p)
{
  return this->port_ = p;
}

#if defined (ACE_HAS_IPV6)
ACE_INLINE bool
TAO_IIOP_Endpoint::is_ipv6_decimal (void) const
{
  return this->is_ipv6_decimal_;
}
#endif /* ACE_HAS_IPV6 */
