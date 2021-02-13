#include "ace/Lock.h"

#if !defined (__ACE_INLINE__)
#include "ace/Lock.inl"
#endif /* __ACE_INLINE__ */



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Lock::~ACE_Lock ()
{
}

ACE_Adaptive_Lock::ACE_Adaptive_Lock ()
  : lock_ (0)
{
}

ACE_Adaptive_Lock::~ACE_Adaptive_Lock ()
{
}

int
ACE_Adaptive_Lock::remove ()
{
  return this->lock_->remove ();
}

int
ACE_Adaptive_Lock::acquire ()
{
  return this->lock_->acquire ();
}

int
ACE_Adaptive_Lock::tryacquire ()
{
  return this->lock_->tryacquire ();
}

int
ACE_Adaptive_Lock::release ()
{
  return this->lock_->release ();
}

int
ACE_Adaptive_Lock::acquire_read ()
{
  return this->lock_->acquire_read ();
}

int
ACE_Adaptive_Lock::acquire_write ()
{
  return this->lock_->acquire_write ();
}

int
ACE_Adaptive_Lock::tryacquire_read ()
{
  return this->lock_->tryacquire_read ();
}

int
ACE_Adaptive_Lock::tryacquire_write ()
{
  return this->lock_->tryacquire_write ();
}

int
ACE_Adaptive_Lock::tryacquire_write_upgrade ()
{
  return this->lock_->tryacquire_write_upgrade ();
}

void
ACE_Adaptive_Lock::dump () const
{
#if defined (ACE_HAS_DUMP)
  //  return this->lock_->dump ();
#endif /* ACE_HAS_DUMP */
}

ACE_END_VERSIONED_NAMESPACE_DECL
