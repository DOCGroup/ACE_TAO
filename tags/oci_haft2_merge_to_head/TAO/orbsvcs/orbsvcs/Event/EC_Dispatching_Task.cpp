// $Id$

#include "EC_Dispatching_Task.h"
#include "EC_ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Dispatching_Task.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Dispatching, "$Id$")

int
TAO_EC_Queue::is_full_i (void)
{
  return ACE_static_cast (size_t, this->cur_count_) > this->high_water_mark_;
}

// ****************************************************************

int
TAO_EC_Dispatching_Task::svc (void)
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

          TAO_EC_Dispatch_Command *command =
            ACE_dynamic_cast(TAO_EC_Dispatch_Command*, mb);

          if (command == 0)
            {
              ACE_Message_Block::release (mb);
              continue;
            }

          int result = command->execute (ACE_ENV_SINGLE_ARG_PARAMETER);
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
TAO_EC_Dispatching_Task::push (TAO_EC_ProxyPushSupplier *proxy,
                               RtecEventComm::PushConsumer_ptr consumer,
                               RtecEventComm::EventSet& event
                               ACE_ENV_ARG_DECL)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  void* buf = this->allocator_->malloc (sizeof (TAO_EC_Push_Command));

  if (buf == 0)
    ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                 CORBA::COMPLETED_NO));

  ACE_Message_Block *mb =
    new (buf) TAO_EC_Push_Command (proxy,
                                   consumer,
                                   event,
                                   this->data_block_.duplicate (),
                                   this->allocator_);
  this->putq (mb);
}

// ****************************************************************

TAO_EC_Dispatch_Command::~TAO_EC_Dispatch_Command (void)
{
}

// ****************************************************************

int
TAO_EC_Shutdown_Task_Command::execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return -1;
}

// ****************************************************************

TAO_EC_Push_Command::~TAO_EC_Push_Command (void)
{
  this->proxy_->_decr_refcnt ();
}

int
TAO_EC_Push_Command::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_->push_to_consumer (this->consumer_.in (),
                                  this->event_
                                   ACE_ENV_ARG_PARAMETER);
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Locked_Data_Block<ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Locked_Data_Block<ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
