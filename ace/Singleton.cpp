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
ACE_Singleton<TYPE, LOCK>::dump (void) const
{
  ACE_TRACE ("ACE_Singleton<TYPE, LOCK>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  ACE_DEBUG ((LM_DEBUG, "instance_ = %x", this->instance_));
  ace_singleton_lock_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
}

template <class TYPE, class LOCK> TYPE *
ACE_Singleton<TYPE, LOCK>::instance (void)
{
  ACE_TRACE ("ACE_Singleton::instance");

#if defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  // Pointer to the Singleton instance.  This works around a bug with
  // G++...
  static TYPE *instance_ = 0;

  // Lock the creation of the singleton.  This works around a
  // "feature" of G++... ;-)
  static LOCK ace_singleton_lock_;
  // Perform the Double-Check pattern...
  if (instance_ == 0)
    {
      ACE_GUARD_RETURN (LOCK, ace_mon, ace_singleton_lock_, 0);

      if (instance_ == 0)
	{
	  // We can replace the following lines with this line once we
	  // update the macro in OS.h 
	  // ACE_NEW_RETURN (instance_, TYPE, 0);

	  instance_ = new TYPE;
	  if (instance_ == 0)
	    {
	      errno = ENOMEM;
	      return 0;
	    }
	  else if (ACE_LOG_MSG->op_status () == -1)
	    {
	      errno = ACE_LOG_MSG->errnum ();
	      delete instance_;
	      instance_ = 0;
	      return 0;
	    }
	}
    }

  return instance_;
#else

  // Perform the Double-Check pattern...
  if (ACE_Singleton<TYPE, LOCK>::instance_ == 0)
    {
      ACE_GUARD_RETURN (LOCK, ace_mon, (ACE_Singleton<TYPE, LOCK>::ace_singleton_lock_), 0);

      if (ACE_Singleton<TYPE, LOCK>::instance_ == 0)
	{
	  // We can replace the following lines with this line once we
	  // update the macro in OS.h 
	  // ACE_NEW_RETURN ((ACE_Singleton<TYPE, LOCK>::instance_), TYPE, 0);

	  instance_ = new TYPE;
	  if (instance_ == 0)
	    {
	      errno = ENOMEM;
	      return 0;
	    }
	  else if (ACE_LOG_MSG->op_status () == -1)
	    {
	      errno = ACE_LOG_MSG->errnum ();
	      delete instance_;
	      instance_ = 0;
	      return 0;
	    }
	}
    }

  return ACE_Singleton<TYPE, LOCK>::instance_;

#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
// Pointer to the Singleton instance.
template <class TYPE, class LOCK> TYPE *
ACE_Singleton<TYPE, LOCK>::instance_ = 0;

// Lock the creation of the singleton.  
template <class TYPE, class LOCK> LOCK
ACE_Singleton<TYPE, LOCK>::ace_singleton_lock_;
#endif /* !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

#endif /* ACE_SINGLETON_C */
