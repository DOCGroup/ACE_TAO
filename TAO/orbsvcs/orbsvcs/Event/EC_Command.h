/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Command
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   Implement the Command objects for the delayed operations in the
//   manipulation of EC_ProxyPushSupplier and EC_ProxyPushConsumer
//   sets.
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

#ifndef TAO_EC_COMMAND_H
#define TAO_EC_COMMAND_H

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class Target, class Object>
class TAO_EC_Connected_Command : public ACE_Command_Base
{
  // = TITLE
  //   EC_Connected_Command
  //
  // = DESCRIPTION
  //   Implements a Command object that invokes the connected_i() method
  //   on the target, passing an argument of type Object.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of Object nor the Target
  //   arguments.
  //   Usually allocated from the heap or an allocator; but it is not
  //   self-managed.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  TAO_EC_Connected_Command (Target *target,
                            Object *object);
  // constructor...

  virtual int execute (void *arg);
  // The callback method, if the argument is not nil it is interpreted
  // as a CORBA::Environment.

private:
  Target *target_;
  // The target

  Object *object_;
  // The argument
};

// ****************************************************************

template<class Target, class Object>
class TAO_EC_Disconnected_Command : public ACE_Command_Base
{
  // = TITLE
  //   EC_Disconnected_Command
  //
  // = DESCRIPTION
  //   Implements a Command object that invokes the disconnected_i()
  //   method on the target, passing an argument of type Object.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of Object nor the Target
  //   arguments.
  //   Usually allocated from the heap or an allocator; but it is not
  //   self-managed.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  TAO_EC_Disconnected_Command (Target *target,
                               Object *object);
  // constructor...

  virtual int execute (void *arg);
  // The callback method, if the argument is not nil it is interpreted
  // as a CORBA::Environment.

private:
  Target *target_;
  // The target

  Object *object_;
  // The argument
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Command.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "EC_Command.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("EC_Command.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_EC_COMMAND_H */
