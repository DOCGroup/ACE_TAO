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
//    Definition of the Consumer_Signal_Handler class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef CONSUMER_SIGNAL_HANDLER_H
#define CONSUMER_SIGNAL_HANDLER_H

#include <signal.h>
#include <ace/Reactor.h>
#include <ace/Event_Handler.h>
#include "Consumer_Handler.h"

// Creating a class to handle signal events.
// Since only signals need to be handled, only the handle_signal
// method is overlaoded.

class Consumer_Handler;

class Consumer_Signal_Handler : public ACE_Event_Handler
{
public:

  Consumer_Signal_Handler (Consumer_Handler *consumer_handler);
  // The consumer_handler refernce will be used to access
  // the servant methods.

  virtual int handle_signal (int signum, siginfo_t*, ucontext_t*);
  // This method takes action on an signal event.
private:

 ~Consumer_Signal_Handler ();

 Consumer_Handler *consumer_handler_;

};


#endif /* CONSUMER_SIGNAL_HANDLER_H */
