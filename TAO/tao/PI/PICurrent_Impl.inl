// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::PICurrent_Impl::PICurrent_Impl (
    TAO_ORB_Core   *orb_core,
    size_t         tss_slot,
    PICurrent_Impl *pop)
  : orb_core_ (orb_core),
    tss_slot_ (tss_slot),
    pop_ (pop),
    push_ (nullptr),
    slot_table_ (),
    lazy_copy_ (nullptr),
    impending_change_callback_ (nullptr)
{
}

ACE_INLINE void
TAO::PICurrent_Impl::convert_from_lazy_to_real_copy ()
{
  // Make sure we take a physical copy of the existing logical
  // copy of the table before it disappears/changes.
  if (nullptr != this->lazy_copy_)
    {
      this->slot_table_ = this->lazy_copy_->current_slot_table ();

      // Must tell the old copied PICurrent_Impl that we no
      // longer want to be told when/if it is going to be
      // changed or destroyed.
      this->lazy_copy_->set_callback_for_impending_change (nullptr);
      this->lazy_copy_ = nullptr;
    }
}

ACE_INLINE void
TAO::PICurrent_Impl::set_callback_for_impending_change (TAO::PICurrent_Impl *p)
{
  this->impending_change_callback_ = (this == p) ? nullptr : p;
}

ACE_INLINE TAO::PICurrent_Impl::Table &
TAO::PICurrent_Impl::current_slot_table ()
{
  return (nullptr == this->lazy_copy_) ?
         this->slot_table_ :
         this->lazy_copy_->current_slot_table ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
