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

template <class TYPE, class LOCK> void
ACE_Singleton<TYPE, LOCK>::dump (void)
{
  ACE_TRACE ("ACE_Singleton<TYPE, LOCK>::dump");

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG, "instance_ = %x", 
	      ACE_Singleton<TYPE, LOCK>::instance_i ()));
  ACE_Singleton<TYPE, LOCK>::singleton_lock_i ().dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK> TYPE *&
ACE_Singleton<TYPE, LOCK>::instance_i (void)
{
#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Pointer to the Singleton instance.  This works around a bug with
  // G++...
  static TYPE *instance_ = 0;

  return instance_;
#else
  return ACE_Singleton<TYPE, LOCK>::instance_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK> LOCK &
ACE_Singleton<TYPE, LOCK>::singleton_lock_i (void)
{
#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Lock the creation of the singleton.  This works around a
  // "feature" of G++... ;-)
  static LOCK ace_singleton_lock_;

  return ace_singleton_lock_;
#else
  return ACE_Singleton<TYPE, LOCK>::ace_singleton_lock_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK> TYPE *
ACE_Singleton<TYPE, LOCK>::instance (void)
{
  ACE_TRACE ("ACE_Singleton<TYPE, LOCK>::instance");

  TYPE *&singleton = ACE_Singleton<TYPE, LOCK>::instance_i ();

  // Perform the Double-Check pattern...
  if (singleton == 0)
    {
      ACE_GUARD_RETURN (LOCK, ace_mon, (ACE_Singleton<TYPE, LOCK>::singleton_lock_i ()), 0);

      if (singleton == 0)
      {
	ACE_NEW_RETURN (singleton, TYPE, 0);

        // Register for destruction with ACE_Object_Manager.
        ACE_Object_Manager::at_exit (singleton, cleanup, 0);
      }
    }

  return singleton;
}

template <class TYPE, class LOCK> TYPE *
ACE_Singleton<TYPE, LOCK>::instance (TYPE *new_instance)
{
  ACE_TRACE ("ACE_Singleton::set_instance");

  ACE_GUARD_RETURN (LOCK, ace_mon, (ACE_Singleton<TYPE, LOCK>::singleton_lock_i ()), 0);

  TYPE *&singleton = ACE_Singleton<TYPE, LOCK>::instance_i ();
  TYPE *old_instance = singleton;
  singleton = new_instance;

  return old_instance;
}

template <class TYPE, class LOCK> void
ACE_Singleton<TYPE, LOCK>::cleanup (void *object, void *)
{
  ACE_TRACE ("ACE_Singleton::cleanup");

  delete (TYPE *) object;
  object = 0;
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class LOCK> TYPE *
ACE_Singleton<TYPE, LOCK>::instance_ = 0;

// Lock the creation of the singleton.  
template <class TYPE, class LOCK> LOCK
ACE_Singleton<TYPE, LOCK>::ace_singleton_lock_;
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

template <class TYPE, class LOCK> void
ACE_TSS_Singleton<TYPE, LOCK>::dump (void)
{
  ACE_TRACE ("ACE_TSS_Singleton<TYPE, LOCK>::dump");

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG, "instance_ = %x", instance_));
  ace_singleton_lock_.dump();                                                  
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK> TYPE *
ACE_TSS_Singleton<TYPE, LOCK>::instance (void)
{
  ACE_TRACE ("ACE_TSS_Singleton<TYPE, LOCK>::instance");

#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Pointer to the Singleton instance.  This works around a bug with
  // G++...
  static ACE_TSS<TYPE> *instance_ = 0;

  // Lock the creation of the singleton.  This works around a
  // "feature" of G++... ;-)
  static LOCK ace_singleton_lock_;

  // Perform the Double-Check pattern...                                        
  if (instance_ == 0)
    {
      ACE_GUARD_RETURN (LOCK, ace_mon, ace_singleton_lock_, 0);

      if (instance_ == 0)
	{
          ACE_NEW_RETURN (instance_, ACE_TSS<TYPE>, 0);

#if 0  /* ACE_Object_Manager::at_thread_exit () is not implemented yet. */
          // Register for destruction with ACE_Object_Manager.
          ACE_Object_Manager::at_thread_exit (instance_, cleanup, 0);
#endif /* 0 */
	}
    }

  return ACE_TSS_GET (instance_, TYPE);
#else

  // Perform the Double-Check pattern...
  if (ACE_TSS_Singleton<TYPE, LOCK>::instance_ == 0)
    {
      ACE_GUARD_RETURN (LOCK, ace_mon, (ACE_TSS_Singleton<TYPE, LOCK>::ace_singleton_lock_), 0);

      if (ACE_TSS_Singleton<TYPE, LOCK>::instance_ == 0)             
	{
          ACE_NEW_RETURN (ACE_TSS_Singleton<TYPE, LOCK>::instance_,
                          ACE_TSS<TYPE>,
                          0);

#if 0  /* ACE_Object_Manager::at_thread_exit () is not implemented yet. */
          // Register for destruction with ACE_Object_Manager.
          ACE_Object_Manager::at_thread_exit (
            ACE_TSS_Singleton<TYPE, LOCK>::instance_,
            cleanup,
            0);
#endif /* 0 */
	}
    }

  return ACE_TSS_GET ((ACE_TSS_Singleton<TYPE, LOCK>::instance_), TYPE);

#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK> void
ACE_TSS_Singleton<TYPE, LOCK>::cleanup (void *object, void *)
{
  ACE_TRACE ("ACE_TSS_Singleton::cleanup");

  delete (TYPE *) object;
  object = 0;
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class LOCK> ACE_TSS<TYPE> *
ACE_TSS_Singleton<TYPE, LOCK>::instance_ = 0;

// Lock the creation of the singleton.
template <class TYPE, class LOCK> LOCK
ACE_TSS_Singleton<TYPE, LOCK>::ace_singleton_lock_;                
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

#endif /* ACE_SINGLETON_C */
