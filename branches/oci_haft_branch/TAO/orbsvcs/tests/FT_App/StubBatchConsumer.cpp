// -*- C++ -*-
//
// $Id$

#include <ace/pre.h>
#include "StubBatchConsumer.h"
#include <ace/Get_Opt.h>
#include <tao/PortableServer/ORB_Manager.h>
#include <orbsvcs/orbsvcs/PortableGroup/PG_Properties_Encoder.h>

StubBatchConsumer::StubBatchConsumer ()
  : quit_(0)
{
}


StubBatchConsumer::~StubBatchConsumer ()
{
}


int StubBatchConsumer::parse_args (int argc, char * argv[])
{
  int optionError = 0;
  // No options for now
  return optionError;
}

/**
 * register this object
 */
int StubBatchConsumer::self_register (TAO_ORB_Manager & orbManager,
    ::FT::FaultNotifier_var & notifier
    ACE_ENV_ARG_DECL)
{
  int result = 0;
  orb_ = orbManager.orb();

  notifier_ = notifier;

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  identity_ = "StubBatchConsumer";

  CosNotifyFilter::Filter_var filter = CosNotifyFilter::Filter::_nil();

  consumerId_ = notifier->connect_sequence_fault_consumer(
    _this(),
    filter);
                                                 
  return result;
}

/**
 * Return a string to identify this object for logging/console message purposes.
 */
const char * StubBatchConsumer::identity () const
{
  return identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
void StubBatchConsumer::self_unregister (ACE_ENV_SINGLE_ARG_DECL)
{
  notifier_->disconnect_consumer(consumerId_ ACE_ENV_ARG_PARAMETER);
}


int StubBatchConsumer::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  return quit_;
}

////////////////
// CORBA methods


//virtual 
void StubBatchConsumer::push_structured_events (
    const CosNotification::EventBatch & notifications
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosEventComm::Disconnected
  ))
{
  int a_big_todo_here;
}

void StubBatchConsumer::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL
  )
  throw (CORBA::SystemException, CosNotifyComm::InvalidEventType)
{
  ACE_ERROR ((LM_ERROR,
    "StubBatchConsumer: offer_change call ignored.\n"
  ));
  int todo;
}     

//virtual 
void StubBatchConsumer::disconnect_sequence_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_ERROR ((LM_ERROR,
    "StubBatchConsumer:disconnect_sequence_push_consumer interpreted as quit request.\n"
  ));
  quit_ = 1;
}           

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
//  template instantiate ACE_Vector < const char * >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
//# pragma instantiate ACE_Vector < const char * >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#include "ace/post.h"
