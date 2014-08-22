/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Consumer_Input_Handler.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 */
//=============================================================================


#ifndef CONSUMER_INPUT_HANDLER_H
#define CONSUMER_INPUT_HANDLER_H

#include "ace/Service_Config.h"
#include "ace/Event_Handler.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class Consumer_Handler;

/**
 * @class Consumer_Input_Handler
 *
 * @brief Handles input events generated from a keyboard.
 *
 * This class holds a <Consumer_Handler> object. It accepts input
 * from the user and passes events to the <Notifier>. It also
 * initiates shutdown.
 */
class Consumer_Input_Handler : public ACE_Event_Handler
{
public:
  // Initialization and termination methods.
  /// Constructor.
  Consumer_Input_Handler(void);

  /// Destructor.
  ~Consumer_Input_Handler (void);

  /// registers the input event handler.
  /// returns 0 on success, -1 on error.
  int initialize (Consumer_Handler *);

  /// Dispatch the callback when events occur.
  virtual int handle_input (ACE_HANDLE);

  /// Close down the handler.
  int close (void);

  /// Report whether the Consumer initiated the shutdown.
  int consumer_initiated_shutdown (void);

  /// Indicate that the Consumer initiated the shutdown.
  void consumer_initiated_shutdown (int);

private:
  /// Pointer to the <Consumer_Handler> that receives notifications
  /// from the <Event_Comm::Notifier>.
    Consumer_Handler *receiver_handler_;

  /// Keep track of whether the Consumer initiated the shutdown.
  int consumer_initiated_shutdown_;
};

#endif /* CONSUMER_INPUT_HANDLER_H */
