// $Id$

#include "ace/Threads/Thread_Exit.h"
#include "ace/OS/OS.h"
#include "ace/Threads/Synch.h"
#include "ace/Utils/Templates/Managed_Object.h"

ACE_RCSID(ace, Thread_Exit, "$Id$")

u_int ACE_Thread_Exit::is_constructed_ = 0;

#if defined (ACE_HAS_SIG_C_FUNC)
extern "C" void
ACE_Thread_Exit_cleanup (void *instance, void *arg)
{
  ACE_Thread_Exit::cleanup (instance, arg);
}
#endif

void
ACE_Thread_Exit::cleanup (void *instance, void *)
{
  ACE_OS_TRACE ("ACE_Thread_Exit::cleanup");

  delete (ACE_TSS_TYPE (ACE_Thread_Exit) *) instance;

  ACE_Thread_Exit::is_constructed_ = 0;
  // All TSS objects have been destroyed.  Reset this flag so
  // ACE_Thread_Exit singleton can be created again.
}

// NOTE: this preprocessor directive should match the one in
// ACE_Task_Base::svc_run () below.  This prevents the two statics
// from being defined.

ACE_Thread_Exit *
ACE_Thread_Exit::instance (void)
{
#if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)
  ACE_OS_TRACE ("ACE_Thread_Exit::instance");

  // Determines if we were dynamically allocated.
  static ACE_TSS_TYPE (ACE_Thread_Exit) *instance_;

  // Implement the Double Check pattern.

  if (ACE_Thread_Exit::is_constructed_ == 0)
    {
      ACE_MT (ACE_Thread_Mutex *lock =
              ACE_Managed_Object<ACE_Thread_Mutex>::get_preallocated_object
                (ACE_Object_Manager::ACE_THREAD_EXIT_LOCK);
              ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *lock, 0));

      if (ACE_Thread_Exit::is_constructed_ == 0)
        {
          ACE_NEW_RETURN (instance_,
                          ACE_TSS_TYPE (ACE_Thread_Exit),
                          0);

          ACE_Thread_Exit::is_constructed_ = 1;

          // Register for destruction with ACE_Object_Manager.
#if defined ACE_HAS_SIG_C_FUNC
          ACE_Object_Manager::at_exit (instance_,
                                       ACE_Thread_Exit_cleanup,
                                       0);
#else
          ACE_Object_Manager::at_exit (instance_,
                                       ACE_Thread_Exit::cleanup,
                                       0);
#endif /* ACE_HAS_SIG_C_FUNC */
        }
    }

  return ACE_TSS_GET (instance_, ACE_Thread_Exit);
#else
  return 0;
#endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION */
}

// Grab hold of the Task * so that we can close() it in the
// destructor.

ACE_Thread_Exit::ACE_Thread_Exit (void)
{
  ACE_OS_TRACE ("ACE_Thread_Exit::ACE_Thread_Exit");
}

// Set the this pointer...

void
ACE_Thread_Exit::thr_mgr (ACE_Thread_Manager *tm)
{
  ACE_OS_TRACE ("ACE_Thread_Exit::thr_mgr");

  if (tm != 0)
    this->thread_control_.insert (tm, 0);
}

// When this object is destroyed the Task is automatically closed
// down!

ACE_Thread_Exit::~ACE_Thread_Exit (void)
{
  ACE_OS_TRACE ("ACE_Thread_Exit::~ACE_Thread_Exit");
}

ACE_Thread_Exit_Maybe::ACE_Thread_Exit_Maybe (int flag)
  : instance_ (0)
{
  if (flag)
    {
      ACE_NEW (instance_, ACE_Thread_Exit);
    }
}

ACE_Thread_Exit_Maybe::~ACE_Thread_Exit_Maybe (void)
{
  delete this->instance_;
}

ACE_Thread_Exit *
ACE_Thread_Exit_Maybe::operator -> (void) const
{
  return this->instance_;
}

ACE_Thread_Exit *
ACE_Thread_Exit_Maybe::instance (void) const
{
  return this->instance_;
}

#if (defined (ACE_HAS_THREADS) && \
      (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || \
       defined (ACE_HAS_TSS_EMULATION)))

# if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_TSS<ACE_Thread_Exit>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<ACE_Thread_Exit>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION) */
