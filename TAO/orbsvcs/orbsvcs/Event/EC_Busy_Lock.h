/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ConsumerAdmin
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   Implement the RtecEventChannelAdmin::ConsumerAdmin interface.
//   This class is an Abstract Factory for the
//   TAO_EC_ProxyPushSupplier.
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_BUSY_LOCK_H
#define TAO_EC_BUSY_LOCK_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class Adaptee>
class TAO_EC_Busy_Lock_Adapter
{
public:
  TAO_EC_Busy_Lock_Adapter (Adaptee* adaptee);
  // Constructor

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

#if defined (__ACE_INLINE__)
#include "EC_Busy_Lock.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "EC_Busy_Lock.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("EC_Busy_Lock.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_EC_BUSY_LOCK_H */
