/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_Input_Handler.cpp
//
// = DESCRIPTION
//    Implementation of the Consumer_Signal_Handler class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#include <Consumer_Signal_Handler.h>

Consumer_Signal_Handler:: Consumer_Signal_Handler (Consumer_Handler *consumer_handler)
{
  consumer_handler_ = consumer_handler;
}

Consumer_Signal_Handler:: ~Consumer_Signal_Handler ()
{
}

int
Consumer_Signal_Handler::handle_signal (int signum, siginfo_t*, ucontext_t*)
{
  switch (signum)
    {
    case SIGWINCH:
      ACE_DEBUG ((LM_DEBUG,
		  "Signal received on terminal window change!\n"));
      break;

    case SIGINT:
      ACE_DEBUG ((LM_DEBUG,
		  " Exiting on receiving ^C\n"));
      break;
    }

  return 0;
}
