/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_RefCount_Guard
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_ESF_REFCOUNT_GUARD_H
#define TAO_ESF_REFCOUNT_GUARD_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class T>
class TAO_ESF_RefCount_Guard
{
  // = TITLE
  //   Reference count based guard.
  //
  // = DESCRIPTION
  //   A common idiom used on event services is to increment a
  //   reference count before starting a long running operation.
  //   The system can then execute the operation without any risk of
  //   having the underlying object destroyed.  The advantage of using
  //   a reference count is that no mutex or lock needs to be held
  //   while the operation is beign executed.
  //
public:
  TAO_ESF_RefCount_Guard (T &refcount);
  // Constructor

  ~TAO_ESF_RefCount_Guard (void);
  // Destructor

protected:
  T &refcount_;
  // The reference count
};

#if defined (__ACE_INLINE__)
#include "ESF_RefCount_Guard.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_RefCount_Guard.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_RefCount_Guard.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_REFCOUNT_GUARD_H */
