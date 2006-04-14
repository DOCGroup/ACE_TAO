// -*- C++ -*-
//
// $Id$

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

ACE_INLINE void
TAO::PICurrent_Impl::lc_slot_table (TAO::PICurrent_Impl *p)
{
  if (p != 0)
    {
      Table * t = &p->current_slot_table ();

      if (t != this->lc_slot_table_)
        {
          this->lc_slot_table_ = t;

          if (this != p)
            p->destruction_callback (this);
        }
      else
        this->lc_slot_table_ = 0;
    }
  else
    this->lc_slot_table_ = 0;
}

ACE_INLINE TAO::PICurrent_Impl::Table *
TAO::PICurrent_Impl::lc_slot_table (void) const
{
  return this->lc_slot_table_;
}
