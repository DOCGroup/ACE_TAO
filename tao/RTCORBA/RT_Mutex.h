// -*- C++ -*-

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

#include /**/ "ace/pre.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTCORBA/RTCORBA_includeC.h"
#include "tao/LocalObject.h"

#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
# include "ace/SString.h"
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    public ::CORBA::LocalObject
{
public:

  /// Acquire the lock.
  virtual void lock (void);

  /// Release the lock.
  virtual void unlock (void);

  /**
   * Acquire the lock, but only wait up to @a max_wait time.  Note
   * that this operation may not be available on all OS platforms, so
   * if you're interested in writing maximally portable programs avoid
   * using this operation in your program designs.
   */
  virtual CORBA::Boolean try_lock (TimeBase::TimeT max_wait);

  /// Returns the name of the mutex.
  virtual const char *name (void) const;

  /// Destructor.
  virtual ~TAO_RT_Mutex (void);

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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_RT_MUTEX_H */
