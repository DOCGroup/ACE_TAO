// $Id$

#include "orbsvcs/CosEvent/CEC_Dispatching_Task.h"

#include "tao/ORB_Constants.h"
#include "ace/OS_NS_errno.h"


#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Dispatching_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_errno.h"

ACE_RCSID (CosEvent,
           CEC_Dispatching,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
            dynamic_cast<TAO_CEC_Dispatch_Command*> (mb);

          if (command == 0)
            {
              ACE_Message_Block::release (mb);
              continue;
            }

          int result = command->execute ();

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
                                ACE_ENV_ARG_DECL)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  void* buf = this->allocator_->malloc (sizeof (TAO_CEC_Push_Command));

  if (buf == 0)
    ACE_THROW (CORBA::NO_MEMORY (TAO::VMCID,
                                 CORBA::COMPLETED_NO));

  ACE_Message_Block *mb =
    new (buf) TAO_CEC_Push_Command (proxy,
                                    event,
                                    this->data_block_.duplicate (),
                                    this->allocator_);
  this->putq (mb);
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_Dispatching_Task::invoke (TAO_CEC_ProxyPushSupplier *proxy,
                                  TAO_CEC_TypedEvent& typed_event
                                  ACE_ENV_ARG_DECL)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  void* buf = this->allocator_->malloc (sizeof (TAO_CEC_Invoke_Command));

  if (buf == 0)
    ACE_THROW (CORBA::NO_MEMORY (TAO::VMCID,
                                 CORBA::COMPLETED_NO));

  ACE_Message_Block *mb =
    new (buf) TAO_CEC_Invoke_Command (proxy,
                                      typed_event,
                                      this->data_block_.duplicate (),
                                      this->allocator_);
  this->putq (mb);
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

// ****************************************************************

TAO_CEC_Dispatch_Command::~TAO_CEC_Dispatch_Command (void)
{
}

// ****************************************************************

int
TAO_CEC_Shutdown_Task_Command::execute (void)
{
  return -1;
}

// ****************************************************************

TAO_CEC_Push_Command::~TAO_CEC_Push_Command (void)
{
  this->proxy_->_decr_refcnt ();
}

int
TAO_CEC_Push_Command::execute (void)
{
  this->proxy_->push_to_consumer (this->event_ ACE_ENV_ARG_PARAMETER);
  return 0;
}

// ****************************************************************

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_Invoke_Command::~TAO_CEC_Invoke_Command (void)
{
  this->proxy_->_decr_refcnt ();
}

int
TAO_CEC_Invoke_Command::execute (void)
{
  this->proxy_->invoke_to_consumer (this->typed_event_ ACE_ENV_ARG_PARAMETER);
  return 0;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_END_VERSIONED_NAMESPACE_DECL
