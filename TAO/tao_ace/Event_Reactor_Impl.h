/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Reactor_Impl.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_EVENT_REACTOR_IMPLEMENTATION_H
#define TAO_ACE_EVENT_REACTOR_IMPLEMENTATION_H
#include "ace/pre.h"
#include "Event_Reactor_Interface.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */




class TAO_ACE_IO_Event_Handler;
class TAO_ACE_Event_Wait_Strategy;
class TAO_ACE_Event_Dispatcher;

/**
 * @class TAO_ACE_Event_Reactor_Impl
 *
 * @brief Provides an abstract interface for
 *
 * @@ todo<Bala>: Write comments.
 */

class TAO_ACE_Export TAO_ACE_Event_Reactor_Impl: public TAO_ACE_Event_Reactor_Interface
{
public:

  friend class TAO_ACE_Event_Dispatcher;

  TAO_ACE_Event_Reactor_Impl (void);

  ~TAO_ACE_Event_Reactor_Impl (void);

  int register_io_handler (
      TAO_ACE_IO_Event_Handler *eh,
      TAO_ACE_IO_Event_Masks::Event_Masks mask);

  int remove_io_handler (
      TAO_ACE_IO_Event_Handler *eh,
      TAO_ACE_IO_Event_Masks::Event_Masks mask);

  int add_event_masks (
      TAO_ACE_IO_Event_Handler *eh,
      TAO_ACE_IO_Event_Masks::Event_Masks mask);

  int clr_event_masks (
      TAO_ACE_IO_Event_Handler *eh,
      TAO_ACE_IO_Event_Masks::Event_Masks mask);

  int handle_events (ACE_Time_Value *time);

private:

  int handle_io_events (void);

private:

  TAO_ACE_Event_Wait_Strategy *event_wait_strategy_;

  TAO_ACE_Event_Dispatcher *event_dispatcher_;
};

#if defined (__ACE_INLINE__)
#include "Event_Reactor_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_ACE_EVENT_REACTOR_IMPLEMENTATION_H*/
