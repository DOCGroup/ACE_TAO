// -*- C++ -*-
//
// $Id$

ACE_INLINE u_long
TAO_Servant_Hash::operator () (PortableServer::Servant servant) const
{
  return static_cast<u_long> (reinterpret_cast<ptrdiff_t> (servant));
}
