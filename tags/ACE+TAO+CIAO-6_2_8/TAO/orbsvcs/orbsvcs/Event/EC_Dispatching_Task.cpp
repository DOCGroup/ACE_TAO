// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Event/EC_Dispatching_Task.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_Defaults.h"

#include "tao/ORB_Constants.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_strings.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Dispatching_Task.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Simple_Queue_Full_Action::TAO_EC_Simple_Queue_Full_Action (void)
  : queue_full_action_return_value_ (WAIT_TO_EMPTY)
{
}

/// Helper function to register the default action into the service
/// configurator.
int
TAO_EC_Simple_Queue_Full_Action::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_EC_Simple_Queue_Full_Action);
}

int
TAO_EC_Simple_Queue_Full_Action::init (int argc, ACE_TCHAR* argv[])
{
  // Here we look at the args and set an internal flag indicating whether
  // the default action should be to wait for the queue to not be full
  // or whether it should be to silently discard the event.

  // @@ This should use the arg shifter stuff, but let's keep it simple to
  // start.

  do {
    if (argc == 0)
      break;

    if (ACE_OS::strcasecmp (ACE_TEXT("wait"), argv[0]) == 0)
      this->queue_full_action_return_value_ = WAIT_TO_EMPTY;
    else if (ACE_OS::strcasecmp (ACE_TEXT("discard"), argv[0]) == 0)
      this->queue_full_action_return_value_ = SILENTLY_DISCARD;
#if 0
    else
      ;
    // probably ought to print an error message here
#endif
  } while (0);

  return 0;
}

int
TAO_EC_Simple_Queue_Full_Action::fini (void)
{
  return 0;
}

int
TAO_EC_Simple_Queue_Full_Action::queue_full_action (TAO_EC_Dispatching_Task * /*task*/,
                                                    TAO_EC_ProxyPushSupplier * /*proxy*/,
                                                    RtecEventComm::PushConsumer_ptr /*consumer*/,
                                                    RtecEventComm::EventSet& /*event*/)
{
  return this->queue_full_action_return_value_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_EC_Simple_Queue_Full_Action,
                       TAO_EC_DEFAULT_QUEUE_FULL_SERVICE_OBJECT_NAME,
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_Simple_Queue_Full_Action),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTEvent_Serv, TAO_EC_Simple_Queue_Full_Action)


TAO_BEGIN_VERSIONED_NAMESPACE_DECL
bool
TAO_EC_Queue::is_full_i (void)
{
  return static_cast<size_t> (this->cur_count_) > this->high_water_mark_;
}

// ****************************************************************

int
TAO_EC_Dispatching_Task::svc (void)
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

          TAO_EC_Dispatch_Command *command =
            dynamic_cast<TAO_EC_Dispatch_Command*> (mb);

          if (command == 0)
            {
              ACE_Message_Block::release (mb);
              continue;
            }

          int const result = command->execute ();

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
TAO_EC_Dispatching_Task::push (TAO_EC_ProxyPushSupplier *proxy,
                               RtecEventComm::PushConsumer_ptr consumer,
                               RtecEventComm::EventSet& event)
{
  if (this->msg_queue()->is_full ())
    {
      if (0 != this->queue_full_service_object_)
        {
          int action =
            this->queue_full_service_object_->queue_full_action (this, proxy,
                                                                 consumer, event);

          if (action == TAO_EC_Queue_Full_Service_Object::SILENTLY_DISCARD)
            return;
          // if action == WAIT_TO_EMPTY then we just go ahead and queue it
        }
        // else go ahead and queue it
    }

  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  void* buf = this->allocator_->malloc (sizeof (TAO_EC_Push_Command));

  if (buf == 0)
    throw CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_NO);

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
TAO_EC_Shutdown_Task_Command::execute (void)
{
  return -1;
}

// ****************************************************************

TAO_EC_Push_Command::~TAO_EC_Push_Command (void)
{
  this->proxy_->_decr_refcnt ();
}

int
TAO_EC_Push_Command::execute (void)
{
  this->proxy_->push_to_consumer (this->consumer_.in (),
                                  this->event_);
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
