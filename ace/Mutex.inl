/* -*- C++ -*- */
// $Id$

ACE_INLINE int
ACE_Mutex::acquire_read (void)
{
// ACE_TRACE ("ACE_Mutex::acquire_read");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_lock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::acquire_write (void)
{
// ACE_TRACE ("ACE_Mutex::acquire_write");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_lock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::tryacquire_read (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire_read");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_trylock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE const ACE_mutex_t &
ACE_Mutex::lock (void) const
{
// ACE_TRACE ("ACE_Mutex::lock");
#if defined (CHORUS)
  if (this->process_lock_)
    return *this->process_lock_;
#endif /* CHORUS */
  return this->lock_;
}

ACE_INLINE int
ACE_Mutex::tryacquire_write (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire_write");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_trylock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::tryacquire_write_upgrade (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire_write_upgrade");
  return 0;
}

ACE_INLINE int
ACE_Mutex::acquire (void)
{
// ACE_TRACE ("ACE_Mutex::acquire");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_lock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_lock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::acquire (ACE_Time_Value &tv)
{
  // ACE_TRACE ("ACE_Mutex::acquire");
  return ACE_OS::mutex_lock (&this->lock_, tv);
}

ACE_INLINE int
ACE_Mutex::acquire (ACE_Time_Value *tv)
{
  // ACE_TRACE ("ACE_Mutex::acquire");
  return ACE_OS::mutex_lock (&this->lock_, tv);
}

ACE_INLINE int
ACE_Mutex::tryacquire (void)
{
// ACE_TRACE ("ACE_Mutex::tryacquire");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_trylock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_trylock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::release (void)
{
// ACE_TRACE ("ACE_Mutex::release");
#if defined (CHORUS)
   if (this->process_lock_)
     return ACE_OS::mutex_unlock (this->process_lock_);
#endif /* CHORUS */
  return ACE_OS::mutex_unlock (&this->lock_);
}

ACE_INLINE int
ACE_Mutex::remove (void)
{
// ACE_TRACE ("ACE_Mutex::remove");
#if defined (CHORUS) || defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_STHREADS)
   int result = 0;
   // In the case of a interprocess mutex, the owner is the first
   // process that created the shared memory object. In this case, the
   // lockname_ pointer will be non-zero (points to allocated memory
   // for the name).  Owner or not, the memory needs to be unmapped
   // from the process.  If we are the owner, the file used for
   // shm_open needs to be deleted as well.
   if (this->process_lock_)
     {
       if (this->removed_ == 0)
         {
           this->removed_ = 1;

           // Only destroy the lock if we're the ones who initialized
           // it.
           if (!this->lockname_)
             ACE_OS::munmap ((void *) this->process_lock_,
                             sizeof (ACE_mutex_t));
           else
             {
               result = ACE_OS::mutex_destroy (this->process_lock_);
               ACE_OS::munmap ((void *) this->process_lock_,
                               sizeof (ACE_mutex_t));
               ACE_OS::shm_unlink (this->lockname_);
               ACE_OS::free (ACE_static_cast (void *,
                                              ACE_const_cast (ACE_TCHAR *,
                                                            this->lockname_)));
             }
         }
     }
   return result;
#else /* !CHORUS */
  int result = 0;
  if (this->removed_ == 0)
    {
      this->removed_ = 1;
      result = ACE_OS::mutex_destroy (&this->lock_);
    }
  return result;
#endif /* CHORUS */
}
