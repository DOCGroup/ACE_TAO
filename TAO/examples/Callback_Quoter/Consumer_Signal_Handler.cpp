/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Consumer_Input_Handler.cpp
 *
 *  $Id$
 *
 *  Implementation of the Consumer_Signal_Handler class.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


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


  try
    {
      if (consumer_handler_->unregistered_ != 1
          && consumer_handler_->registered_ == 1)
        {
          this->consumer_handler_->server_->unregister_callback
            (this->consumer_handler_->consumer_var_.in ());
          ACE_DEBUG ((LM_DEBUG,
                      "Consumer Unregistered\n"));
        }
      this->consumer_handler_->consumer_servant_->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Consumer_Input_Handler::quit_consumer_process()");
      return -1;
    }

  return 0;
}
