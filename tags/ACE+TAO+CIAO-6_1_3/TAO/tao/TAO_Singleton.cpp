// -*- C++ -*-
//
// $Id$

#ifndef TAO_SINGLETON_CPP
#define TAO_SINGLETON_CPP

#include "tao/TAO_Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Singleton_Manager.h"

#include "ace/Guard_T.h"
#include "ace/Object_Manager.h"
#include "ace/Log_Msg.h"
#include "ace/os_include/os_typeinfo.h"

#if !defined (__ACE_INLINE__)
#include "tao/TAO_Singleton.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TYPE, class ACE_LOCK> void
TAO_Singleton<TYPE, ACE_LOCK>::dump (void)
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("TAO_Singleton<TYPE, ACE_LOCK>::dump");

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("instance_ = %@"),
              TAO_Singleton<TYPE, ACE_LOCK>::instance_i ()));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
#endif /* ACE_HAS_DUMP */
}

template <class TYPE, class ACE_LOCK> TAO_Singleton<TYPE, ACE_LOCK> *&
TAO_Singleton<TYPE, ACE_LOCK>::instance_i (void)
{
#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Pointer to the Singleton instance.  This works around a bug with
  // G++ and it's (mis-)handling of templates and statics...
  static TAO_Singleton<TYPE, ACE_LOCK> *singleton_ = 0;

  return singleton_;
#else
  return TAO_Singleton<TYPE, ACE_LOCK>::singleton_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class ACE_LOCK> TYPE *
TAO_Singleton<TYPE, ACE_LOCK>::instance (void)
{
  ACE_TRACE ("TAO_Singleton<TYPE, ACE_LOCK>::instance");

  TAO_Singleton<TYPE, ACE_LOCK> *&singleton =
    TAO_Singleton<TYPE, ACE_LOCK>::instance_i ();

  // Perform the Double-Check pattern...
  if (singleton == 0)
    {
      if (TAO_Singleton_Manager::starting_up () ||
          TAO_Singleton_Manager::shutting_down ())
        {
          // The program is still starting up, and therefore assumed
          // to be single threaded.  There's no need to double-check.
          // Or, the TAO_Singleton_Manager instance has been destroyed,
          // so the preallocated lock is not available.  Either way,
          // don't register for destruction with the
          // TAO_Singleton_Manager:  we'll have to leak this instance.

          ACE_NEW_RETURN (singleton, (TAO_Singleton<TYPE, ACE_LOCK>), 0);
        }
      else
        {
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
          // Obtain a lock from the ACE_Object_Manager.  The pointer
          // is static, so we only obtain one per TAO_Singleton
          // instantiation.
          static ACE_LOCK *lock = 0;
          if (ACE_Object_Manager::get_singleton_lock (lock) != 0)
            // Failed to acquire the lock!
            return 0;

          ACE_GUARD_RETURN (ACE_LOCK, ace_mon, *lock, 0);

          if (singleton == 0)
            {
#endif /* ACE_MT_SAFE */
              ACE_NEW_RETURN (singleton, (TAO_Singleton<TYPE, ACE_LOCK>), 0);

              // Register for destruction with TAO_Singleton_Manager.
              TAO_Singleton_Manager::at_exit (singleton, 0, typeid (TYPE).name());
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
            }
#endif /* ACE_MT_SAFE */
        }
    }

  return &singleton->instance_;
}

template <class TYPE, class ACE_LOCK> void
TAO_Singleton<TYPE, ACE_LOCK>::cleanup (void *)
{
  delete this;
  TAO_Singleton<TYPE, ACE_LOCK>::instance_i () = 0;
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class ACE_LOCK> TAO_Singleton<TYPE, ACE_LOCK> *
TAO_Singleton<TYPE, ACE_LOCK>::singleton_ = 0;

template <class TYPE, class ACE_LOCK> TAO_TSS_Singleton<TYPE, ACE_LOCK> *
TAO_TSS_Singleton<TYPE, ACE_LOCK>::singleton_ = 0;
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */


template <class TYPE, class ACE_LOCK> void
TAO_TSS_Singleton<TYPE, ACE_LOCK>::dump (void)
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("TAO_TSS_Singleton<TYPE, ACE_LOCK>::dump");

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG,  ACE_TEXT ("instance_ = %@"),
              TAO_TSS_Singleton<TYPE, ACE_LOCK>::instance_i ()));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
#endif /* ACE_HAS_DUMP */
}

template <class TYPE, class ACE_LOCK> TAO_TSS_Singleton<TYPE, ACE_LOCK> *&
TAO_TSS_Singleton<TYPE, ACE_LOCK>::instance_i (void)
{
#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Pointer to the Singleton instance.  This works around a bug with
  // G++ and it's (mis-)handling of templates and statics...
  static TAO_TSS_Singleton<TYPE, ACE_LOCK> *singleton_ = 0;

  return singleton_;
#else
  return TAO_TSS_Singleton<TYPE, ACE_LOCK>::singleton_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class ACE_LOCK> TYPE *
TAO_TSS_Singleton<TYPE, ACE_LOCK>::instance (void)
{
  ACE_TRACE ("TAO_TSS_Singleton<TYPE, ACE_LOCK>::instance");

  TAO_TSS_Singleton<TYPE, ACE_LOCK> *&singleton =
    TAO_TSS_Singleton<TYPE, ACE_LOCK>::instance_i ();

  // Perform the Double-Check pattern...
  if (singleton == 0)
    {
      if (TAO_Singleton_Manager::starting_up () ||
          TAO_Singleton_Manager::shutting_down ())
        {
          // The program is still starting up, and therefore assumed
          // to be single threaded.  There's no need to double-check.
          // Or, the TAO_Singleton_Manager instance has been destroyed,
          // so the preallocated lock is not available.  Either way,
          // don't register for destruction with the
          // TAO_Singleton_Manager:  we'll have to leak this instance.

          ACE_NEW_RETURN (singleton, (TAO_TSS_Singleton<TYPE, ACE_LOCK>), 0);
        }
      else
        {
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
          // Obtain a lock from the ACE_Object_Manager.  The pointer
          // is static, so we only obtain one per TAO_Singleton
          // instantiation.
          static ACE_LOCK *lock = 0;
          if (ACE_Object_Manager::get_singleton_lock (lock) != 0)
            // Failed to acquire the lock!
            return 0;

          ACE_GUARD_RETURN (ACE_LOCK, ace_mon, *lock, 0);

          if (singleton == 0)
            {
#endif /* ACE_MT_SAFE */
              ACE_NEW_RETURN (singleton, (TAO_TSS_Singleton<TYPE, ACE_LOCK>),
                              0);

              // Register for destruction with TAO_Singleton_Manager.
              TAO_Singleton_Manager::at_exit (singleton, 0, typeid (TYPE).name ());
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
            }
#endif /* ACE_MT_SAFE */
        }
    }

  return ACE_TSS_GET (&singleton->instance_, TYPE);
}

template <class TYPE, class ACE_LOCK> void
TAO_TSS_Singleton<TYPE, ACE_LOCK>::cleanup (void *)
{
  delete this;
  TAO_TSS_Singleton<TYPE, ACE_LOCK>::instance_i () = 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_SINGLETON_CPP */
