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
//    Implementation of the Consumer_Input_Handler class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#include "Consumer_Input_Handler.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"

Consumer_Input_Handler::Consumer_Input_Handler (Consumer_Handler *consumer_handler)
  {
    consumer_handler_ = consumer_handler;

  }

int
Consumer_Input_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  TAO_TRY
    {
      // The string could read contains \n\0 hence using ACE_OS::read
      // which returns the no of bytes read and hence i can manipulate
      // and remove the devil from the picture i.e '\n' ! ;)

      ssize_t strlen = ACE_OS::read (ACE_STDIN,
				     buf,
				     sizeof buf);
      if (buf[strlen -1] == '\n')
	buf[strlen -1] = '\0';

      switch (tolower (buf[0]))
	{
	case 'r':
	  {
	    register_consumer ();
	    break;
	  }
	case 'u':
	  {
	    unregister_consumer ();
	    break;
	  }
        case 'q':
	  {
	    quit_consumer_process ();
            break;
	  }
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Input_Handler::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


int
Consumer_Input_Handler::register_consumer ()
{

  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      // Get the stockname the consumer is interested in.
      char stockname[BUFSIZ];

      ACE_DEBUG ((LM_DEBUG,
		  "Stockname?"));

      ssize_t strlen = ACE_OS::read (ACE_STDIN,
				     stockname,
				     sizeof stockname);

      if (stockname[strlen -1] == '\n')
	stockname[strlen -1] = '\0';

      this->consumer_handler_->stock_name_ = stockname;

      // Get the threshold value.
      char needed_stock_value[BUFSIZ];
      ACE_DEBUG ((LM_DEBUG,
		  "Threshold Stock value?"));

      strlen = ACE_OS::read (ACE_STDIN,
			     needed_stock_value,
			     sizeof needed_stock_value);

      if (needed_stock_value[strlen -1] == '\n')
	needed_stock_value[strlen -1] = '\0';

      this->consumer_handler_->threshold_value_ =
	ACE_OS::atoi (needed_stock_value);

      // Register with the server.
      this->consumer_handler_->server_->register_callback (this->consumer_handler_->stock_name_,
							   this->consumer_handler_->threshold_value_,
							   this->consumer_handler_->consumer_var_.in (),
							   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Note the registration.
      consumer_handler_->registered_ = 1;
      consumer_handler_->unregistered_ = 0;

      ACE_DEBUG ((LM_DEBUG,
		  "registeration done!\n"));
    }

  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Consumer_Input_Handler::register_consumer()\n");
      return -1;
    }
  TAO_ENDTRY;

return 0;
}



int
Consumer_Input_Handler::unregister_consumer ()
{
  // Only if the consumer is registered can the
  // unregistration take place.

  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      if (consumer_handler_->registered_ == 1)
	{
	  this->consumer_handler_->server_->unregister_callback (this->consumer_handler_->consumer_var_.in());

	  TAO_CHECK_ENV;

	  ACE_DEBUG ((LM_DEBUG,
		      " Consumer Unregistered \n "));
	  consumer_handler_->unregistered_ = 1;
	  consumer_handler_->registered_ = 0;
	}
      else
	ACE_DEBUG ((LM_DEBUG,
		    " Invalid Operation: Consumer not Registered\n"));

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Consumer_Input_Handler::unregister_consumer()");
      return -1;
    }
  TAO_ENDTRY;

return 0;
}

int
Consumer_Input_Handler::quit_consumer_process ()
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
	  consumer_handler_->unregistered_ = 0;
          consumer_handler_->registered_ = 0;
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
