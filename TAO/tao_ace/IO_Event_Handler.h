/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    IO_Event_Handler.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_IO_EVENT_HANDLER_H
#define TAO_ACE_IO_EVENT_HANDLER_H
#include "ace/pre.h"
#include "IO_Event_Masks.h"



#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Reactive_Handler.h"

/**
 * @class TAO_ACE_IO_Event_Handler
 *
 * @brief Provides an abstract interface for handling socket events.
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_IO_Event_Handler: public TAO_ACE_Reactive_Handler
{
public:


  /* Callback function that invokes the right callback method based
   * on the type of mask that is passed via <t>. The mask should
   * indicate only one event. Sending a bitwise 'or' or 'and' of many
   * events will lead to none of the events being handled.
   */
  virtual int handle_event (TAO_ACE_IO_Event_Masks::Event_Masks t =
                            TAO_ACE_IO_Event_Masks::NULL_MASK);

  /// Get/Set the handle of the underlying socket
  virtual ACE_HANDLE get_handle (void) = 0;
  virtual void set_handle (ACE_HANDLE h) = 0;
protected:

  /// Called when input events occur (e.g., connection or data).
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE) = 0;

  /// Called when output events are possible (e.g., when flow control
  /// abates or non-blocking connection completes).
  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE) = 0;

  /// Called when an exceptional events occur (e.g., SIGURG).
  virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE) = 0;

private:

};


#include "ace/post.h"
#endif /*TAO_ACE_IO_EVENT_HANDLER_H*/
