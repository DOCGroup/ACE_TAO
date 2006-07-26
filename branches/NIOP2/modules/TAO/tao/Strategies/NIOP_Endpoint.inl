// -*- C++ -*-
//
// $Id: NIOP_Endpoint.i,v 1.7 2005/11/04 18:39:05 ossama Exp $

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const char *
TAO_NIOP_Endpoint::host (void) const
{
  return this->host_.in ();
}

ACE_INLINE CORBA::UShort
TAO_NIOP_Endpoint::port (void) const
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO_NIOP_Endpoint::port (CORBA::UShort p)
{
  return this->port_ = p;
}

TAO_END_VERSIONED_NAMESPACE_DECL
