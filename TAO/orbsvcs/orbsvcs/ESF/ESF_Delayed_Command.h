/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Delayed_Command
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_ESF_DELAYED_COMMAND_H
#define TAO_ESF_DELAYED_COMMAND_H

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class Target, class Object>
class TAO_ESF_Connected_Command : public ACE_Command_Base
{
  // = TITLE
  //   ESF_Connected_Command
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
  TAO_ESF_Connected_Command (Target *target,
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
class TAO_ESF_Reconnected_Command : public ACE_Command_Base
{
  // = TITLE
  //   ESF_Reconnected_Command
  //
  // = DESCRIPTION
  //   Implements a Command object that invokes the reconnected_i() method
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
  TAO_ESF_Reconnected_Command (Target *target,
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
class TAO_ESF_Disconnected_Command : public ACE_Command_Base
{
  // = TITLE
  //   ESF_Disconnected_Command
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
  TAO_ESF_Disconnected_Command (Target *target,
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

template<class Target>
class TAO_ESF_Shutdown_Command : public ACE_Command_Base
{
  // = TITLE
  //   ESF_Shutdown_Command
  //
  // = DESCRIPTION
  //   Implements a Command object that invokes the shutdown_i()
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
  TAO_ESF_Shutdown_Command (Target *target);
  // constructor...

  virtual int execute (void *arg);
  // The callback method, if the argument is not nil it is interpreted
  // as a CORBA::Environment.

private:
  Target *target_;
  // The target
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ESF_Delayed_Command.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Delayed_Command.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Delayed_Command.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_DELAYED_COMMAND_H */
