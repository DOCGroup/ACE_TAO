/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Reactor.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_EVENT_REACTOR_H
#define TAO_ACE_EVENT_REACTOR_H
#include "ace/pre.h"

#include "IO_Event_Masks.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"

/**
 * @class TAO_ACE_Event_Reactor
 *
 * @brief Provides an abstract interface for
 *
 * @@ todo<Bala>: Write comments.
 */

class TAO_ACE_Event_Reactor_Interface;
class TAO_ACE_IO_Event_Handler;
class ACE_Time_Value;

class TAO_ACE_Export TAO_ACE_Event_Reactor
{
public:

  TAO_ACE_Event_Reactor (TAO_ACE_Event_Reactor_Interface *interface,
                         int delete_impl = 0);

  TAO_ACE_Event_Reactor (void);

  ~TAO_ACE_Event_Reactor (void);

  /// Get pointer to a process-wide <ACE_Reactor>.
  static TAO_ACE_Event_Reactor *instance (void);

  /**
   * Set pointer to a process-wide <ACE_Reactor> and return existing
   * pointer.  If <delete_reactor> != 0 then we'll delete the Reactor
   * at destruction time.
   */
  static TAO_ACE_Event_Reactor *instance (TAO_ACE_Event_Reactor *,
                                          int delete_reactor = 0);

  /// Delete the dynamically allocated Singleton
  static void close_singleton (void);

  int register_io_handler (TAO_ACE_IO_Event_Handler *eh,
                           TAO_ACE_IO_Event_Masks::Event_Masks mask);

  int remove_io_handler (TAO_ACE_IO_Event_Handler *eh,
                         TAO_ACE_IO_Event_Masks::Event_Masks mask);

  int add_event_masks (TAO_ACE_IO_Event_Handler *eh,
                       TAO_ACE_IO_Event_Masks::Event_Masks mask);

  int clr_event_masks (TAO_ACE_IO_Event_Handler *eh,
                       TAO_ACE_IO_Event_Masks::Event_Masks mask);

  int handle_events (ACE_Time_Value *time);


private:

  TAO_ACE_Event_Reactor_Interface *reactor_interface_;

  int delete_implementation_;

  /// Pointer to a process-wide <ACE_Reactor> singleton.
  static TAO_ACE_Event_Reactor *reactor_;

  /// Must delete the <reactor_> singleton if non-0.
  static int delete_reactor_;

private:

  /// Deny access since member-wise won't work...
  ACE_UNIMPLEMENTED_FUNC (TAO_ACE_Event_Reactor (const TAO_ACE_Event_Reactor &));
  ACE_UNIMPLEMENTED_FUNC (TAO_ACE_Event_Reactor &operator = (const TAO_ACE_Event_Reactor &));
};


#if defined (__ACE_INLINE__)
#include "Event_Reactor.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_ACE_EVENT_REACTOR_H*/
