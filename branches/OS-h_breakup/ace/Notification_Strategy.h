/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notification_Strategy.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================
#ifndef ACE_NOTIFICATION_STRATEGY_H
#define ACE_NOTIFICATION_STRATEGY_H
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
 * @brief Abstract class used for notifying an interested party
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


#if defined (__ACE_INLINE__)
#include "ace/Notification_Strategy.inl"
#endif /* __ACE_INLINE __ */

#include "ace/post.h"
#endif /*ACE_NOTIFICATION_STRATEGY_H */
