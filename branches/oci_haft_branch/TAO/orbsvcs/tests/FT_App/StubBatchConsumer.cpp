// -*- C++ -*-
//
// $Id$

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


::PortableServer::POA_ptr StubBatchConsumer::_default_POA (ACE_ENV_SINGLE_ARG_DECL)
{
  return ::PortableServer::POA::_duplicate(this->poa_ ACE_ENV_ARG_PARAMETER);
}

PortableServer::ObjectId StubBatchConsumer::objectId()const
{
  return this->objectId_.in();
}


/**
 * register this object
 */
int StubBatchConsumer::init (CORBA::ORB_var & orb, ::FT::FaultNotifier_var & notifier ACE_ENV_ARG_DECL)
{
  int result = 0;
  this->orb_ = orb;

  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);

  // Get the POA object.
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ()
                                  ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(this->poa_))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // Register with the POA.

  this->objectId_ = this->poa_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  this->notifier_ = notifier;

  this->identity_ = "StubBatchConsumer";

  CosNotifyFilter::Filter_var filter = CosNotifyFilter::Filter::_nil();

  this->consumerId_ = notifier->connect_sequence_fault_consumer(
    _this(),
    filter);

  return result;
}

/**
 * Return a string to identify this object for logging/console message purposes.
 */
const char * StubBatchConsumer::identity () const
{
  return this->identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
void StubBatchConsumer::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  this->notifier_->disconnect_consumer(this->consumerId_ ACE_ENV_ARG_PARAMETER);
}


int StubBatchConsumer::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  return this->quit_;
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
  this->quit_ = 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
//  template instantiate ACE_Vector < const char * >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
//# pragma instantiate ACE_Vector < const char * >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

