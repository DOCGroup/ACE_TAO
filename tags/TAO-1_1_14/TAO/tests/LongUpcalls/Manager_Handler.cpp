//
// $Id$
//

#include "Manager_Handler.h"

ACE_RCSID(LongUpcalls, Manager_Handler, "$Id$")

Manager_Handler::Manager_Handler (Test::Manager_ptr manager,
                                  Test::Controller_ptr controller)
  : manager_ (Test::Manager::_duplicate (manager))
  , controller_ (Test::Controller::_duplicate (controller))
{
}

void
Manager_Handler::start_workers (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->manager_->start_workers (CORBA::Short(10),
                                     CORBA::Long(1000),
                                     this->controller_.in (),
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised in nested start workers: ");
    }
  ACE_ENDTRY;
}

void
Manager_Handler::start_workers_excep (Test::AMI_ManagerExceptionHolder * holder,
                                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_start_workers (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised in asynch start workers: ");
    }
  ACE_ENDTRY;
}

