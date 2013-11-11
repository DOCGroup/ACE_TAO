//=============================================================================
/**
 *  @file    Consumer_Input_Handler.cpp
 *
 *  $Id$
 *
 *  Implementation of the Consumer_Input_Handler class.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "Consumer_Input_Handler.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_ctype.h"

Consumer_Input_Handler::Consumer_Input_Handler (Consumer_Handler *consumer_handler)
  {
    consumer_handler_ = consumer_handler;

  }

int
Consumer_Input_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  // The string could read contains \n\0 hence using ACE_OS::read
  // which returns the no of bytes read and hence i can manipulate
  // and remove the devil from the picture i.e '\n' ! ;)

  ssize_t strlen = ACE_OS::read (ACE_STDIN,
                                 buf,
                                 sizeof buf);
  if (buf[strlen -1] == '\n')
    buf[strlen -1] = '\0';

  switch (ACE_OS::ace_tolower (buf[0]))
    {
    case Consumer_Input_Handler::REGISTER:
      {
        this->register_consumer ();
        break;
      }
    case Consumer_Input_Handler::UNREGISTER:
      {
        this->unregister_consumer ();
        break;
      }
    case Consumer_Input_Handler::EXIT:
      {
        this->quit_consumer_process ();
        break;
      }
    }
  return 0;
}


int
Consumer_Input_Handler::register_consumer ()
{

  // Get the stockname the consumer is interested in.
  static char stockname[BUFSIZ];

  ACE_DEBUG ((LM_DEBUG,
              "Stockname?"));

  ssize_t strlen = ACE_OS::read (ACE_STDIN,
                                 stockname,
                                 sizeof stockname - 1);

  // Taking care of platforms where an carriage return is padded with newline.
  if (stockname[strlen -2] == '\n' || stockname[strlen -2] == '\r')
    stockname[strlen -2] = '\0';
  else
    if (stockname[strlen -1] == '\n' || stockname[strlen -1] == '\r')
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


  try
    {

      // Register with the server.
      this->consumer_handler_->server_->register_callback (this->consumer_handler_->stock_name_.c_str (),
                                                           this->consumer_handler_->threshold_value_,
                                                           this->consumer_handler_->consumer_var_.in ());

      // Note the registration.
      consumer_handler_->registered_ = 1;
      consumer_handler_->unregistered_ = 0;

      // @@ Up to this point..

      ACE_DEBUG ((LM_DEBUG,
                  "registeration done!\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Consumer_Input_Handler::register_consumer()\n");
      return -1;
    }

return 0;
}



int
Consumer_Input_Handler::unregister_consumer ()
{
  // Only if the consumer is registered can the
  // unregistration take place.

  if (consumer_handler_->registered_ == 1)
    {
      this->consumer_handler_->server_->unregister_callback (this->consumer_handler_->consumer_var_.in());

      ACE_DEBUG ((LM_DEBUG,
                  " Consumer Unregistered \n"));
      consumer_handler_->unregistered_ = 1;
      consumer_handler_->registered_ = 0;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                " Invalid Operation: Consumer not Registered\n"));


  return 0;
}

int
Consumer_Input_Handler::quit_consumer_process ()
{
  // Only if the consumer is registered and wants to shut
  // down, its necessary to unregister and then shutdown.


  try
    {
      if (consumer_handler_->unregistered_ != 1 && consumer_handler_->registered_ == 1)
        {
          // If the notifier has exited and the consumer tries to call
          // the unregister_callback method tehn an execption will be
          // raised. Hence check for this case using.
          this->consumer_handler_->server_->unregister_callback (this->consumer_handler_->consumer_var_.in ());

          ACE_DEBUG ((LM_DEBUG,
                      " Consumer Unregistered \n"));
          consumer_handler_->unregistered_ = 0;
          consumer_handler_->registered_ = 0;
        }
      this->consumer_handler_->consumer_servant_->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      // There would be an exception only if there is a communication
      // failure between the notifier and consumer. On catching the
      // exception proclaim the problem and do a graceful exit.
      ex._tao_print_exception ("Communication failed!\n");

      try
        {
          this->consumer_handler_->consumer_servant_->shutdown ();
        }
      catch (const CORBA::Exception&)
        {
        }

      return -1;
    }

  return 0;
}

Consumer_Input_Handler::~Consumer_Input_Handler (void)
{
  // No-op
}
