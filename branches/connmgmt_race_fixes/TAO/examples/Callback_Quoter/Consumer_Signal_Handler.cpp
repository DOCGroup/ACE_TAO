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

#include "Consumer_Signal_Handler.h"

Consumer_Signal_Handler::Consumer_Signal_Handler (Consumer_Handler *consumer_handler)
  : consumer_handler_ (consumer_handler)
{
}

Consumer_Signal_Handler:: ~Consumer_Signal_Handler (void)
{
}

// Method to handle the ^C signal.
int
Consumer_Signal_Handler::handle_signal (int /* signum */,
                                        siginfo_t*,
                                        ucontext_t*)
{
  ACE_DEBUG ((LM_DEBUG,
              " Exiting on receiving ^C\n"));

  quit_on_signal ();
  
  return 0;
}

// Method called before the Event_Handler dies.
int
Consumer_Signal_Handler::handle_close (ACE_HANDLE,
                                       ACE_Reactor_Mask)
{
  // End of the signal handler.
  delete this;

  return 0;
}

int
Consumer_Signal_Handler::quit_on_signal (void)
{
  // Only if the consumer is registered and wants to shut down, its
  // necessary to unregister and then shutdown.

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      if (consumer_handler_->unregistered_ != 1
          && consumer_handler_->registered_ == 1)
	{
	  this->consumer_handler_->server_->unregister_callback 
            (this->consumer_handler_->consumer_var_.in ());
	  ACE_DEBUG ((LM_DEBUG,
		      "Consumer Unregistered\n"));
        }
      this->consumer_handler_->consumer_servant_->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Consumer_Input_Handler::quit_consumer_process()");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
