/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Supplier_Input_Handler.h
 *
 *  Handle input from the keyboard.
 *
 *  @author Douglas C. Schmidt (d.schmidt@vanderbilt.edu) and Pradeep Gore (pradeep@cs.wustl.edu)
 */
//=============================================================================


#ifndef SUPPLIER_INPUT_HANDLER_H
#define SUPPLIER_INPUT_HANDLER_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class Notifier_Handler;

/**
 * @class Supplier_Input_Handler
 *
 * @brief Handles input events generated from a keyboard.
 *
 * The events are currently framed and forwarded to all Consumers.
 * In the future, we will need to be more selective and only send
 * to those Consumers whose filtering criteria matches!
 */
class Supplier_Input_Handler : public ACE_Event_Handler
{
public:
  /// Constructor.
  Supplier_Input_Handler ();

  /// Destructor.
  ~Supplier_Input_Handler ();

  /// Initialization.
  int initialize (Notifier_Handler *);

  /// Frame input events and notify <Consumers>.
  virtual int handle_input (ACE_HANDLE);

  /// Close down the handler.
  int close ();

protected:
  /// Pointer to a <Notifier_Handler> that's used to inform Consumers
  /// that events of interest have occurred.
    Notifier_Handler *notifier_;
};

#endif /* SUPPLIER_INPUT_HANDLER_H */
