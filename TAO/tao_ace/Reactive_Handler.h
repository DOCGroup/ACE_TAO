/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Reactive_Handler.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_REACTIVE_HANDLER_H
#define TAO_ACE_REACTIVE_HANDLER_H
#include "ace/pre.h"
#include "ace/Atomic_Op.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_ACE_export.h"

/**
 * @class TAO_ACE_Reactive_Handler
 *
 * @brief Provides an abstract interface for handling various types of
 * Events.
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_Reactive_Handler
{
public:
  /// Priority as per the CORBA spec.
  enum
  {
    MIN_PRIORITY = 0,
    MAX_PRIORITY = 32767
  };

  /// Destructor is virtual to enable proper cleanup
  virtual ~TAO_ACE_Reactive_Handler (void);

  /// Incr/Decr reference counts and return the value of the reference
  /// counts.
  long incr_ref_count (void);
  long decr_ref_count (void);

  /// return the value of the refcount
  long refcount (void) const;

  // = Get/set priority
  /* Priorities run from MIN_PRIORITY (which is the "lowest priority")
   * to MAX_PRIORITY (which is the "highest priority"). The set method
   * does error checking and sets priority only in the range of MIN to
   * MAX priorities.
   * @todo: Need to investigate whether we get anything by putting the
   * priority in the base class.
   */
  long priority (void) const;
  void priority (long priority);

protected:

  /* Constructor is protected so that this class cannot be
   * instantiated by anyone
   */
  TAO_ACE_Reactive_Handler (long priority =
                            TAO_ACE_Reactive_Handler::MIN_PRIORITY);

private:

  /// Utility typedef
  typedef ACE_Atomic_Op <ACE_SYNCH_MUTEX, long> Atomic_Counter;

  /* Reference count for memory management. Object of this class will
   * be created with reference count of 1, implying that the creator
   * owns us. If he transfers ownership to someone else, the other
   * person should increment the refcount and the creator should
   * decrement the refcount. This object will be deleted when the
   * refcount goes to 0.
   */
  Atomic_Counter  ref_count_;

  /// Priority of the handler
  long priority_;
};


#if defined (__ACE_INLINE__)
#include "Reactive_Handler.inl"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /*TAO_ACE_REACTIVE_HANDLER*/
