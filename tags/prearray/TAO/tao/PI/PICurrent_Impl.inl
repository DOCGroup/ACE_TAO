// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO::PICurrent_Impl::copy_callback (TAO::PICurrent_Copy_Callback *cb)
{
  this->copy_callback_ = cb;
}

ACE_INLINE void
TAO::PICurrent_Impl::destruction_callback (TAO::PICurrent_Impl *p)
{
  this->destruction_callback_ = p;
}

ACE_INLINE TAO::PICurrent_Impl::Table &
TAO::PICurrent_Impl::slot_table (void)
{
  return this->slot_table_;
}

ACE_INLINE TAO::PICurrent_Impl::Table &
TAO::PICurrent_Impl::current_slot_table (void)
{
  return
    this->lc_slot_table_ == 0 ? this->slot_table_ : *this->lc_slot_table_;
}

ACE_INLINE bool
TAO::PICurrent_Impl::lc_slot_table (TAO::PICurrent_Impl *p)
{
  // Being told to lazy copy some other table?
  if (p != 0)
    {
      // Which actual table are we supposed to lazy copy?
      Table * t = &p->current_slot_table ();

      // Only if we have not already lazy copied this table
      if (t != this->lc_slot_table_)
        {
          // Whould this be a lazy copy of ourselves?
          if (t == &this->slot_table_)
            this->lc_slot_table_ = 0; // Already ourself!
          else
            {
              this->lc_slot_table_ = t;

              // Ensure remote table will tell us if it is
              // going to change or destroy itself.
              if (this != p)
                p->destruction_callback (this);
            }
        }
    }
  else
    this->lc_slot_table_ = 0;

  return (0 != this->lc_slot_table_);
}

ACE_INLINE TAO::PICurrent_Impl::Table *
TAO::PICurrent_Impl::lc_slot_table (void) const
{
  return this->lc_slot_table_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
