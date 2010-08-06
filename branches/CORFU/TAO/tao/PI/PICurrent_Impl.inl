// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::PICurrent_Impl::PICurrent_Impl ()
  : slot_table_ (),
    lazy_copy_ (0),
    impending_change_callback_ (0)
{
}

ACE_INLINE
TAO::PICurrent_Impl::~PICurrent_Impl ()
{
  // Break any existing ties that another PICurrent has with our table
  // since our table will no longer exist once this destructor completes.
  if (0 != this->impending_change_callback_)
    this->impending_change_callback_->convert_from_lazy_to_real_copy ();

  // If we have logically copied another table, ensure it is told about our
  // demise so that it will not call our non-existant
  // convert_from_lazy_to_real_copy() when it changes/destructs.
  if (0 != this->lazy_copy_)
    this->lazy_copy_->set_callback_for_impending_change (0);
}

ACE_INLINE void
TAO::PICurrent_Impl::convert_from_lazy_to_real_copy ()
{
  // Make sure we take a physical copy of the existing logical
  // copy of the table before it disappears/changes.
  if (0 != this->lazy_copy_)
    {
      this->slot_table_ = this->lazy_copy_->current_slot_table ();

      // Must tell the old copied PICurrent_Impl that we no
      // longer want to be told when/if it is going to be
      // changed or destroyed.
      this->lazy_copy_->set_callback_for_impending_change (0);
      this->lazy_copy_ = 0;
    }
}

ACE_INLINE void
TAO::PICurrent_Impl::set_callback_for_impending_change (TAO::PICurrent_Impl *p)
{
  this->impending_change_callback_ = (this == p) ? 0 : p;
}

ACE_INLINE TAO::PICurrent_Impl::Table &
TAO::PICurrent_Impl::current_slot_table ()
{
  return (0 == this->lazy_copy_) ?
         this->slot_table_ :
         this->lazy_copy_->current_slot_table ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
