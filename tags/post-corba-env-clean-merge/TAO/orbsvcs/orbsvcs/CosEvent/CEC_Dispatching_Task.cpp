// $Id$

#include "CEC_Dispatching_Task.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_Dispatching_Task.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_Dispatching, "$Id$")

int
TAO_CEC_Dispatching_Task::svc (void)
{
  int done = 0;
  while (!done)
    {
      ACE_TRY_NEW_ENV
        {
          ACE_Message_Block *mb;
          if (this->getq (mb) == -1)
            if (ACE_OS::last_error () == ESHUTDOWN)
              return 0;
          else
            ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) getq error in Dispatching Queue\n"));

          TAO_CEC_Dispatch_Command *command =
            ACE_dynamic_cast(TAO_CEC_Dispatch_Command*, mb);

          if (command == 0)
            {
              ACE_Message_Block::release (mb);
              continue;
            }

          int result = command->execute (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_Message_Block::release (mb);

          if (result == -1)
            done = 1;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "EC (%P|%t) exception in dispatching queue");
        }
      ACE_ENDTRY;
    }
  return 0;
}

void
TAO_CEC_Dispatching_Task::push (TAO_CEC_ProxyPushSupplier *proxy,
                                CORBA::Any& event
                                TAO_ENV_ARG_DECL)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  void* buf = this->allocator_->malloc (sizeof (TAO_CEC_Push_Command));

  if (buf == 0)
    ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                 CORBA::COMPLETED_NO));

  ACE_Message_Block *mb =
    new (buf) TAO_CEC_Push_Command (proxy,
                                    event,
                                    this->data_block_.duplicate (),
                                    this->allocator_);
  this->putq (mb);
}

// ****************************************************************

TAO_CEC_Dispatch_Command::~TAO_CEC_Dispatch_Command (void)
{
}

// ****************************************************************

int
TAO_CEC_Shutdown_Task_Command::execute (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return -1;
}

// ****************************************************************

TAO_CEC_Push_Command::~TAO_CEC_Push_Command (void)
{
  this->proxy_->_decr_refcnt ();
}

int
TAO_CEC_Push_Command::execute (TAO_ENV_SINGLE_ARG_DECL)
{
  this->proxy_->push_to_consumer (this->event_ TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Locked_Data_Block<ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Locked_Data_Block<ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
