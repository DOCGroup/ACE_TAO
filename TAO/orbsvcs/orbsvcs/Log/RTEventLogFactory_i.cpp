#include "orbsvcs/Log/RTEventLogFactory_i.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/RTEventLogNotification.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/ESF/ESF_Peer_Workers.h"
#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "ace/Auto_Ptr.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Log,
           RTEventLogFactory_i,
           "$Id$")



TAO_RTEventLogFactory_i::TAO_RTEventLogFactory_i (void)
: poa_ (PortableServer::POA::_nil ()),
naming_ (CosNaming::NamingContext::_nil ())
{
  // No-Op.
}

TAO_RTEventLogFactory_i::~TAO_RTEventLogFactory_i()
{
  // No-Op.
}

int
TAO_RTEventLogFactory_i::init (PortableServer::POA_ptr poa,
                               const char* child_poa_name,
                               CosNaming::NamingContext_ptr naming
                               ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (poa))
    return -1;

  this->naming_ = CosNaming::NamingContext::_duplicate (naming);
  // Save the naming context.

  // Create a UNIQUE_ID and USER_ID policy because we want the POA
  // to detect duplicates for us.
  PortableServer::IdUniquenessPolicy_var idpolicy =
    poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::IdAssignmentPolicy_var assignpolicy =
    poa->create_id_assignment_policy (PortableServer::USER_ID
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create a PolicyList
  CORBA::PolicyList policy_list;
  policy_list.length (0);
// TODO : Problem with these policies.
/*  policy_list [0] =
    PortableServer::IdUniquenessPolicy::_duplicate (idpolicy.in ());
  policy_list [1] =
    PortableServer::IdAssignmentPolicy::_duplicate (assignpolicy.in ());
*/
  PortableServer::POAManager_ptr manager =
    poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  // @@ Pradeep : TODO - find a way to destroy the policy_list if we return here.

  // Create the child POA.
  this->poa_ = poa->create_POA (child_poa_name,
                                manager,
                                policy_list
                                ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  idpolicy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  assignpolicy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //this->poa_ =  PortableServer::POA::_duplicate (poa);
  // uncomment this if we want to use the parent poa for some reason.

  PortableServer::POA_var defPOA = this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  TAO_EC_Event_Channel_Attributes attr (defPOA.in (), defPOA.in ());

  this->impl = 0;
  ACE_NEW_THROW_EX (impl,
                    TAO_EC_Event_Channel (attr),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);

  auto_ptr <TAO_EC_Event_Channel> ec (impl);

  impl->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::ObjectId_var oidec = poa_->activate_object (ec.get ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ec.release ();

  CORBA::Object_var objec =
    poa_->id_to_reference (oidec.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->event_channel_ = RtecEventChannelAdmin::EventChannel::_narrow (objec.in ());


  this->consumer_admin_ = this->event_channel_->for_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_NEW_THROW_EX (this->notifier_, 
                    TAO_RTEventLogNotification(this->event_channel_.in ()),
                    CORBA::NO_MEMORY ());
  return 0;
}

RTEventLogAdmin::EventLogFactory_ptr
TAO_RTEventLogFactory_i::activate (PortableServer::POA_ptr poa
                                   ACE_ENV_ARG_DECL)
{
  RTEventLogAdmin::EventLogFactory_var v_return;

  PortableServer::ObjectId_var oid =
    poa->activate_object (this
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (v_return._retn ());

  CORBA::Object_var obj =
    poa->id_to_reference (oid.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (v_return._retn ());

  // narrow and store the result..
  this->log_mgr_ =
    DsLogAdmin::LogMgr::_narrow (obj.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (v_return._retn ());

  v_return =
    RTEventLogAdmin::EventLogFactory::_narrow (obj.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (RTEventLogAdmin::EventLogFactory::_nil ());

  return v_return._retn ();
}

RTEventLogAdmin::EventLog_ptr 
TAO_RTEventLogFactory_i::create (
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_rec_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds,
        DsLogAdmin::LogId_out id
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::InvalidLogFullAction,
        DsLogAdmin::InvalidThreshold
      ))
{
  // Get an id for this Log.
  this->max_id_++;

  RTEventLogAdmin::EventLog_ptr eventlog =
    this->create_with_id (this->max_id_,
                          full_action,
                          max_rec_size,
                          thresholds
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (RTEventLogAdmin::EventLog::_nil ());

  // Set the id to return..
  id = this->max_id_;

  // Store the id in the LogIdList.
  CORBA::ULong len = logid_list_.length();
  logid_list_.length(len+1);
  logid_list_[len] = id;

  return eventlog;
}

RTEventLogAdmin::EventLog_ptr 
TAO_RTEventLogFactory_i::create_with_id (
        DsLogAdmin::LogId id,
        DsLogAdmin::LogFullActionType full_action,
        CORBA::ULongLong max_size,
        const DsLogAdmin::CapacityAlarmThresholdList & thresholds
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        DsLogAdmin::LogIdAlreadyExists,
        DsLogAdmin::InvalidLogFullAction,
        DsLogAdmin::InvalidThreshold
      ))
{
  ACE_UNUSED_ARG (thresholds);

  // Make sure the id not used up.
  if (hash_map_.find (id) == 0)
    ACE_THROW_RETURN (DsLogAdmin::LogIdAlreadyExists (),
                      RTEventLogAdmin::EventLog::_nil ());

  RTEventLogAdmin::EventLog_var event_log;
  // Object to return.

  TAO_RTEventLog_i* event_log_i;

  ACE_NEW_THROW_EX (event_log_i,
                    TAO_RTEventLog_i (*this,
                                      this->log_mgr_.in (),
                                      this,
                                      this->notifier_,
                                      id,
                                      full_action,
                                      max_size                              
                                      ),
                    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (event_log._retn ());

  PortableServer::ServantBase_var safe_event_log_i = event_log_i;
  // Transfer ownership to POA.  

  event_log_i->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (event_log._retn ());

  //initialise the LogConsumer object
  event_log_i->activate (ACE_ENV_SINGLE_ARG_PARAMETER);

  // Register with the poa
  event_log = event_log_i->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (event_log._retn ());

  // Add to the Hash table..
  if (hash_map_.bind (id, RTEventLogAdmin::EventLog::_duplicate(event_log.in ())) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      RTEventLogAdmin::EventLog::_nil ());

  notifier_->object_creation (RTEventLogAdmin::EventLog::_duplicate(event_log.in ()),
                              id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (RTEventLogAdmin::EventLog::_nil ());

  return event_log._retn ();
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr 
TAO_RTEventLogFactory_i::obtain_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return consumer_admin_->obtain_push_supplier();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr <TAO_RTEventLog_i>;
template class ACE_Auto_Basic_Ptr<TAO_RTEventLog_i>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr <TAO_RTEventLog_i>
#pragma instantiate ACE_Auto_Event_Ptr <TAO_RTEventLog_i>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
