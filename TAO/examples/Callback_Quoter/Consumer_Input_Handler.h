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

#include <signal.h>
#include <ace/Reactor.h>
#include <ace/Event_Handler.h>
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

  int handle_input (ACE_HANDLE);
  // Handle the user input.

  int register_consumer ();
  // Registration with the notifier.

  int unregister_consumer ();
  // Cancelling the registration with the notifier.

  int quit_consumer_process ();
  // Ends the consumer process.

  friend class ACE_Shutup_GPlusPlus;
  // Turn off g++ warning

private:

  ~Consumer_Input_Handler ()
   {
     // No-op
    }

  Consumer_Handler *consumer_handler_;
  // The Consumer_Handler object.

};

#define REGISTER 'r'
// The character that the user must type to register the consumer with
// the Notifier_server.

#define UNREGISTER 'u'
// The character that the user must type to unregister the consumer with
// the Notifier_server.

#define EXIT 'q'
// The character the user must type to quit the consumer client
// application.

#endif /* CONSUMER_INPUT_HANDLER_H */
