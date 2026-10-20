// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO::SSLIOP::Current::tss_slot (size_t slot)
{
  this->tss_slot_ = slot;
}

ACE_INLINE int
TAO::SSLIOP::Current::implementation (TAO::SSLIOP::Current_Impl *impl)
{
  if (this->orb_core_ == nullptr)
    return -1;

  return this->orb_core_->set_tss_resource (this->tss_slot_, impl);
}

ACE_INLINE TAO::SSLIOP::Current_Impl *
TAO::SSLIOP::Current::implementation ()
{
  if (this->orb_core_ == nullptr)
    return nullptr;

  TAO::SL3::SecurityCurrent_Impl *impl =
    static_cast<TAO::SL3::SecurityCurrent_Impl *> (
      this->orb_core_->get_tss_resource (this->tss_slot_));

  // Make sure we've got SSL session state in TSS before allowing
  // further use of the SSLIOP::Current object.
  if (impl != nullptr && impl->tag () == ::SSLIOP::TAG_SSL_SEC_TRANS)
    return dynamic_cast<TAO::SSLIOP::Current_Impl *> (impl);

  return nullptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
