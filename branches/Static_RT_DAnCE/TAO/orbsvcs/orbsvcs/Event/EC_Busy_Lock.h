// -*- C++ -*-

/**
 *  @file   EC_Busy_Lock.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_BUSY_LOCK_H
#define TAO_EC_BUSY_LOCK_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class Adaptee>
class TAO_EC_Busy_Lock_Adapter
{
public:
  /// Constructor
  TAO_EC_Busy_Lock_Adapter (Adaptee* adaptee);

  // = The ACE_Lock methods, please check $ACE_ROOT/ace/Synch.h for
  // details.

  int remove (void);
  int acquire (void);
  int tryacquire (void);
  int release (void);
  int acquire_read (void);
  int acquire_write (void);
  int tryacquire_read (void);
  int tryacquire_write (void);

private:
  Adaptee* adaptee_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Busy_Lock.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Event/EC_Busy_Lock.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("EC_Busy_Lock.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_EC_BUSY_LOCK_H */
