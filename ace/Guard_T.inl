/* -*- C++ -*- */
// $Id$

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::acquire (void)
{
  return this->owner_ = this->lock_->acquire ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::tryacquire (void)
{
  return this->owner_ = this->lock_->tryacquire ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::release (void)
{
  if (this->owner_ == -1)
    return -1;
  else
    {
      this->owner_ = -1;
      return this->lock_->release ();
    }
}

template <class ACE_LOCK> ACE_INLINE
ACE_Guard<ACE_LOCK>::ACE_Guard (ACE_LOCK &l)
  : lock_ (&l),
    owner_ (0)
{
  this->acquire ();
}

template <class ACE_LOCK> ACE_INLINE
ACE_Guard<ACE_LOCK>::ACE_Guard (ACE_LOCK &l, int block)
  : lock_ (&l),
    owner_ (0)
{
  if (block)
    this->acquire ();
  else
    this->tryacquire ();
}

template <class ACE_LOCK> ACE_INLINE
ACE_Guard<ACE_LOCK>::ACE_Guard (ACE_LOCK &l, int block, int become_owner)
  : lock_ (&l),
    owner_ (become_owner == 0 ? -1 : 0)
{
  ACE_UNUSED_ARG (block);
}

// Implicitly and automatically acquire (or try to acquire) the
// lock.

template <class ACE_LOCK> ACE_INLINE
ACE_Guard<ACE_LOCK>::~ACE_Guard (void)
{
  this->release ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::locked (void) const
{
  return this->owner_ != -1;
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Guard<ACE_LOCK>::remove (void)
{
  return this->lock_->remove ();
}

template <class ACE_LOCK> ACE_INLINE void
ACE_Guard<ACE_LOCK>::disown (void)
{
  this->owner_ = -1;
}

template <class ACE_LOCK> ACE_INLINE
ACE_Write_Guard<ACE_LOCK>::ACE_Write_Guard (ACE_LOCK &m)
  : ACE_Guard<ACE_LOCK> (&m)
{
  this->acquire_write ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Write_Guard<ACE_LOCK>::acquire_write (void)
{
  return this->owner_ = this->lock_->acquire_write ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Write_Guard<ACE_LOCK>::acquire (void)
{
  return this->owner_ = this->lock_->acquire_write ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Write_Guard<ACE_LOCK>::tryacquire_write (void)
{
  return this->owner_ = this->lock_->tryacquire_write ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Write_Guard<ACE_LOCK>::tryacquire (void)
{
  return this->owner_ = this->lock_->tryacquire_write ();
}

template <class ACE_LOCK> ACE_INLINE
ACE_Write_Guard<ACE_LOCK>::ACE_Write_Guard (ACE_LOCK &m,
                                            int block)
  : ACE_Guard<ACE_LOCK> (&m)
{
  if (block)
    this->acquire_write ();
  else
    this->tryacquire_write ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Read_Guard<ACE_LOCK>::acquire_read (void)
{
  return this->owner_ = this->lock_->acquire_read ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Read_Guard<ACE_LOCK>::acquire (void)
{
  return this->owner_ = this->lock_->acquire_read ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Read_Guard<ACE_LOCK>::tryacquire_read (void)
{
  return this->owner_ = this->lock_->tryacquire_read ();
}

template <class ACE_LOCK> ACE_INLINE int
ACE_Read_Guard<ACE_LOCK>::tryacquire (void)
{
  return this->owner_ = this->lock_->tryacquire_read ();
}

template <class ACE_LOCK> ACE_INLINE
ACE_Read_Guard<ACE_LOCK>::ACE_Read_Guard (ACE_LOCK &m)
  : ACE_Guard<ACE_LOCK> (&m)
{
  this->acquire_read ();
}

template <class ACE_LOCK> ACE_INLINE
ACE_Read_Guard<ACE_LOCK>::ACE_Read_Guard (ACE_LOCK &m,
                                          int block)
  : ACE_Guard<ACE_LOCK> (&m)
{
  if (block)
    this->acquire_read ();
  else
    this->tryacquire_read ();
}
