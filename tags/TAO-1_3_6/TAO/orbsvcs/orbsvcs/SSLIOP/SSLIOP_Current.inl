// -*- C++ -*-
//
// $Id$

ACE_INLINE void
TAO_SSLIOP_Current::tss_slot (size_t slot)
{
  this->tss_slot_ = slot;
}

ACE_INLINE int
TAO_SSLIOP_Current::implementation (TAO_SSLIOP_Current_Impl *impl)
{
  if (this->orb_core_ == 0)
    return -1;

  return this->orb_core_->set_tss_resource (this->tss_slot_, impl);
}

ACE_INLINE TAO_SSLIOP_Current_Impl *
TAO_SSLIOP_Current::implementation (void)
{
  if (this->orb_core_ == 0)
    return 0;

  TAO_Security_Current_Impl *impl =
    ACE_static_cast (TAO_Security_Current_Impl *,
                     this->orb_core_->get_tss_resource (this->tss_slot_));

  // Make sure we've got SSL session state in TSS before allowing
  // further use of the SSLIOP::Current object.
  if (impl != 0 && impl->tag () == SSLIOP::TAG_SSL_SEC_TRANS)
    return ACE_dynamic_cast (TAO_SSLIOP_Current_Impl *,
                             impl);

  return 0;
}
