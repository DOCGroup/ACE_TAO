// Singleton.cpp
// $Id$

#if !defined (ACE_SINGLETON_C)
#define ACE_SINGLETON_C

#define ACE_BUILD_DLL
#include "ace/Singleton.h"
#include "ace/Synch_T.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
#include "ace/Singleton.i"
#endif /* __ACE_INLINE__ */

template <class TYPE, class ACE_LOCK> void
ACE_Singleton<TYPE, ACE_LOCK>::dump (void)
{
  ACE_TRACE ("ACE_Singleton<TYPE, ACE_LOCK>::dump");

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG, "instance_ = %x",
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
      if (ACE_Object_Manager::starting_up ())
        {
          // The program is still starting up, and therefore assumed
          // to be single threaded.  There's no need to double-check.
#endif /* ACE_MT_SAFE */
          ACE_NEW_RETURN (singleton, (ACE_Singleton<TYPE, ACE_LOCK>), 0);

          // Register for destruction with ACE_Object_Manager.
          ACE_Object_Manager::at_exit (singleton);
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
        }
      else
        {
          // Use the Object_Manager's preallocated lock.
          ACE_LOCK *lock =
            ACE_Object_Manager::get_singleton_lock ((ACE_LOCK *) 0);
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
  ACE_DEBUG ((LM_DEBUG, "instance_ = %x",
              ACE_Singleton<TYPE, ACE_LOCK>::instance_i ()));
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
      if (ACE_Object_Manager::starting_up ())
        {
          // The program is still starting up, and therefore assumed
          // to be single threaded.  There's no need to double-check.
#endif /* ACE_MT_SAFE */
          ACE_NEW_RETURN (singleton, (ACE_TSS_Singleton<TYPE, ACE_LOCK>), 0);

#if 0  /* ACE_Object_Manager::at_thread_exit () is not implemented yet. */
          // Register for destruction with ACE_Object_Manager.
          ACE_Object_Manager::at_thread_exit (singleton);
#endif /* 0 */
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
        }
      else
        {
          // Use the Object_Manager's preallocated lock.
          ACE_LOCK *lock =
            ACE_Object_Manager::get_singleton_lock ((ACE_LOCK *) 0);
          ACE_GUARD_RETURN (ACE_LOCK, ace_mon, *lock, 0);

          if (singleton == 0)
            {
              ACE_NEW_RETURN (singleton,
                              (ACE_TSS_Singleton<TYPE, ACE_LOCK>), 0);

#if 0  /* ACE_Object_Manager::at_thread_exit () is not implemented yet. */
              // Register for destruction with ACE_Object_Manager.
              ACE_Object_Manager::at_thread_exit (singleton);
#endif /* 0 */
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
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class ACE_LOCK> ACE_TSS_Singleton <TYPE, ACE_LOCK> *
ACE_TSS_Singleton<TYPE, ACE_LOCK>::singleton_ = 0;
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

#endif /* ACE_SINGLETON_C */
