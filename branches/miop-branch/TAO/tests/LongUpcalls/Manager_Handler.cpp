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
Manager_Handler::start_workers (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->manager_->start_workers (CORBA::Short(10),
                                     CORBA::Long(1000),
                                     this->controller_.in ()
                                     TAO_ENV_ARG_PARAMETER);
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
Manager_Handler::start_workers_excep (Test::AMI_ManagerExceptionHolder * holder
                                      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_start_workers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised in asynch start workers: ");
    }
  ACE_ENDTRY;
}

