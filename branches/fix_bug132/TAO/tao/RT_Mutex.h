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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/RTCORBAS.h"
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

class TAO_Export TAO_RT_Mutex : public RTCORBA::Mutex, public TAO_Local_RefCounted_Object
{
public:
  /// Constructor.
  TAO_RT_Mutex (void);

  /// Destructor.
  virtual ~TAO_RT_Mutex (void);

  /// Acquire the lock.
  virtual void lock (CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Release the lock.
  virtual void unlock (CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Acquire the lock, but only wait up to <max_wait> time.  Note
  //that this operation may not be available on all OS platforms, so
  //if you're interested in writing maximally portable programs avoid
  //using this operation in your program designs.
  virtual CORBA::Boolean try_lock (TimeBase::TimeT max_wait,
                                   CORBA::Environment &ACE_TRY_ENV =
                                   TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  /// Synchronization lock.
  TAO_SYNCH_MUTEX mu_;
};

// Forward reference for the TAO_Named_RT_Mutex class
class TAO_Named_RT_Mutex_Manager;

/**
 * @class TAO_Named_RT_Mutex
 *
 * @brief Extension to TAO_RT_Mutex to support named mutexes.
 *
 */

class TAO_Export TAO_Named_RT_Mutex : public TAO_RT_Mutex
{
public:
  /// Constructor.
  TAO_Named_RT_Mutex (const char *name,
                      TAO_Named_RT_Mutex_Manager &mutex_mgr);

  /// Destructor.
  virtual ~TAO_Named_RT_Mutex (void);

protected:
  // Don't allow unnamed named mutexes
  TAO_Named_RT_Mutex (void);

  char *name_;

  TAO_Named_RT_Mutex_Manager &mutex_mgr_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif /* TAO_RT_MUTEX_H */
