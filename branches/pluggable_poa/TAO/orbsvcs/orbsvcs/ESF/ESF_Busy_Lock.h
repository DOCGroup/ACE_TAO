/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Busy_Lock
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_ESF_BUSY_LOCK_H
#define TAO_ESF_BUSY_LOCK_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class Adaptee>
class TAO_ESF_Busy_Lock_Adapter
{
public:
  TAO_ESF_Busy_Lock_Adapter (Adaptee* adaptee);
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
#include "ESF_Busy_Lock.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Busy_Lock.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Busy_Lock.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_BUSY_LOCK_H */
