// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/CosEvent/CEC_Dispatching_Task.h"

#include "tao/ORB_Constants.h"
#include "ace/OS_NS_errno.h"


#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_Dispatching_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_errno.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_CEC_Dispatching_Task::svc (void)
{
  bool done = false;

  while (!done)
    {
      try
        {
          ACE_Message_Block *mb = 0;

          if (this->getq (mb) == -1)
            {
              if (ACE_OS::last_error () == ESHUTDOWN)
                {
                  return 0;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              "EC (%P|%t) getq error in Dispatching Queue\n"));
                }
            }

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
            {
              done = true;
            }
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("EC (%P|%t) exception in dispatching queue");
        }
    }

  return 0;
}

void
TAO_CEC_Dispatching_Task::push (TAO_CEC_ProxyPushSupplier *proxy,
                                CORBA::Any& event)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  void* buf = this->allocator_->malloc (sizeof (TAO_CEC_Push_Command));

  if (buf == 0)
    throw CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_NO);

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
                                  TAO_CEC_TypedEvent& typed_event)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  void* buf = this->allocator_->malloc (sizeof (TAO_CEC_Invoke_Command));

  if (buf == 0)
    throw CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_NO);

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
  this->proxy_->push_to_consumer (this->event_);
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
  this->proxy_->invoke_to_consumer (this->typed_event_);
  return 0;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_END_VERSIONED_NAMESPACE_DECL
