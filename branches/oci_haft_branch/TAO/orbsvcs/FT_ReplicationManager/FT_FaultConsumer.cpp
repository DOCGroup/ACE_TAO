/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_FaultConsumer.cpp
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *  This file provides the implementation of the TAO::FT_FaultConsumer
 *  class.  The TAO::FT_FaultConsumer connects to the FaultNotifier to
 *  receive fault reports.  It interacts with the ReplicationManager
 *  to process fault reports (e.g., to set a new primary on an object
 *  group or to create a new member of an object group).
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================

#include "ace/pre.h"

#include "orbsvcs/FT_ReplicationManager/FT_FaultConsumer.h"
#include "orbsvcs/FT_ReplicationManagerC.h"

ACE_RCSID (FT_FaultConsumer,
           FT_FaultConsumer,
           "$Id$")

/// Default constructor.
TAO::FT_FaultConsumer::FT_FaultConsumer ()
  : poa_ (PortableServer::POA::_nil ())
  , fault_notifier_ (FT::FaultNotifier::_nil ())
  , replication_manager_ (FT::ReplicationManager::_nil ())
  , consumer_id_ (0)
  , notifications_ (0)
{
}

/// Destructor.
TAO::FT_FaultConsumer::~FT_FaultConsumer ()
{
}

/**
* Connect to the FT::FaultNotifier.
* Note: We make the following assumptions about what the
* application will do:
* - Create an instance of this consumer class.
* - Obtain the object reference of the FaultNotifier to which this
*   consumer should connect.
* - Call this init() method, passing it the POA with which we
*   have been activated, the FaultNotifier, and ReplicationManager
*   object references.
*/
int TAO::FT_FaultConsumer::init (
  PortableServer::POA_ptr poa,
  FT::FaultNotifier_ptr fault_notifier,
  FT::ReplicationManager_ptr replication_manager
  ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (poa));
  ACE_ASSERT (!CORBA::is_nil (fault_notifier));
  ACE_ASSERT (!CORBA::is_nil (replication_manager));

  // Duplicate the object references passed in.
  this->poa_ = 
    PortableServer::POA::_duplicate (poa);
  this->fault_notifier_ = 
    FT::FaultNotifier::_duplicate (fault_notifier);
  this->replication_manager_ = 
    FT::ReplicationManager::_duplicate (replication_manager);

  //@@ Should this init() method activate the consumer in the POA, or
  // should the application do that?
  // I don't think this object can activate itself because it doesn't
  // know the policies on the POA.  So, we assume the application has
  // already activated us.
  //@@ For now, let's try just activating it in the POA.

  // Activate this consumer in the POA.
  PortableServer::ObjectId_var oid = 
    this->poa_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  CORBA::Object_var obj = 
    this->poa_->id_to_reference (oid.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow it to CosNotifyComm::StructuredPushConsumer.
  CosNotifyComm::StructuredPushConsumer_var consumer = 
    CosNotifyComm::StructuredPushConsumer::_nil ();
  consumer = CosNotifyComm::StructuredPushConsumer::_narrow (
    obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Subscribe to the FaultNotifier.
  CosNotifyFilter::Filter_var filter = CosNotifyFilter::Filter::_nil ();
  this->consumer_id_ = fault_notifier_->connect_structured_fault_consumer (
    consumer.in(), filter ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Success.
  return 0;
}

/**
* Clean house for process shut down.
* - Disconnect from FT::FaultNotifier.
* - Deactivate from the POA.
*/
int TAO::FT_FaultConsumer::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  // Disconnect from the FaultNotifier.
  this->fault_notifier_->disconnect_consumer (this->consumer_id_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //@@ Should this fini() method deactivate the consumer from the POA, or
  // should the application do that?
  //
  // Deactivate ourselves from the POA.
  PortableServer::ObjectId_var oid = 
    this->poa_->servant_to_id (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->poa_->deactivate_object (oid.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
 
  // Success.
  return 0;
}


////////////////
// CORBA methods

void TAO::FT_FaultConsumer::push_structured_event (
  const CosNotification::StructuredEvent &event
  ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosEventComm::Disconnected))
{
  this->notifications_ += 1;
  ACE_DEBUG ((LM_DEBUG,
    "TAO::FT_FaultConsumer: Received Fault notification(%d):\n"
    "TAO::FT_FaultConsumer:   Header EventType domain: %s\n"
    "TAO::FT_FaultConsumer:   Header EventType type: %s\n"
    "TAO::FT_FaultConsumer:   Header EventName: %s\n",
    ACE_static_cast (unsigned int, this->notifications_),
    ACE_static_cast (const char *, event.header.fixed_header.event_type.domain_name),
    ACE_static_cast (const char *, event.header.fixed_header.event_type.type_name),
    ACE_static_cast (const char *, event.header.fixed_header.event_name)
    ));

  const CosNotification::FilterableEventBody & filterable = 
    event.filterable_data;

  size_t propertyCount = filterable.length ();
  for (size_t nProp = 0; nProp < propertyCount; ++nProp)
  {
    const CosNotification::Property & property = filterable[nProp];
    ACE_DEBUG ((LM_DEBUG,
      "TAO::FT_FaultConsumer:   Property Name: %s\n",
      ACE_static_cast (const char *, property.name)
      ));
  }

  //TODO: Finish the implementation of push_structured_event() to:
  // - analyze the fault
  // - if it was a primary fault, set a new primary replica
  // - if the number of replicas falls below MinimumNumberReplicas
  //   property and membership style is Infrastructure-Controlled,
  //   create and add a new member.

}

void TAO::FT_FaultConsumer::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosNotifyComm::InvalidEventType))
{
  ACE_DEBUG ((LM_DEBUG, 
    "TAO::FT_FaultConsumer::offer_change() call ignored.\n"
    ));
}

void TAO::FT_FaultConsumer::disconnect_structured_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //TODO: For now, we are just ignoring the disconnect callback.
  ACE_DEBUG ((LM_DEBUG, 
    "TAO::FT_FaultConsumer::disconnect_structured_push_consumer() call ignored.\n"
    ));
}

#include "ace/post.h"

