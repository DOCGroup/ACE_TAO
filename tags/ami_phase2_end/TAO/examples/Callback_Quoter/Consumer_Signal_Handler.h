/* -*- C++ -*- */
// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_Input_Handler.h
//
// = DESCRIPTION
//    Definition of the Consumer_Signal_Handler class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef CONSUMER_SIGNAL_HANDLER_H
#define CONSUMER_SIGNAL_HANDLER_H

#include "ace/OS.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "Consumer_Handler.h"

class Consumer_Handler;

class Consumer_Signal_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   Creating a class to handle signal events.
  //
  // = DESCRIPTION
  //   Since only signals need to be handled, only the <handle_signal> method
  //   is overlaoded.
public:

  Consumer_Signal_Handler (Consumer_Handler *consumer_handler);
  // The consumer_handler refernce will be used to access the servant
  // methods.

  int handle_signal (int signum,
                             siginfo_t*,
                             ucontext_t*);
  // This method takes action on an signal event.

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask);
  // For removal of the signal handler from the dispatch tables.  When
  // the handle_signal () returns < 0 this method will be executed
  // automatically.

private:
  friend class ACE_Shutup_GPlusPlus;
  // Turn off g++ warning

  ~Consumer_Signal_Handler (void);
  // Private destructor so that the signal handler is always created
  // dynamically and hence the heap doesnt get corrupted.

  int quit_on_signal (void);
  // Exit gracefully on a signal.

  Consumer_Handler *consumer_handler_;
  // Reference to the Consumer_Handler which is used in accessing the
  // servant methods.
};

#endif /* CONSUMER_SIGNAL_HANDLER_H */
