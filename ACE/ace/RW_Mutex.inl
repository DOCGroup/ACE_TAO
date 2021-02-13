// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const ACE_rwlock_t &
ACE_RW_Mutex::lock () const
{
// ACE_TRACE ("ACE_RW_Mutex::lock");
  return this->lock_;
}

ACE_INLINE int
ACE_RW_Mutex::remove ()
{
// ACE_TRACE ("ACE_RW_Mutex::remove");
  int result = 0;
  if (!this->removed_)
    {
      this->removed_ = true;
      result = ACE_OS::rwlock_destroy (&this->lock_);
    }
  return result;
}

ACE_INLINE int
ACE_RW_Mutex::acquire_read ()
{
// ACE_TRACE ("ACE_RW_Mutex::acquire_read");
  return ACE_OS::rw_rdlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::acquire_write ()
{
// ACE_TRACE ("ACE_RW_Mutex::acquire_write");
  return ACE_OS::rw_wrlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::acquire ()
{
// ACE_TRACE ("ACE_RW_Mutex::acquire");
  return ACE_OS::rw_wrlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::tryacquire_read ()
{
// ACE_TRACE ("ACE_RW_Mutex::tryacquire_read");
  return ACE_OS::rw_tryrdlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::tryacquire_write ()
{
// ACE_TRACE ("ACE_RW_Mutex::tryacquire_write");
  return ACE_OS::rw_trywrlock (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::tryacquire_write_upgrade ()
{
// ACE_TRACE ("ACE_RW_Mutex::tryacquire_write_upgrade");
  return ACE_OS::rw_trywrlock_upgrade (&this->lock_);
}

ACE_INLINE int
ACE_RW_Mutex::tryacquire ()
{
// ACE_TRACE ("ACE_RW_Mutex::tryacquire");
  return this->tryacquire_write ();
}

ACE_INLINE int
ACE_RW_Mutex::release ()
{
// ACE_TRACE ("ACE_RW_Mutex::release");
  return ACE_OS::rw_unlock (&this->lock_);
}

ACE_END_VERSIONED_NAMESPACE_DECL
