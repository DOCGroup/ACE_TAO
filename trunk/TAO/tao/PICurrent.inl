// -*- C++ -*-
//
// $Id$

ACE_INLINE PortableInterceptor::SlotId
TAO_PICurrent::slot_count (void) const
{
  return this->slot_count_;
}

ACE_INLINE void
TAO_PICurrent::check_validity (const PortableInterceptor::SlotId &id
                               ACE_ENV_ARG_DECL)
{
  // No need to acquire a lock for this check.  At this point, these
  // attributes are read only.
  if (id >= this->slot_count_)
    ACE_THROW (PortableInterceptor::InvalidSlot ());
}

// ------------------------------------------------------------------

ACE_INLINE TAO_PICurrent_Impl *
TAO_PICurrent_Impl::pi_peer (void)
{
  return this->pi_peer_;
}

ACE_INLINE void
TAO_PICurrent_Impl::pi_peer (TAO_PICurrent_Impl *peer)
{
  this->pi_peer_ = peer;
}

ACE_INLINE TAO_PICurrent_Impl::Table &
TAO_PICurrent_Impl::slot_table (void)
{
  return this->slot_table_;
}

ACE_INLINE void
TAO_PICurrent_Impl::dirty (CORBA::Boolean d)
{
  this->dirty_ = d;
}

ACE_INLINE CORBA::Boolean
TAO_PICurrent_Impl::dirty (void) const
{
  return this->dirty_;
}
