/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Reactor_Interface.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_EVENT_REACTOR_INTERFACE_H
#define TAO_ACE_EVENT_REACTOR_INTERFACE_H
#include "ace/pre.h"
#include "IO_Event_Masks.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ACE_IO_Event_Handler;
class ACE_Time_Value;

/**
 * @class TAO_ACE_Event_Reactor_Interface
 *
 * @brief Provides an abstract interface for
 *
 * @@ todo<Bala>: Write comments.
 */

class TAO_ACE_Export TAO_ACE_Event_Reactor_Interface
{
public:

  virtual ~TAO_ACE_Event_Reactor_Interface (void);

  virtual int register_io_handler (
      TAO_ACE_IO_Event_Handler *eh,
      TAO_ACE_IO_Event_Masks::Event_Masks mask) = 0;

  virtual int remove_io_handler (
      TAO_ACE_IO_Event_Handler *eh,
      TAO_ACE_IO_Event_Masks::Event_Masks mask) = 0;

  virtual int add_event_masks (
      TAO_ACE_IO_Event_Handler *eh,
      TAO_ACE_IO_Event_Masks::Event_Masks mask) = 0;

  virtual int clr_event_masks (
      TAO_ACE_IO_Event_Handler *eh,
      TAO_ACE_IO_Event_Masks::Event_Masks mask) = 0;

  virtual int handle_events (ACE_Time_Value *time) = 0;

};


#include "ace/post.h"
#endif /*TAO_ACE_EVENT_REACTOR_INTERFACE_H*/
