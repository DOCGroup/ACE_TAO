/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    EventComm
//
// = FILENAME
//    Supplier_Input_Handler.h
//
// = DESCRIPTION
//    Handle input from the keyboard.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu) and
//    Pradeep Gore (pradeep@cs.wustl.edu)
//
// ============================================================================

#ifndef SUPPLIER_INPUT_HANDLER_H
#define SUPPLIER_INPUT_HANDLER_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class Notifier_Handler;

class Supplier_Input_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   Handles input events generated from a keyboard.
  //
  // = DESCRIPTION
  //   The events are currently framed and forwarded to all Consumers.
  //   In the future, we will need to be more selective and only send
  //   to those Consumers whose filtering criteria matches!
public:
  // = Initialization and termination methods.
  Supplier_Input_Handler (void);
  // Constructor.

  ~Supplier_Input_Handler (void);
  // Destructor.

  int initialize (Notifier_Handler *);
  // Initialization.

  virtual int handle_input (ACE_HANDLE);
  // Frame input events and notify <Consumers>.

  int close (void);
  // Close down the handler.

protected:
    Notifier_Handler *notifier_;
  // Pointer to a <Notifier_Handler> that's used to inform Consumers
  // that events of interest have occurred.
};

#endif /* SUPPLIER_INPUT_HANDLER_H */
