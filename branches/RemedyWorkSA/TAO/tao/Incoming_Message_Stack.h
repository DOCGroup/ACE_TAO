// -*- C++ -*-

//=============================================================================
/**
 *  @file Incoming_Message_Stack.h
 *
 *  $Id$
 *
 *  @author Frank Rehberger <frehberg@prismtech.com>
 */
//=============================================================================

#ifndef TAO_INCOMING_MESSAGE_STACK_H
#define TAO_INCOMING_MESSAGE_STACK_H

#include /**/ "ace/pre.h"

#include "tao/Queued_Data.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Allocator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
/**
 * @class Incoming_Message_Stack
 *
 * @brief Implements stack for TAO_Queued_Data.
 *
 * Internal class, providing stack functionality for TAO_Queued_Data
 * objects.  Stack operations don't require memory allocation.
*/
class Incoming_Message_Stack
{
public:
  /// default constructor, initiliazes empty stack.
  Incoming_Message_Stack();

  /// destructor, releases all elements on stack
  ~Incoming_Message_Stack() ;

  /// pushing a new element onto stack,
  /// @a data must be a valid pointer, not NULL
  void push(TAO_Queued_Data *data);

  /// removing top element of stack,
  /// @return 0 for Ok and @a data is defined, -1 for error
  int pop (TAO_Queued_Data* &data);

  /// peeking top element of stack
  /// @return 0 for Ok, -1 for error
  int top (TAO_Queued_Data* &data) const;

private:
  /// Top element of stack
  TAO_Queued_Data *top_;

private:
  /// Default Copy-Constructor -  not for public usage.
  Incoming_Message_Stack (const Incoming_Message_Stack&);

  /// Assignment operator - not for public usage
  Incoming_Message_Stack& operator= (Incoming_Message_Stack& other);
};
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Incoming_Message_Stack.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_INCOMING_MESSAGE_STACK_H*/
