/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Strategies.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_STRATEGIES_H
#define ACE_STRATEGIES_H
#include "ace/pre.h"

#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decls.
class ACE_Reactor;

/**
 * @class ACE_Notification_Strategy
 *
 * @brief Abstract class used for notifing an interested party
 *
 * A vehicle for extending the behavior of ACE_Message_Queue wrt
 * notification *without subclassing*.  Thus, it's an example of
 * the Bridge/Strategy patterns.
 */
class ACE_Export ACE_Notification_Strategy
{
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

/**
 * @class ACE_Reactor_Notification_Strategy
 *
 * @brief Used to notify an ACE_Reactor
 *
 * Integrates the <ACE_Message_Queue> notification into the
 * <ACE_Reactor::notify> method.
 */
class ACE_Export ACE_Reactor_Notification_Strategy : public ACE_Notification_Strategy
{
public:
  ACE_Reactor_Notification_Strategy (ACE_Reactor *reactor,
                                     ACE_Event_Handler *eh,
                                     ACE_Reactor_Mask mask);

  /// Default dtor.
  virtual ~ACE_Reactor_Notification_Strategy (void);

  virtual int notify (void);

  virtual int notify (ACE_Event_Handler *,
                      ACE_Reactor_Mask mask);

  // Get/Set the reactor
  ACE_Reactor *reactor (void);
  void reactor (ACE_Reactor *r);

protected:
  ACE_Reactor *reactor_;
};

/**
 * @class ACE_Connection_Recycling_Strategy
 *
 * @brief Defines the interface for a connection recycler.
 */
class ACE_Export ACE_Connection_Recycling_Strategy
{
public:
  /// Virtual Destructor
  virtual ~ACE_Connection_Recycling_Strategy (void);

  /// Remove from cache.
  virtual int purge (const void *recycling_act) = 0;

  /// Add to cache.
  virtual int cache (const void *recycling_act) = 0;

  virtual int recycle_state (const void *recycling_act,
                             ACE_Recyclable_State new_state) = 0;

  /// Get/Set <recycle_state>.
  virtual ACE_Recyclable_State recycle_state (const void *recycling_act) const = 0;

  /// Mark as closed.
  virtual int mark_as_closed (const void *recycling_act) = 0;

  /// Mark as closed.(non-locking version)
  virtual int mark_as_closed_i (const void *recycling_act) = 0;

  /// Cleanup hint and reset <*act_holder> to zero if <act_holder != 0>.
  virtual int cleanup_hint (const void *recycling_act,
                            void **act_holder = 0) = 0;

protected:
  /// Default ctor.
  ACE_Connection_Recycling_Strategy (void);
};

class ACE_Export ACE_Recyclable
{
public:
  /// Destructor.
  virtual ~ACE_Recyclable (void);

  // = Set/Get the recyclable bit
  ACE_Recyclable_State recycle_state (void) const;
  void recycle_state (ACE_Recyclable_State new_state);

protected:
  /// Protected constructor.
  ACE_Recyclable (ACE_Recyclable_State initial_state);

  /// Our state.
  ACE_Recyclable_State recycle_state_;
};

class ACE_Export ACE_Hashable
{
public:
  /// Destructor.
  virtual ~ACE_Hashable (void);

  /// Computes and returns hash value.  This "caches" the hash value to
  /// improve performance.
  virtual u_long hash (void) const;

protected:
  /// Protected constructor.
  ACE_Hashable (void);

  /// This is the method that actually performs the non-cached hash
  /// computation.
  virtual u_long hash_i (void) const = 0;

  /// Pre-computed hash-value.
  u_long hash_value_;

};

class ACE_Export ACE_Refcountable
{
public:
  /// Destructor.
  virtual ~ACE_Refcountable (void);

  // = Increment/Decrement refcount
  int increment (void);
  int decrement (void);

  /// Returns the current refcount.
  int refcount (void) const;

protected:
  /// Protected constructor.
  ACE_Refcountable (int refcount);

  /// Current refcount.
  int refcount_;
};

// This needs to come here to avoid circular dependencies.
#include "ace/Strategies_T.h"

#if defined (__ACE_INLINE__)
#include "ace/Strategies.i"
#endif /* __ACE_INLINE __ */

#include "ace/post.h"
#endif /* ACE_STRATEGIES_H */
