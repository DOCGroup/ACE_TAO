// $Id$

#include "EC_Dispatching_Task.h"
#include "EC_ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Dispatching_Task.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Dispatching, "$Id$")

int
TAO_EC_Dispatching_Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      while (1)
        {
          ACE_Message_Block *mb;
          if (this->getq (mb) == -1)
            if (ACE_OS::last_error () == ESHUTDOWN)
              return 0;
          else
            ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) getq error in Dispatching Queue\n"));

          TAO_EC_Dispatch_Command *command =
            ACE_dynamic_cast(TAO_EC_Dispatch_Command*, mb);

          if (command == 0)
            {
              ACE_Message_Block::release (mb);
              continue;
            }

          int result = command->execute (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_Message_Block::release (mb);

          if (result == -1)
            break;
        }
      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "EC (%P|%t) exception in dispatching queue");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

TAO_EC_Dispatch_Command::~TAO_EC_Dispatch_Command (void)
{
}

// ****************************************************************

int
TAO_EC_Shutdown_Command::execute (CORBA::Environment&)
{
  return -1;
}

// ****************************************************************

int
TAO_EC_Push_Command::execute (CORBA::Environment& ACE_TRY_ENV)
{
  this->proxy_->push_to_consumer (this->event_, ACE_TRY_ENV);
  return 0;
}
