// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO::True_RefCount_Policy::True_RefCount_Policy (void)
  : lock_ (),
    refcount_ (1)
{
}

ACE_INLINE
TAO::True_RefCount_Policy::add_ref (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

  ++this->refcount_;
}

ACE_INLINE
TAO::True_RefCount_Policy::remove_ref (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

    --this->refcount_;

    if (this->refcount_ != 0)
      return;
  }

  delete this;
}
