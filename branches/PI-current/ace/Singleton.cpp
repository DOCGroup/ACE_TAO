// $Id$

#ifndef ACE_SINGLETON_C
#define ACE_SINGLETON_C

#define ACE_BUILD_DLL
#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_T.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Singleton.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Singleton, "$Id$")

template <class TYPE, class ACE_LOCK> void
ACE_Singleton<TYPE, ACE_LOCK>::dump (void)
{
  ACE_TRACE ("ACE_Singleton<TYPE, ACE_LOCK>::dump");

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("instance_ = %x"),
              ACE_Singleton<TYPE, ACE_LOCK>::instance_i ()));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class ACE_LOCK> ACE_Singleton<TYPE, ACE_LOCK> *&
ACE_Singleton<TYPE, ACE_LOCK>::instance_i (void)
{
#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Pointer to the Singleton instance.  This works around a bug with
  // G++ and it's (mis-)handling of templates and statics...
  static ACE_Singleton<TYPE, ACE_LOCK> *singleton_ = 0;

  return singleton_;
#else
  return ACE_Singleton<TYPE, ACE_LOCK>::singleton_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class ACE_LOCK> TYPE *
ACE_Singleton<TYPE, ACE_LOCK>::instance (void)
{
  ACE_TRACE ("ACE_Singleton<TYPE, ACE_LOCK>::instance");

  ACE_Singleton<TYPE, ACE_LOCK> *&singleton =
    ACE_Singleton<TYPE, ACE_LOCK>::instance_i ();

  // Perform the Double-Check pattern...
  if (singleton == 0)
    {
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
      if (ACE_Object_Manager::starting_up () ||
          ACE_Object_Manager::shutting_down ())
        {
          // The program is still starting up, and therefore assumed
          // to be single threaded.  There's no need to double-check.
          // Or, the ACE_Object_Manager instance has been destroyed,
          // so the preallocated lock is not available.  Either way,
          // don't register for destruction with the
          // ACE_Object_Manager:  we'll have to leak this instance.
#endif /* ACE_MT_SAFE */

          ACE_NEW_RETURN (singleton, (ACE_Singleton<TYPE, ACE_LOCK>), 0);

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
        }
      else
        {
          // Obtain a lock from the ACE_Object_Manager.  The pointer
          // is static, so we only obtain one per ACE_Singleton
          // instantiation.
          static ACE_LOCK *lock = 0;
          if (ACE_Object_Manager::get_singleton_lock (lock) != 0)
            // Failed to acquire the lock!
            return 0;

          ACE_GUARD_RETURN (ACE_LOCK, ace_mon, *lock, 0);

          if (singleton == 0)
            {
              ACE_NEW_RETURN (singleton, (ACE_Singleton<TYPE, ACE_LOCK>), 0);

              // Register for destruction with ACE_Object_Manager.
              ACE_Object_Manager::at_exit (singleton);
            }
        }
#endif /* ACE_MT_SAFE */
    }

  return &singleton->instance_;
}

template <class TYPE, class ACE_LOCK> void
ACE_Singleton<TYPE, ACE_LOCK>::cleanup (void *)
{
  delete this;
  ACE_Singleton<TYPE, ACE_LOCK>::instance_i () = 0;
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class ACE_LOCK> ACE_Singleton<TYPE, ACE_LOCK> *
ACE_Singleton<TYPE, ACE_LOCK>::singleton_ = 0;
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

template <class TYPE, class ACE_LOCK> void
ACE_TSS_Singleton<TYPE, ACE_LOCK>::dump (void)
{
  ACE_TRACE ("ACE_TSS_Singleton<TYPE, ACE_LOCK>::dump");

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("instance_ = %x"),
              ACE_TSS_Singleton<TYPE, ACE_LOCK>::instance_i ()));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class ACE_LOCK> ACE_TSS_Singleton<TYPE, ACE_LOCK> *&
ACE_TSS_Singleton<TYPE, ACE_LOCK>::instance_i (void)
{
#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Pointer to the Singleton instance.  This works around a bug with
  // G++ and it's (mis-)handling of templates and statics...
  static ACE_TSS_Singleton<TYPE, ACE_LOCK> *singleton_ = 0;

  return singleton_;
#else
  return ACE_TSS_Singleton<TYPE, ACE_LOCK>::singleton_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class ACE_LOCK> TYPE *
ACE_TSS_Singleton<TYPE, ACE_LOCK>::instance (void)
{
  ACE_TRACE ("ACE_TSS_Singleton<TYPE, ACE_LOCK>::instance");

  ACE_TSS_Singleton<TYPE, ACE_LOCK> *&singleton =
    ACE_TSS_Singleton<TYPE, ACE_LOCK>::instance_i ();

  // Perform the Double-Check pattern...
  if (singleton == 0)
    {
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
      if (ACE_Object_Manager::starting_up () ||
          ACE_Object_Manager::shutting_down ())
        {
          // The program is still starting up, and therefore assumed
          // to be single threaded.  There's no need to double-check.
          // Or, the ACE_Object_Manager instance has been destroyed,
          // so the preallocated lock is not available.  Either way,
          // don't register for destruction with the
          // ACE_Object_Manager:  we'll have to leak this instance.
#endif /* ACE_MT_SAFE */

          ACE_NEW_RETURN (singleton, (ACE_TSS_Singleton<TYPE, ACE_LOCK>), 0);

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
        }
      else
        {
          // Obtain a lock from the ACE_Object_Manager.  The pointer
          // is static, so we only obtain one per ACE_Singleton instantiation.
          static ACE_LOCK *lock = 0;
          if (ACE_Object_Manager::get_singleton_lock (lock) != 0)
            // Failed to acquire the lock!
            return 0;

          ACE_GUARD_RETURN (ACE_LOCK, ace_mon, *lock, 0);

          if (singleton == 0)
            {
              ACE_NEW_RETURN (singleton, (ACE_TSS_Singleton<TYPE, ACE_LOCK>),
                              0);

              // Register for destruction with ACE_Object_Manager.
              ACE_Object_Manager::at_exit (singleton);
            }
        }
#endif /* ACE_MT_SAFE */
    }

  return ACE_TSS_GET (&singleton->instance_, TYPE);
}

template <class TYPE, class ACE_LOCK> void
ACE_TSS_Singleton<TYPE, ACE_LOCK>::cleanup (void *)
{
  ACE_TRACE ("ACE_TSS_Singleton::cleanup");

  delete this;
  ACE_TSS_Singleton<TYPE, ACE_LOCK>::instance_i () = 0;
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class ACE_LOCK> ACE_TSS_Singleton <TYPE, ACE_LOCK> *
ACE_TSS_Singleton<TYPE, ACE_LOCK>::singleton_ = 0;
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

#endif /* ACE_SINGLETON_C */
