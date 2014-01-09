//
// $Id$
//

#include "Manager_Handler.h"

Manager_Handler::Manager_Handler (Test::Manager_ptr manager,
                                  Test::Controller_ptr controller)
  : manager_ (Test::Manager::_duplicate (manager))
  , controller_ (Test::Controller::_duplicate (controller))
{
}

void
Manager_Handler::start_workers (void)
{
  try
    {
      this->manager_->start_workers (CORBA::Short(10),
                                     CORBA::Long(1000),
                                     this->controller_.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised in nested start workers: ");
    }
}

void
Manager_Handler::start_workers_excep (::Messaging::ExceptionHolder * holder)
{
  try
    {
      holder->raise_exception ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised in asynch start workers: ");
    }
}

