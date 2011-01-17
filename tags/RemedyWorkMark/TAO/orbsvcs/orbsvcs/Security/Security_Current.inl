// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE size_t
TAO_Security_Current::tss_slot (void) const
{
  return this->tss_slot_;
}

ACE_INLINE TAO::Security::Current_Impl *
TAO_Security_Current::implementation (void)
{
  if (this->orb_core_ == 0 && this->init () != 0)
    return 0;

  TAO::Security::Current_Impl *impl =
    static_cast<TAO::Security::Current_Impl *> (
      this->orb_core_->get_tss_resource (this->tss_slot_));

  return impl;
}

TAO_END_VERSIONED_NAMESPACE_DECL
