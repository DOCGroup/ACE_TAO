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
      quit_on_signal ();
     break;

    case SIGINT:
      ACE_DEBUG ((LM_DEBUG,
		  " Exiting on receiving ^C\n"));
      quit_on_signal ();
      break;
    }

  return 0;
}

int
Consumer_Signal_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  // End of the signal handler.
  delete this;

  return 0;
}


int
Consumer_Signal_Handler::quit_on_signal ()
{
  // Only if the consumer is registered and wants to shut
  // down, its necessary to unregister and then shutdown.

  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      if (consumer_handler_->unregistered_ != 1 && consumer_handler_->registered_ == 1)
	{
	  this->consumer_handler_->server_->unregister_callback (this->consumer_handler_->consumer_var_.in ());
	  ACE_DEBUG ((LM_DEBUG,
		      " Consumer Unregistered \n "));

	  TAO_CHECK_ENV;
	}
      this->consumer_handler_->consumer_servant_->shutdown (TAO_TRY_ENV);

    }

  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Consumer_Input_Handler::quit_consumer_process()");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
