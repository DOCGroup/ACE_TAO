/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO
//
// = DESCRIPTION
//   Implementation of RTCORBA::Mutex interface.
//
// = FILENAME
//   RT_Mutex.h
//
// = AUTHOR
//   Marina Spivak (marina@cs.wustl.edu)
//
// ============================================================================

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

class TAO_Export TAO_RT_Mutex :
  public RTCORBA::Mutex,
  public CORBA::LocalObject
{
  // = TITLE
  //   RTCORBA::Mutex implementation.
  //
  // = DESCRIPTION
  //   Placeholder for mutex implementation.
  //
public:
  TAO_RT_Mutex (void);
  // Constructor.

  virtual ~TAO_RT_Mutex (void);
  // Destructor.

  virtual void lock (CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void unlock (CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean try_lock (TimeBase::TimeT max_wait,
                                   CORBA::Environment &ACE_TRY_ENV =
                                   TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  // = Override CORBA::LocalObject no-op methods to
  // provide reference counting.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.
};

#if defined (__ACE_INLINE__)
#include "tao/RT_Mutex.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif /* TAO_RT_MUTEX_H */
