// -*- C++ -*-
//
// $Id$

ACE_INLINE int
TAO_SSLIOP_Current::implementation (TAO_SSLIOP_Current_Impl *impl)
{
  if (this->orb_core_ == 0 && this->init () != 0)
    return -1;

  return this->orb_core_->set_tss_resource (this->tss_slot_, impl);
}

ACE_INLINE TAO_SSLIOP_Current_Impl *
TAO_SSLIOP_Current::implementation (void)
{
  if (this->orb_core_ == 0 && this->init () != 0)
    return 0;

  TAO_SSLIOP_Current_Impl *impl =
    ACE_static_cast (TAO_SSLIOP_Current_Impl *,
                     this->orb_core_->get_tss_resource (this->tss_slot_));

  return impl;
}
