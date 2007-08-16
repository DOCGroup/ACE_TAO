// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE u_long
TAO_Servant_Hash::operator () (PortableServer::Servant servant) const
{
  return static_cast<u_long> (reinterpret_cast<ptrdiff_t> (servant));
}

TAO_END_VERSIONED_NAMESPACE_DECL
