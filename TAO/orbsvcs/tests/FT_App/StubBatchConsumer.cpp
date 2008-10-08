// -*- C++ -*-
//
// $Id$

#include "StubBatchConsumer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"

StubBatchConsumer::StubBatchConsumer ()
  : quit_(0)
{
}


StubBatchConsumer::~StubBatchConsumer ()
{
}


int StubBatchConsumer::parse_args (int argc, ACE_TCHAR * argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  int optionError = 0;
  // No options for now
  return optionError;
}


::PortableServer::POA_ptr StubBatchConsumer::_default_POA (void)
{
  return ::PortableServer::POA::_duplicate(this->poa_.in ());
}

PortableServer::ObjectId StubBatchConsumer::objectId()const
{
  return this->object_id_.in();
}


/**
 * register this object
 */
int StubBatchConsumer::init (CORBA::ORB_ptr orb, ::FT::FaultNotifier_var & notifier)
{
  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->notifier_ = notifier;
  this->identity_ = "StubBatchConsumer";


  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA);

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);

  // Get the POA .
  this->poa_ = PortableServer::POA::_narrow (poa_object.in ());


  if (CORBA::is_nil(this->poa_.in ()))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();

  // Register with the POA.

  this->object_id_ = this->poa_->activate_object (this);

  // find my identity as an object

  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (object_id_.in ());

  CosNotifyFilter::Filter_var filter = CosNotifyFilter::Filter::_nil();

  this->consumer_id_ = notifier->connect_sequence_fault_consumer(
    CosNotifyComm::SequencePushConsumer::_narrow(this_obj.in ()),
    filter.in ());

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
void StubBatchConsumer::fini (void)
{
  this->notifier_->disconnect_consumer(this->consumer_id_);
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
  )
{
  ACE_UNUSED_ARG (notifications);
  //@@ sequence of structured event handling not written yet
}

void StubBatchConsumer::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  )
{
  ACE_UNUSED_ARG (added);
  ACE_UNUSED_ARG (removed);
  ACE_ERROR ((LM_ERROR,
    "StubBatchConsumer: offer_change call ignored.\n"
  ));
  // @@ not implemented (not likely to be needed)
}

//virtual
void StubBatchConsumer::disconnect_sequence_push_consumer (
  )
{
  ACE_ERROR ((LM_ERROR,
    "StubBatchConsumer:disconnect_sequence_push_consumer interpreted as quit request.\n"
  ));
  this->quit_ = 1;
}
