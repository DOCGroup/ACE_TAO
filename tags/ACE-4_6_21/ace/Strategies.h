/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//   Strategies.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_STRATEGIES_H
#define ACE_STRATEGIES_H

#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decls.
class ACE_Reactor;

class ACE_Export ACE_Notification_Strategy
{
  // = TITLE
  //     Abstract class used for notifing an interested party
  //
  // = DESCRIPTION
  //     A vehicle for extending the behavior of ACE_Message_Queue wrt
  //     notification *without subclassing*.  Thus, it's an example of
  //     the Bridge/Strategy patterns.
public:
  ACE_Notification_Strategy (ACE_Event_Handler *eh,
                             ACE_Reactor_Mask mask);
  virtual ~ACE_Notification_Strategy (void);

  virtual int notify (void) = 0;
  virtual int notify (ACE_Event_Handler *,
                      ACE_Reactor_Mask mask) = 0;

  // Get/Set the event handler
  ACE_Event_Handler *event_handler (void);
  void event_handler (ACE_Event_Handler *eh);

  // Get/Set the reactor mask
  ACE_Reactor_Mask mask (void);
  void mask (ACE_Reactor_Mask m);

protected:
  ACE_Event_Handler *eh_;
  ACE_Reactor_Mask mask_;
};

class ACE_Export ACE_Reactor_Notification_Strategy : public ACE_Notification_Strategy
{
  // = TITLE
  //     Used to notify an ACE_Reactor
  //
  // = DESCRIPTION
  //     Integrates the ACE_Message_Queue notification into the
  //     ACE_Reactor::notify() method.
public:
  ACE_Reactor_Notification_Strategy (ACE_Reactor *reactor,
                                     ACE_Event_Handler *eh,
                                     ACE_Reactor_Mask mask);

  virtual ~ACE_Reactor_Notification_Strategy (void);
  // Default dtor.

  virtual int notify (void);

  virtual int notify (ACE_Event_Handler *,
                      ACE_Reactor_Mask mask);

  // Get/Set the reactor
  ACE_Reactor *reactor (void);
  void reactor (ACE_Reactor *r);

protected:
  ACE_Reactor *reactor_;
};

class ACE_Export ACE_Connection_Recycling_Strategy
{
  // = TITLE
  //     Defines the interface for a connection recycler.
public:
  virtual ~ACE_Connection_Recycling_Strategy (void);
  // Virtual Destructor

  virtual int purge (const void *recycling_act) = 0;
  // Remove from cache.

  virtual int cache (const void *recycling_act) = 0;
  // Add to cache.

  virtual int mark_as_closed (const void *recycling_act) = 0;
  // Mark as closed.

  virtual int cleanup_hint (const void *recycling_act) = 0;
  // Cleanup as hint.

protected:
  ACE_Connection_Recycling_Strategy (void);
  // Default ctor.
};

class ACE_Export ACE_Recyclable
{
public:
  enum State
  {
    IDLE,
    BUSY,
    CLOSED,
    UNKNOWN
  };

  virtual ~ACE_Recyclable (void);
  // Destructor.

  // = Set/Get the recyclable bit
  State state (void) const;
  void state (State new_state);

protected:
  ACE_Recyclable (State initial_state);
  // Protected constructor.

  State state_;
  // Our state.
};

class ACE_Export ACE_Hashable
{
public:
  virtual ~ACE_Hashable (void);
  // Destructor.

  virtual u_long hash (void) const;
  // Computes and returns hash value.  This "caches" the hash value to
  // improve performance.

protected:
  ACE_Hashable (void);
  // Protected constructor.

  virtual u_long hash_i (void) const = 0;
  // This is the method that actually performs the non-cached hash
  // computation.

  u_long hash_value_;
  // Pre-computed hash-value.

};

class ACE_Export ACE_Refcountable
{
public:
  virtual ~ACE_Refcountable (void);
  // Destructor.

  // = Increment/Decrement refcount
  int increment (void);
  int decrement (void);

  int refcount (void) const;
  // Returns the current refcount.

protected:
  ACE_Refcountable (int refcount);
  // Protected constructor.

  int refcount_;
  // Current refcount.
};

// This needs to come here to avoid circular dependencies.
#include "ace/Strategies_T.h"

#if defined (__ACE_INLINE__)
#include "ace/Strategies.i"
#endif /* __ACE_INLINE __ */

#endif /* ACE_STRATEGIES_H */
