/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   RT_Mutex.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_RT_MUTEX_H
#define TAO_RT_MUTEX_H

#include "ace/pre.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define TAO_RTCORBA_SAFE_INCLUDE
#include "RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_RT_Mutex
 *
 * @brief  Abstract base class for the TAO RT Mutex implementations
 *
 * This class just serves as a base class for any of the TAO
 * RT Mutex implementations.  Instances of these classes should
 * be created using the RTCORBA::create_mutex() method.
 *
 */

class TAO_RTCORBA_Export TAO_RT_Mutex
  : public RTCORBA::Mutex,
    public TAO_Local_RefCounted_Object
{
public:
  /// Constructor.
  TAO_RT_Mutex (void);

  /// Destructor.
  virtual ~TAO_RT_Mutex (void);

  /// Acquire the lock.
  virtual void lock (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Release the lock.
  virtual void unlock (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Acquire the lock, but only wait up to @a max_wait time.  Note
   * that this operation may not be available on all OS platforms, so
   * if you're interested in writing maximally portable programs avoid
   * using this operation in your program designs.
   */
  virtual CORBA::Boolean try_lock (TimeBase::TimeT max_wait
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns the name of the mutex.
  virtual const char *name (void) const;

protected:
  /// Synchronization lock.
  TAO_SYNCH_MUTEX mu_;
};

#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
/**
 * @class TAO_Named_RT_Mutex
 *
 * @brief Extension to TAO_RT_Mutex to support named mutexes.
 *
 */
class TAO_RTCORBA_Export TAO_Named_RT_Mutex : public TAO_RT_Mutex
{
public:
  /// Constructor.
  TAO_Named_RT_Mutex (const char *name);

  /// Returns the name of the mutex.
  virtual const char *name (void) const;

protected:

  /// My name.
  ACE_CString name_;
};
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include "ace/post.h"
#endif /* TAO_RT_MUTEX_H */
