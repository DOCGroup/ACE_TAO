/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_Input_Handler.h
//
// = DESCRIPTION
//    Definition of the Callback_Qouter Consumer Client class, Consumer_Input_Handler.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef CONSUMER_INPUT_HANDLER_H
#define CONSUMER_INPUT_HANDLER_H

#include "ace/OS.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "Consumer_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Creating a class to handle input events.
// Since only inputs need to be handled, only the handle_input
// method is overlaoded.

class Consumer_Handler;

class Consumer_Input_Handler : public ACE_Event_Handler
{
  // = TITLE
  //    Callback Quoter Consumer Client class.
  //
  // = DESCRIPTION
  //    Connects to the Callback Quoter server and
  //    registers the Consumer object with the it
  //    and receives the stock status from the Notifier.

public:
  Consumer_Input_Handler (Consumer_Handler *consumer_handler);
  // Constructor.

  int handle_input (ACE_HANDLE);
  // Handle the user input.

  int register_consumer (void);
  // Registration with the notifier.

  int unregister_consumer (void);
  // Cancelling the registration with the notifier.

  int quit_consumer_process (void);
  // Ends the consumer process.

  friend class ACE_Shutup_GPlusPlus;
  // Turn off g++ warning

  enum
  {
    // =  TITLE
    //   A set of values for the execution of the consumer.
    //
    // = DESCRIPTION
    //   Used so that the process of registering, unregistering
    //   and exitting neednt be dependent on 'r' 'u' and 'q'.
    //   Also, #define clutters up the global namespace.

    REGISTER = 'r',
    // The character that the user must type to register the consumer with
    // the Notifier_server.

    UNREGISTER = 'u',
    // The character that the user must type to unregister the consumer with
    // the Notifier_server.

    EXIT = 'q'
    // The character the user must type to quit the consumer client
    // application.
  };

private:
  ~Consumer_Input_Handler (void);
  // the destructor.

  Consumer_Handler *consumer_handler_;
  // The Consumer_Handler object.



};

#endif /* CONSUMER_INPUT_HANDLER_H */
