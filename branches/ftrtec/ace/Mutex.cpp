// $Id$

#if !defined (__ACE_INLINE__)
#include "ace/Mutex.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Mutex, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Mutex)

void
ACE_Mutex::dump (void) const
{
// ACE_TRACE ("ACE_Mutex::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
#if defined (CHORUS)
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("lockname_ = %s\n"), this->lockname_));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("process_lock_ = %x\n"), this->process_lock_));
#endif /* CHORUS */
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Mutex::ACE_Mutex (int type, const ACE_TCHAR *name,
                      ACE_mutexattr_t *arg, mode_t mode)
  :
#if defined (CHORUS) || defined (ACE_HAS_PTHREADS) || defined(ACE_HAS_STHREADS)
    process_lock_ (0),
    lockname_ (0),
#endif /* CHORUS */
    removed_ (0)
{
  // ACE_TRACE ("ACE_Mutex::ACE_Mutex");

  // These platforms need process-wide mutex to be in shared memory.
#if defined (CHORUS) || defined(ACE_HAS_PTHREADS) || defined (ACE_HAS_STHREADS)
  if (type == USYNC_PROCESS)
    {
      // Let's see if the shared memory entity already exists.
      ACE_HANDLE fd = ACE_OS::shm_open (name, O_RDWR | O_CREAT | O_EXCL, mode);
      if (fd == ACE_INVALID_HANDLE)
        {
          if (errno == EEXIST)
            fd = ACE_OS::shm_open (name, O_RDWR | O_CREAT, mode);
          else
            return;
        }
      else
        {
          // We own this shared memory object!  Let's set its size.
          if (ACE_OS::ftruncate (fd,
                                 sizeof (ACE_mutex_t)) == -1)
            {
              ACE_OS::close (fd);
              return;
            }
          this->lockname_ = ACE_OS::strdup (name);
          if (this->lockname_ == 0)
            {
              ACE_OS::close (fd);
              return;
            }
        }

      this->process_lock_ =
        (ACE_mutex_t *) ACE_OS::mmap (0,
                                      sizeof (ACE_mutex_t),
                                      PROT_RDWR,
                                      MAP_SHARED,
                                      fd,
                                      0);
      ACE_OS::close (fd);
      if (this->process_lock_ == MAP_FAILED)
        return;

      if (this->lockname_
          && ACE_OS::mutex_init (this->process_lock_,
                                 type,
                                 name,
                                 arg) != 0)
        return;
    }
   // It is ok to fall through into the <mutex_init> below if the
   // USYNC_PROCESS flag is not enabled.
#else
  ACE_UNUSED_ARG (mode);
#endif /* CHORUS */

  if (ACE_OS::mutex_init (&this->lock_,
                          type,
                          name,
                          arg) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Mutex::ACE_Mutex")));
}

ACE_Mutex::~ACE_Mutex (void)
{
// ACE_TRACE ("ACE_Mutex::~ACE_Mutex");
  this->remove ();
}
