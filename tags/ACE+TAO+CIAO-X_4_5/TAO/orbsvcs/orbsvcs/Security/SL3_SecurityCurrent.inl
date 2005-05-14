// -*- C++ -*-
//
// $Id$

#include "tao/ORB_Core.h"


ACE_INLINE size_t
TAO::SL3::SecurityCurrent::tss_slot (void) const
{
  return this->tss_slot_;
}

ACE_INLINE TAO::SL3::SecurityCurrent_Impl *
TAO::SL3::SecurityCurrent::implementation (void)
{
  TAO::SL3::SecurityCurrent_Impl *impl =
    static_cast<TAO::SL3::SecurityCurrent_Impl *> (
      this->orb_core_->get_tss_resource (this->tss_slot_));

  return impl;
}
