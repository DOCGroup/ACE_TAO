// Singleton.cpp
// $Id$

#if !defined (ACE_SINGLETON_C)
#define ACE_SINGLETON_C

#define ACE_BUILD_DLL
#include "ace/Singleton.h"
#include "ace/Synch_T.h"

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
      ACE_GUARD_RETURN (LOCK, ace_mon, ACE_Singleton<TYPE, LOCK>::singleton_lock_i (), 0);

      if (singleton == 0)
	ACE_NEW_RETURN (singleton, TYPE, 0);
    }

  return singleton;
}

template <class TYPE, class LOCK> TYPE *
ACE_Singleton<TYPE, LOCK>::instance (TYPE *new_instance)
{
  ACE_TRACE ("ACE_Singleton::set_instance");

  TYPE *&singleton = ACE_Singleton<TYPE, LOCK>::instance_i ();
  ACE_GUARD_RETURN (LOCK, ace_mon, ACE_Singleton<TYPE, LOCK>::singleton_lock_i (), 0);

  TYPE *old_instance = singleton;
  singleton = new_instance;

  return singleton;
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class LOCK> TYPE *
ACE_Singleton<TYPE, LOCK>::instance_ = 0;

// Lock the creation of the singleton.  
template <class TYPE, class LOCK> LOCK
ACE_Singleton<TYPE, LOCK>::ace_singleton_lock_;
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

template <class TYPE, class LOCK, ACE_Singleton_Strategy MEMORY> void
ACE_SingletonEx<TYPE, LOCK, MEMORY>::dump (void)
{
  ACE_TRACE ("ACE_SingletonEx<TYPE, LOCK, MEMORY>::dump");

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG, "instance_ = %x", 
	      ACE_Singleton<TYPE, LOCK>::instance_i ()));
  ACE_Singleton<TYPE, LOCK>::singleton_lock_i ().dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK, ACE_Singleton_Strategy MEMORY> TYPE *&
ACE_SingletonEx<TYPE, LOCK, MEMORY>::instance_i (void)
{
#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Pointer to the Singleton instance.  This works around a bug with
  // G++...
  static TYPE *instance_ = 0;

  return instance_;
#else
  return ACE_SingletonEx<TYPE, LOCK, MEMORY>::instance_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK, ACE_Singleton_Strategy MEMORY> LOCK &
ACE_SingletonEx<TYPE, LOCK, MEMORY>::singleton_lock_i (void)
{
#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Lock the creation of the singleton.  This works around a
  // "feature" of G++... ;-)
  static LOCK ace_singletonex_lock_;

  return ace_singletonex_lock_;
#else
  return ACE_SingletonEx<TYPE, LOCK, MEMORY>::ace_singletonex_lock_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK, ACE_Singleton_Strategy MEMORY> TYPE *
ACE_SingletonEx<TYPE, LOCK, MEMORY>::instance (void)
{
  ACE_TRACE ("ACE_SingletonEx<TYPE, LOCK, MEMORY>::instance");

  TYPE *&singleton = ACE_SingletonEx<TYPE, LOCK>::instance_i ();

  // Perform the Double-Check pattern...
  if (singleton == 0)
    {
      ACE_GUARD_RETURN (LOCK, ace_mon, ACE_SingletonEx<TYPE, LOCK>::singleton_lock_i (), 0);

      if (singleton == 0)
	ACE_NEW_RETURN (singleton, TYPE, 0);
    }

  if (MEMORY == ACE_SINGLETON_HEAP)
    return singleton;
  else
    return ACE_TSS_GET (singleton, TYPE);
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class LOCK, ACE_Singleton_Strategy MEMORY> TYPE *
ACE_SingletonEx<TYPE, LOCK, MEMORY>::instance_ = 0;

// Lock the creation of the singleton.  
template <class TYPE, class LOCK, ACE_Singleton_Strategy MEMORY> LOCK
ACE_SingletonEx<TYPE, LOCK, MEMORY>::ace_singletonex_lock_;
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

#endif /* ACE_SINGLETON_C */
