// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO_ServantBase_var::TAO_ServantBase_var (void)
  : ptr_ (0)
{
}

ACE_INLINE
TAO_ServantBase_var::TAO_ServantBase_var (TAO_ServantBase *p)
  : ptr_ (p)
{
}

ACE_INLINE TAO_ServantBase *
TAO_ServantBase_var::operator->() const
{
  return this->ptr_;
}

ACE_INLINE TAO_ServantBase *
TAO_ServantBase_var::in (void) const
{
  return this->ptr_;
}

ACE_INLINE TAO_ServantBase *&
TAO_ServantBase_var::inout (void)
{
  return this->ptr_;
}

// -------------------------------------------------------------------

ACE_INLINE u_long
TAO_Servant_Hash::operator () (PortableServer::Servant servant) const
{
  return static_cast<u_long> (reinterpret_cast<ptrdiff_t> (servant));
}

