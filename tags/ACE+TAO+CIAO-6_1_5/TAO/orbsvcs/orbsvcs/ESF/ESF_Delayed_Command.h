// -*- C++ -*-

/**
 *  @file   ESF_Delayed_Command.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_DELAYED_COMMAND_H
#define TAO_ESF_DELAYED_COMMAND_H

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ESF_Connected_Command
 *
 * @brief Implements a Command object that invokes the connected_i()
 * method on the target, passing an argument of type Object.
 *
 * <H2>Memory Managment</H2>
 * It does not assume ownership of Object nor the Target
 * arguments.
 * Usually allocated from the heap or an allocator; but it is not
 * self-managed.
 *
 * <H2>Locking</H2>
 * No provisions for locking, access must be serialized
 * externally.
 */
template<class Target, class Object>
class TAO_ESF_Connected_Command : public ACE_Command_Base
{
public:
  /// constructor...
  TAO_ESF_Connected_Command (Target *target,
                            Object *object);

  /// The callback method, if the argument is not nil it is interpreted
  /// as a CORBA::Environment.
  virtual int execute (void *arg);

private:
  /// The target
  Target *target_;

  /// The argument
  Object *object_;
};

// ****************************************************************

/**
 * @class TAO_ESF_Reconnected_Command
 *
 * @brief Implements a Command object that invokes the reconnected_i()
 * method on the target, passing an argument of type Object.
 *
 * <H2>Memory Managment</H2>
 * It does not assume ownership of Object nor the Target
 * arguments.
 * Usually allocated from the heap or an allocator; but it is not
 * self-managed.
 *
 * <H2>Locking</H2>
 * No provisions for locking, access must be serialized
 * externally.
 */
template<class Target, class Object>
class TAO_ESF_Reconnected_Command : public ACE_Command_Base
{
public:
  /// constructor...
  TAO_ESF_Reconnected_Command (Target *target,
                            Object *object);

  /// The callback method, if the argument is not nil it is interpreted
  /// as a CORBA::Environment.
  virtual int execute (void *arg);

private:
  /// The target
  Target *target_;

  /// The argument
  Object *object_;
};

// ****************************************************************

/**
 * @class TAO_ESF_Disconnected_Command
 *
 * @brief Implements a Command object that invokes the
 * disconnected_i() method on the target, passing an argument of type
 * Object.
 *
 * <H2>Memory Managment</H2>
 * It does not assume ownership of Object nor the Target
 * arguments.
 * Usually allocated from the heap or an allocator; but it is not
 * self-managed.
 *
 * <H2>Locking</H2>
 * No provisions for locking, access must be serialized
 * externally.
 */
template<class Target, class Object>
class TAO_ESF_Disconnected_Command : public ACE_Command_Base
{
public:
  /// constructor...
  TAO_ESF_Disconnected_Command (Target *target,
                               Object *object);

  /// The callback method, if the argument is not nil it is interpreted
  /// as a CORBA::Environment.
  virtual int execute (void *arg);

private:
  /// The target
  Target *target_;

  /// The argument
  Object *object_;
};

// ****************************************************************

/**
 * @class TAO_ESF_Shutdown_Command
 *
 * @brief Implements a Command object that invokes the shutdown_i()
 * method on the target, passing an argument of type Object.
 *
 * <H2>Memory Management</H2>
 * It does not assume ownership of Object nor the Target
 * arguments.
 * Usually allocated from the heap or an allocator; but it is not
 * self-managed.
 *
 * <H2>Locking</H2>
 * No provisions for locking, access must be serialized
 * externally.
 */
template<class Target>
class TAO_ESF_Shutdown_Command : public ACE_Command_Base
{
public:
  /// constructor...
  TAO_ESF_Shutdown_Command (Target *target);

  /// The callback method, if the argument is not nil it is interpreted
  /// as a CORBA::Environment.
  virtual int execute (void *arg);

private:
  /// The target
  Target *target_;
};

// ****************************************************************

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Delayed_Command.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Delayed_Command.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Delayed_Command.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_DELAYED_COMMAND_H */
