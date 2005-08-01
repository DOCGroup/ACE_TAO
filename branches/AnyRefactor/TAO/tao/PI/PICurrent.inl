// -*- C++ -*-
//
// $Id$


ACE_INLINE PortableInterceptor::SlotId
TAO::PICurrent::slot_count (void) const
{
  return this->slot_count_;
}

ACE_INLINE void
TAO::PICurrent::initialize (PortableInterceptor::SlotId sc)
{
  this->slot_count_ = sc;
}

