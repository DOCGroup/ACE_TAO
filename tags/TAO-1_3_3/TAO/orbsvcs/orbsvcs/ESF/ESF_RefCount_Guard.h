/* -*- C++ -*- */
/**
 *  @file   ESF_RefCount_Guard.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_REFCOUNT_GUARD_H
#define TAO_ESF_REFCOUNT_GUARD_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_ESF_RefCount_Guard
 *
 * @brief Reference count based guard.
 *
 * A common idiom used on event services is to increment a
 * reference count before starting a long running operation.
 * The system can then execute the operation without any risk of
 * having the underlying object destroyed.  The advantage of using
 * a reference count is that no mutex or lock needs to be held
 * while the operation is beign executed.
 */
template<class T>
class TAO_ESF_RefCount_Guard
{
public:
  /// Constructor
  TAO_ESF_RefCount_Guard (T &refcount);

  /// Destructor
  ~TAO_ESF_RefCount_Guard (void);

protected:
  /// The reference count
  T &refcount_;
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
